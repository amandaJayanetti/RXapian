#include <Rcpp.h>
#include <xapian.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Rcpp::List convertToDataFrame(Rcpp::List list) {
  Rcpp::List dataFrame = clone(list);
  Rcpp::GenericVector aRow = dataFrame(0);
  Rcpp::StringVector rowNames(aRow.length());
  
  for (int i = 0; i < aRow.length(); ++i) {
    char name[5];
    sprintf(&(name[0]), "%d", i+1);
    rowNames(i) = name;
  }
  
  dataFrame.attr("row.names") = rowNames;
  Rcpp::StringVector colNames(dataFrame.length());
  
  int ascii=97;
  for (int j = 0; j < dataFrame.length(); ++j) {
    char name[6];
    //sprintf(&(name[0]), "%d", j+1);
    sprintf(&(name[0]), "%c", char(ascii++));
    colNames(j) = name;
  }
  
  dataFrame.attr("names") = colNames;
  dataFrame.attr("class") = "data.frame";
  return dataFrame;
}

int columnNo( const string & data){
  int col=0;
  std::istringstream stream(data);
  std::string line;
  while(std::getline(stream, line)) {
    col++;
  }
  return col;
}

string rowVal(const string & data,int row){
  int r=0;
  string val;
  std::istringstream stream(data);
  std::string line;
  while(std::getline(stream, line)) {
    if(r==row){
      val=line;
      break; 
    }
    r++;
  }
  if(val=="")
    return "NA";
  else
    return val;
}

Xapian::Query parseQuery(Rcpp::List & query){
  Xapian::Query returnQuery;
  if(query.containsElementNamed("queryString")){
    
    Xapian::QueryParser queryparser;
    
    if(query.containsElementNamed("stemmer")){
      std::string lang=Rcpp::as<std::string>(query["stemmer"]);
      queryparser.set_stemmer(Xapian::Stem(lang));
    }
    
    if(query.containsElementNamed("strategy")){
      //maybe an if else should be used here
    }
    else
      queryparser.set_stemming_strategy(queryparser.STEM_SOME);
    
    if(query.containsElementNamed("prefix.fields")){
      Rcpp::List prefixFields=query["prefix.fields"];
      for( Rcpp::List::iterator it = prefixFields.begin(); it != prefixFields.end(); ++it ) {
        Rcpp::List list=it.operator*();
        std::string prefix=Rcpp::as<std::string>(list["prefix"]);
        std::string fieldName=Rcpp::as<std::string>(list["name"]);
        queryparser.add_prefix(fieldName, prefix);
      }
    }
    
    std::string aQuery=Rcpp::as<std::string>(query["queryString"]);
    returnQuery = queryparser.parse_query(aQuery);
  }
  
  return returnQuery;
}

//' Search wrapper
//'
//' @param dbpath	path to a Xapian database
//' @param queryList    a list of data to create a Xapian::Query 
//' 
//' @examples
//' \dontrun{
//' db<- c("path/to/database")
//' query<-list(queryString="watch",
//'             prefix.fields=list(list(prefix="S",name="title"),
//'                           list(prefix="XD",name="description")),
//'             stemmer="en")
//' searchWrapper(db,query)
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::List searchWrapper(Rcpp::CharacterVector & dbpath,Rcpp::List & queryList){
  Rcpp::List aQuery=queryList;
  std::string path=Rcpp::as<std::string>(dbpath);
  
  // Open the Xapian::Database that is to be searched 
  Xapian::Database db(path);
 
  // Create a Xapian::Query from user specified data
  Xapian::Query query = parseQuery(aQuery);
  
  //Use a Xapian::Enquire object on the Xapian::Database to run the query
  Xapian::Enquire enquire(db);
  enquire.set_query(query);
  
  // offset - defines starting point within result set
  // pagesize - defines number of records to retrieve
  Xapian::doccount offset = 0;
  Xapian::doccount pagesize = 10;
  
  //Create and return a data.frame with results for the searched query
  
  Xapian::MSet mset = enquire.get_mset(offset, pagesize);
  
  int cols=0;
  for (Xapian::MSetIterator m = mset.begin(); m != mset.end(); ++m) {
    const string & data = m.get_document().get_data();
    cols=columnNo(data);
    break;
  }
  
  Rcpp::List output;
  for(int i=0;i<cols;i++){
    Rcpp::List rowVec;
    for (Xapian::MSetIterator m = mset.begin(); m != mset.end(); ++m) {
      const string & data = m.get_document().get_data();
      string aVal=rowVal(data,i);
      rowVec.push_back(aVal);
    }
    stringstream ss;
    ss << i;
    string str = ss.str();
    output[str]=rowVec;
  }
  
  return convertToDataFrame(output);
  
}
