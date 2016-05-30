#include <Rcpp.h>
#include <xapian.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int rowCount(Rcpp::DataFrame & dataframe){
  Rcpp::DataFrame df=dataframe;
  Rcpp::NumericVector aCol = df[0];
  int rows = aCol.size();
  return rows;
}

int colCount(Rcpp::DataFrame & dataframe){
  Rcpp::DataFrame df=dataframe;
  int cols=df.size();
  return cols;
}

Rcpp::StringVector colNames(Rcpp::DataFrame & dataframe){
  Rcpp::DataFrame df=dataframe;
  Rcpp::StringVector nameVec=df.attr("names");
  return nameVec;
}

Rcpp::String columnName(Rcpp::DataFrame & dataFrame,int colIndex){
  Rcpp::DataFrame df=dataFrame;
  int index=colIndex;
  Rcpp::StringVector columns=colNames(df);
  return columns[index];
}

int colIndex(Rcpp::DataFrame & dataFrame,string colName){
  Rcpp::DataFrame df=dataFrame;
  string name=colName;
  Rcpp::StringVector columns=colNames(df);
  int index=0;
  for(Rcpp::StringVector::iterator it = columns.begin(); it != columns.end(); ++it) {
    string col=Rcpp::as<std::string>(*it);
    if(name==col){
      break;
    }
    index++;
  }
  return index;
}

Rcpp::StringVector parseRow(Rcpp::DataFrame & dataframe,int row){
  Rcpp::DataFrame df=dataframe;
  int rowNo=row;
  int nRows = rowCount(df);
  int nCols=df.size();
  Rcpp::StringVector rowVec;
  
  for(int i=0;i<nRows;i++){
    if(i==rowNo){
      for (int j=0; j<nCols;j++) {
        Rcpp::StringVector column = df[j] ;  //selecting jth column of data frame
        rowVec.push_back(Rcpp::as<string>(column[i]));
      }
      break;
    }
  }
  return rowVec;  
}

vector<std::string> parseField(const string & field,const char & separator){
  string aField=field;
  const char aSeparator=separator;
  vector<std::string> result;
  size_t character = 0;
  do {
    size_t start = character;
    character = aField.find(aSeparator, character);
    size_t len = character - start;
    string aField2 = Xapian::Unicode::tolower(aField.substr(start, len));
    size_t trim = aField2.find_last_not_of(" \t");
    if (trim != string::npos) {
      aField2.resize(trim + 1);
      trim = aField2.find_first_not_of(" \t");
      if (trim)
        aField2.erase(0, trim);
      result.push_back(aField2); 
    }
  } while (character++ != string::npos);
  return result;
}

//' indexing wrapper.
//'
//' @param dbpath	path to a Xapian database
//' @param dataFrame	data frame to be indexed
//' @param idColumn	index of a column in the data frame whose row value will be used as a unique identifier to each document(row).
//' @param indexFields	list of 'list of fields' that will be indexed using a Xapian::TermGenerator
//' @param stemmer 	the stemmer that should be applied to the Xapian::TermGenerator
//'
//' @export
//'
//'
// [[Rcpp::export]]
void index(Rcpp::CharacterVector & dbpath,Rcpp::DataFrame & dataFrame,Rcpp::NumericVector & idColumn,Rcpp::List & indexFields,Rcpp::CharacterVector & stemmer){
  
  std::string path=Rcpp::as<std::string>(dbpath);
  Rcpp::DataFrame df=dataFrame;
  int id=Rcpp::as<int>(idColumn);
  int dfCols=colCount(df);
  if(id<0 || id>dfCols)
    Rcpp::stop("Invalid argument for indexCol");
  Rcpp::List indexList=indexFields;
  
  
  Xapian::WritableDatabase db(path, Xapian::DB_CREATE_OR_OPEN);
  Xapian::TermGenerator termgenerator;
  
  //Add a stemmer to Xapian::Termgenerator
  if(stemmer){
    std::string lang=Rcpp::as<std::string>(stemmer);
    termgenerator.set_stemmer(Xapian::Stem(lang));
  }
  
  int nRows = rowCount(df);
  
  // Extract data in dataFrame's rows and add them as Xapian::Documents to the Xapian::Database
  for(int i=0;i<nRows;i++){
    
    Rcpp::StringVector dfRow=parseRow(df,i);
    Xapian::Document doc;
    termgenerator.set_document(doc);
    string identifier=Rcpp::as<std::string>(dfRow[id]);
    string docString=identifier;
    int listPos=0;
    
    //add text and prefixes to Xapian::Termgenerator
    for( Rcpp::List::iterator it = indexList.begin(); it != indexList.end(); ++it ) {
      Rcpp::List list=it.operator*();
      int colNo;
      
      if(list.containsElementNamed("prefix")){
        try{
          if(list.containsElementNamed("index")){
            colNo=Rcpp::as<int>(list["index"]);
          }
          else{
            std::string colName=Rcpp::as<std::string>(list["name"]); 
            colNo=colIndex(df,colName);
          }
        }
        catch(...){
          Rcpp::stop("Either name or index of the columns should be specified.");
        }
        
        if(colNo<0 || colNo>dfCols)
          Rcpp::stop("Invalid argument for indexFields");
        std::string prefix=Rcpp::as<std::string>(list["prefix"]);
        const string & field =Rcpp::as<std::string>(dfRow[colNo]);
        termgenerator.index_text(field, 1,prefix);
      }
    }
    
    for( Rcpp::List::iterator it = indexList.begin(); it != indexList.end(); ++it ) {
      Rcpp::List list=it.operator*();
      int colNo;
      
      try{
        if(list.containsElementNamed("index")){
          colNo=Rcpp::as<int>(list["index"]);
        }
        else{
          std::string colName=Rcpp::as<std::string>(list["name"]); 
          colNo=colIndex(df,colName);
        }
      }
      catch(...){
        Rcpp::stop("Either name or index of the columns should be specified.");
      }
      if(colNo<0 || colNo>dfCols)
        Rcpp::stop("Invalid argument for indexFields");
      const string & field =Rcpp::as<std::string>(dfRow[colNo]);
      termgenerator.index_text(field);
      termgenerator.increase_termpos();
      docString = docString +"\n"+field;
    }
    
    doc.set_data(docString);
    string idterm = "Q" + identifier;
    doc.add_boolean_term(idterm);
    db.replace_document(idterm, doc); 
  }
  
}
