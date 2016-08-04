#include <Rcpp.h>
#include <xapian.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int
rowCount(Rcpp::DataFrame & dataframe)
{
    Rcpp::DataFrame df = dataframe;
    Rcpp::NumericVector aCol = df[0];
    int rows = aCol.size();
    return rows;
}

int
colCount(Rcpp::DataFrame & dataframe)
{
    Rcpp::DataFrame df = dataframe;
    int cols = df.size();
    return cols;
}

Rcpp::StringVector
colNames(Rcpp::DataFrame & dataframe)
{
    Rcpp::DataFrame df = dataframe;
    Rcpp::StringVector nameVec = df.attr("names");
    return nameVec;
}

Rcpp::String
columnName(Rcpp::DataFrame & dataFrame, int colIndex)
{
    Rcpp::DataFrame df = dataFrame;
    int index = colIndex;
    Rcpp::StringVector columns = colNames(df);
    return columns[index];
}

int
colIndex(Rcpp::DataFrame & dataFrame, string colName)
{
    Rcpp::DataFrame df = dataFrame;
    string name = colName;
    Rcpp::StringVector columns = colNames(df);
    int index = 0;
    for (Rcpp::StringVector::iterator it = columns.begin(); it != columns.end(); ++it) {
	string col = Rcpp::as<std::string>(*it);
	if (name == col) {
	    break;
	}
	index++;
    }
    return index;
}

Rcpp::StringVector
parseRow(Rcpp::DataFrame & dataframe, int row)
{
    Rcpp::DataFrame df = dataframe;
    int rowNo = row;
    int nRows = rowCount(df);
    int nCols = df.size();
    Rcpp::StringVector rowVec;

    for (int i = 0; i < nRows; i++) {
	if (i == rowNo) {
	    for (int j = 0; j < nCols; j++) {
		Rcpp::StringVector column = df[j]; //selecting jth column of data frame
		rowVec.push_back(Rcpp::as<string>(column[i]));
	    }
	    break;
	}
    }
    return rowVec;
}

vector<std::string>
parseField(const string & field, const char & separator)
{
    string aField = field;
    const char aSeparator = separator;
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
	    if (trim) aField2.erase(0, trim);
	    result.push_back(aField2);
	}
    } while (character++ != string::npos);
    return result;
}

