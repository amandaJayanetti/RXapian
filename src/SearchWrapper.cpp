#include <Rcpp.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <xapian.h>

using namespace std;

Rcpp::List
convertToDataFrame(Rcpp::List list)
{
    Rcpp::List dataFrame = clone(list);
    Rcpp::GenericVector aRow = dataFrame(0);
    Rcpp::StringVector rowNames(aRow.length());

    for (int i = 0; i < aRow.length(); ++i) {
	char name[5];
	snprintf(name, 5, "%d", i + 1);
	rowNames(i) = name;
    }

    dataFrame.attr("row.names") = rowNames;
    Rcpp::StringVector colNames(dataFrame.length());

    // Name columns from 'a' upwards
    int ascii;
    for (int j = 0, ascii = 97; j < dataFrame.length(); ++j, ++ascii) {
	char name[6];
	snprintf(name, 6, "%c", ascii);
	colNames(j) = name;
    }

    dataFrame.attr("names") = colNames;
    dataFrame.attr("class") = "data.frame";
    return dataFrame;
}

int
lineCount(const string & data)
{
    int col = 0;
    std::istringstream stream(data);
    std::string line;
    while (std::getline(stream, line)) {
	col++;
    }
    return col;
}

string
rowVal(const string & data, int row)
{
    int r = 0;
    string val;
    std::istringstream stream(data);
    std::string line;
    while (std::getline(stream, line)) {
	if (r == row) {
	    val = line;
	    break;
	}
	r++;
    }
    if (val == "")
	return "NA";
    else
	return val;
}

Xapian::Query
parseQuery(Rcpp::List & query)
{
    Xapian::Query returnQuery;
    if (query.containsElementNamed("queryString")) {

	Xapian::QueryParser queryparser;

	if (query.containsElementNamed("stemmer")) {
	    std::string lang = Rcpp::as<std::string>(query["stemmer"]);
	    queryparser.set_stemmer(Xapian::Stem(lang));
	}

	if (query.containsElementNamed("strategy")) {
	    // maybe an if else should be used here
	} else
	    queryparser.set_stemming_strategy(queryparser.STEM_SOME);

	if (query.containsElementNamed("prefix.fields")) {
	    Rcpp::List prefixFields = query["prefix.fields"];
	    for (Rcpp::List::iterator it = prefixFields.begin(); it != prefixFields.end(); ++it) {
		Rcpp::List list = it.operator*();
		std::string prefix = Rcpp::as<std::string>(list["prefix"]);
		std::string fieldName = Rcpp::as<std::string>(list["name"]);
		queryparser.add_prefix(fieldName, prefix);
	    }
	}

	std::string aQuery = Rcpp::as<std::string>(query["queryString"]);
	returnQuery = queryparser.parse_query(aQuery);
    }

    return returnQuery;
}

std::vector<Xapian::ValueCountMatchSpy *>
parseSpy(Xapian::Enquire enquire, Rcpp::NumericVector & spy)
{
    std::vector<Xapian::ValueCountMatchSpy *> aList;
    for (Rcpp::NumericVector::iterator itr = spy.begin(); itr != spy.end(); ++itr) {
	Xapian::ValueCountMatchSpy * aSpy = new Xapian::ValueCountMatchSpy(*itr);
	Rcpp::Rcout << *itr << std::endl;
	enquire.add_matchspy(aSpy);
	aList.push_back(aSpy);
    }
    return aList;
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
//' enq<-list(matchspy=c(0,1))
//' searchWrapper(db,enq,query)
//' }
//'
//' @return
// [[Rcpp::export]]
Rcpp::List
searchWrapper(Rcpp::CharacterVector & dbpath, Rcpp::List & enquireList, Rcpp::List & queryList)
{
    std::string path = Rcpp::as<std::string>(dbpath);

    // Open the Xapian::Database that is to be searched
    Xapian::Database db(path);

    // Create a Xapian::Query from user specified data
    Xapian::Query query = parseQuery(queryList);

    // Use a Xapian::Enquire object on the Xapian::Database to run the query
    Xapian::Enquire enquire(db);
    enquire.set_query(query);

    // Set up a spy to inspect a particular value at specified slot
    std::vector<Xapian::ValueCountMatchSpy *> spy;
    if (enquireList.containsElementNamed("matchspy")) {
	Rcpp::NumericVector spyVec = enquireList["matchspy"];
	spy = parseSpy(enquire, spyVec);
    }

    // first- defines starting point within result set
    // pagesize - defines number of records to retrieve
    Xapian::doccount first = 0;
    Xapian::doccount maxitems = 10;
    Xapian::doccount checkatleast = 0;

    if (enquireList.containsElementNamed("first")) first = Rcpp::as<int>(enquireList["first"]);
    if (enquireList.containsElementNamed("first")) maxitems = Rcpp::as<int>(enquireList["maxitems"]);
    if (enquireList.containsElementNamed("checkatleast")) checkatleast = Rcpp::as<int>(enquireList["checkatleast"]);

    // Create and return a data.frame with results for the searched query

    Xapian::MSet mset = enquire.get_mset(first, maxitems, checkatleast);
    if (!(mset.begin() == mset.end())) {
	int cols = 0;
	const string & data = mset.begin().get_document().get_data();
	cols = lineCount(data);

	Rcpp::List output;
	for (int i = 0; i < cols; i++) {
	    Rcpp::List rowVec;
	    for (Xapian::MSetIterator m = mset.begin(); m != mset.end(); ++m) {
		const string & data = m.get_document().get_data();
		string aVal = rowVal(data, i);
		rowVec.push_back(aVal);
	    }
	    stringstream ss;
	    ss << i;
	    string str = ss.str();
	    output[str] = rowVec;
	}

	bool displaySpies = true;
	if (enquireList.containsElementNamed("viewSpies")) displaySpies = Rcpp::as<bool>(enquireList["viewSpies"]);

	if (displaySpies) {
	    // Fetch and display the spy values
	    for (std::vector<Xapian::ValueCountMatchSpy *>::iterator itr = spy.begin(); itr != spy.end(); ++itr) {
		Rcpp::Rcout << "new spy" << std::endl;
		Xapian::ValueCountMatchSpy * aSpy = *itr;
		for (Xapian::TermIterator facet = aSpy->values_begin(); facet != aSpy->values_end(); ++facet) {
		    Rcpp::Rcout << "Facet: " << *facet << "; count: " << facet.get_termfreq() << std::endl;
		}
	    }
	}

	return convertToDataFrame(output);
    } else
	return NULL;
}