//' indexing wrapper
//'
//' @param dbpath	path to a Xapian database
//' @param dataFrame	data frame to be indexed
//' @param idColumn	index of a column in the data frame whose row value will be used as a unique identifier to each document(row)
//' @param indexFields	list of 'list of fields' that will be indexed using a Xapian::TermGenerator
//' @param filterFields	list with information on boolean terms to be added to Xapian::Document
//' @param stemmer 	the stemmer that should be applied to the Xapian::TermGenerator
//' @param valueSlots list of arguments required for advanced indexing
//' @examples
//' \dontrun{
//' db<- c("path/to/database")
//' data<-read.csv("path/to/csv") or data<-data.frame(..)
//' id<-c(0)
//' indexFields<-list(list(index=2,prefix="S"),list(index=8,prefix="XD"))
//' filterFields<-list(list(index=6,prefix="XM",separator=";"))
//' indexWrapper(db,data,id,indexFields,filterFields,"en")
//' }
//'
//' @return none
// [[Rcpp::export]]
void
indexWrapper(Rcpp::CharacterVector & dbpath, Rcpp::DataFrame & dataFrame, Rcpp::NumericVector & idColumn,
             Rcpp::List & indexFields, Rcpp::List & filterFields, Rcpp::List & valueSlots,
             Rcpp::CharacterVector & stemmer)
{

    std::string path = Rcpp::as<std::string>(dbpath);
    Rcpp::DataFrame df = dataFrame;
    int id = Rcpp::as<int>(idColumn);
    int dfCols = colCount(df);
    if (id < 0 || id > dfCols) Rcpp::stop("Invalid argument for indexCol");
    Rcpp::List indexList = indexFields;
    Rcpp::List filterList = filterFields;

    // Create or open the Xapian::Database at the specified location
    Xapian::WritableDatabase db(path, Xapian::DB_CREATE_OR_OPEN);

    // Set up a Xapian::Termgenerator to be used in indexing.
    Xapian::TermGenerator termgenerator;

    //Add a stemmer to Xapian::Termgenerator
    if (stemmer) {
	std::string lang = Rcpp::as<std::string>(stemmer);
	termgenerator.set_stemmer(Xapian::Stem(lang));
    }

    int nRows = rowCount(df);

    // Extract data in data.frame's rows and add them as Xapian::Documents to the Xapian::Database
    for (int i = 0; i < nRows; i++) {

	// Extract a row from the data.frame
	Rcpp::StringVector dfRow = parseRow(df, i);

	//Create a document and add it to the Xapian::Termgenerator
	Xapian::Document doc;
	termgenerator.set_document(doc);

	string identifier = Rcpp::as<std::string>(dfRow[id]);
	string docString = identifier;

	// Index each column of the data.frame with a suitable prefix
	for (Rcpp::List::iterator it = indexList.begin(); it != indexList.end(); ++it) {
	    Rcpp::List list = it.operator*();
	    int colNo;

	    if (list.containsElementNamed("prefix")) {
		try {
		    if (list.containsElementNamed("index")) {
			colNo = Rcpp::as<int>(list["index"]);
		    } else {
			std::string colName = Rcpp::as<std::string>(list["name"]);
			colNo = colIndex(df, colName);
		    }
		} catch (...) {
		    Rcpp::stop("Either name or index of the columns should be specified.");
		}

		if (colNo < 0 || colNo > dfCols) Rcpp::stop("Invalid argument for indexFields");
		std::string prefix = Rcpp::as<std::string>(list["prefix"]);
		const string & field = Rcpp::as<std::string>(dfRow[colNo]);
		termgenerator.index_text(field, 1, prefix);
	    }
	}

	// Index columns without prefixes for general search
	for (Rcpp::List::iterator it = indexList.begin(); it != indexList.end(); ++it) {
	    Rcpp::List list = it.operator*();
	    int colNo;

	    try {
		if (list.containsElementNamed("index")) {
		    colNo = Rcpp::as<int>(list["index"]);
		} else {
		    std::string colName = Rcpp::as<std::string>(list["name"]);
		    colNo = colIndex(df, colName);
		}
	    } catch (...) {
		Rcpp::stop("Either name or index of the columns should be specified.");
	    }
	    if (colNo < 0 || colNo > dfCols) Rcpp::stop("Invalid argument for indexFields");
	    const string & field = Rcpp::as<std::string>(dfRow[colNo]);
	    termgenerator.index_text(field);
	    termgenerator.increase_termpos();
	    docString = docString + "\n" + field;
	}

	//Add boolean terms to Xapian::Document
	for (Rcpp::List::iterator it = filterList.begin(); it != filterList.end(); ++it) {
	    Rcpp::List list = it.operator*();
	    int colNo;

	    try {
		if (list.containsElementNamed("index")) {
		    colNo = Rcpp::as<int>(list["index"]);
		} else {
		    std::string colName = Rcpp::as<std::string>(list["name"]);
		    colNo = colIndex(df, colName);
		}
	    } catch (...) {
		Rcpp::stop("Either name or index of the columns should be specified.");
	    }

	    if (colNo < 0 || colNo > dfCols) Rcpp::stop("Invalid argument for indexFields");
	    const string & field = Rcpp::as<std::string>(dfRow[colNo]);
	    const string & prefix = Rcpp::as<std::string>(list["prefix"]);
	    vector<std::string> fields;
	    if (list.containsElementNamed("separator")) {
		const char & separator = Rcpp::as<char>(list["separator"]);
		fields = parseField(field, separator);
	    } else {
		fields.push_back(field);
	    }
	    for (std::vector<string>::iterator itr = fields.begin(); itr != fields.end(); ++itr) {
		string material = *itr;
		doc.add_boolean_term(prefix + material);
	    }
	}

	//Add value slots
	for (Rcpp::List::iterator it = valueSlots.begin(); it != valueSlots.end(); ++it) {
	    Rcpp::List list = *it;
	    bool serialiseVal = false;
	    bool override_frame_values = false; // use original row values in data frame
	    int slot;
	    std::string val;

	    if (list.containsElementNamed("slot")) {
		try {
		    slot = Rcpp::as<int>(list["slot"]);
		} catch (...) {
		    Rcpp::stop("Invalid argument for slot");
		}
	    } else
		Rcpp::stop("Slot number should be specified.");

	    if (list.containsElementNamed("serialise")) serialiseVal = Rcpp::as<bool>(list["serialise"]);
	    if (list.containsElementNamed("values"))
		override_frame_values = true; // use a column vector of modified row values

	    if (!override_frame_values) {
		int colNo;

		if (list.containsElementNamed("index")) {
		    try {
			colNo = Rcpp::as<int>(list["index"]);
			if (colNo < 0 || colNo > dfCols)
			    Rcpp::stop(
			        "Invalid argument: index should correspond to a column number in the data frame");
		    } catch (...) {
			Rcpp::stop("Invalid argument for index");
		    }
		} else if (list.containsElementNamed("name")) {
		    std::string colName;
		    try {
			colName = Rcpp::as<std::string>(list["name"]);
		    } catch (...) {
			Rcpp::stop("Invalid argument for name");
		    }
		    colNo = colIndex(df, colName);
		} else
		    Rcpp::stop("Either name or index of the columns should be specified.");

		const string & field = Rcpp::as<std::string>(dfRow[colNo]);

		if (serialiseVal) {
		    double dVal;
		    try {
			stringstream ss(field);
			ss >> dVal;
		    } catch (...) {
			Rcpp::stop("Error in parsing data frame field as a double");
		    }
		    doc.add_value(slot, Xapian::sortable_serialise(dVal));
		} else {
		    doc.add_value(slot, field);
		}
	    } else {
		std::string vecType = Rcpp::as<std::string>(list["type"]);
		if (vecType == "double") {
		    Rcpp::DoubleVector dVec = list["values"];
		    double dVal = dVec[i];
		    if (!(Rcpp::DoubleVector::is_na(dVal))) doc.add_value(slot, Xapian::sortable_serialise(dVal));
		} else {
		    Rcpp::StringVector sVec = list["values"];
		    std::string sVal = Rcpp::as<std::string>(sVec[i]);
		    if (serialiseVal) {
			double dVal;
			try {
			    stringstream ss(sVal);
			    ss >> dVal;
			} catch (...) {
			    Rcpp::stop("Error in parsing data frame field as a double");
			}
			doc.add_value(slot, Xapian::sortable_serialise(dVal));
		    } else {
			doc.add_value(slot, sVal);
		    }
		}
	    }
	}

	//Store all the columns for display purposes
	doc.set_data(docString);

	//Use an identifier to ensure each object ends up in the Xapian::Database only once
	//even if xapian_index() function is called multiple times
	string idterm = "Q" + identifier;
	doc.add_boolean_term(idterm);
	db.replace_document(idterm, doc);
    }
}
