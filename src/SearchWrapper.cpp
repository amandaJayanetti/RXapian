#include "config.h"
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

#if defined(XAPIAN_RANGE_PROCESSOR_SUPPORT)

	// check for RangeProcessors
	if (query.containsElementNamed("VRP")) {
	    Rcpp::List processors = query["VRP"];

	    for (Rcpp::List::iterator it = processors.begin(); it != processors.end(); ++it) {
		Rcpp::List list = *it;
		string type = Rcpp::as<std::string>(list["type"]);

		// if NumberRangeProcessor
		if (!type.compare("proc.num")) {
		    int aVal = list["value"];

		    if (list.containsElementNamed("check.str")) {
			string aStr = Rcpp::as<std::string>(list["check.str"]);

			unsigned flags = 0;
			Rcpp::StringVector flagVec;
			if (list.containsElementNamed("flags")) {
			    flagVec = list["flags"];
			    for (Rcpp::StringVector::iterator itr = flagVec.begin(); itr != flagVec.end(); ++itr) {
				std::string aflag = Rcpp::as<std::string>(*itr);
				if (!aflag.compare("RP_SUFFIX"))
				    flags = flags | Xapian::RP_SUFFIX;
				else
				    flags = flags | Xapian::RP_REPEATED;
			    }
			}
			Xapian::NumberRangeProcessor * num_proc = new Xapian::NumberRangeProcessor(aVal, aStr, flags);
			queryparser.add_rangeprocessor(num_proc);
		    } else {
			Xapian::NumberRangeProcessor * num_proc = new Xapian::NumberRangeProcessor(aVal);
			queryparser.add_rangeprocessor(num_proc);
		    }
		}

		// if RangeProcessor
		if (!type.compare("proc.str")) {
		    int aVal = list["value"];
		    if (list.containsElementNamed("check.str")) {
			string aStr = Rcpp::as<std::string>(list["check.str"]);

			unsigned flags = 0;
			Rcpp::StringVector flagVec;
			if (list.containsElementNamed("flags")) {
			    flagVec = list["flags"];
			    for (Rcpp::StringVector::iterator itr = flagVec.begin(); itr != flagVec.end(); ++itr) {
				std::string aflag = Rcpp::as<std::string>(*itr);
				if (!aflag.compare("RP_SUFFIX"))
				    flags = flags | Xapian::RP_SUFFIX;
				else
				    flags = flags | Xapian::RP_REPEATED;
			    }
			}
			Xapian::RangeProcessor * str_proc = new Xapian::RangeProcessor(aVal, aStr, flags);
			queryparser.add_rangeprocessor(str_proc);
		    } else {
			Xapian::RangeProcessor * str_proc = new Xapian::RangeProcessor(aVal);
			queryparser.add_rangeprocessor(str_proc);
		    }
		}

		// if dateRangeProcessor
		if (!type.compare("proc.date")) {
		    int aVal = list["value"];
		    int epoch_year_ = 1970;
		    if (list.containsElementNamed("epoch_year")) epoch_year_ = list["epoch_year"];

		    unsigned flags = 0;
		    Rcpp::StringVector flagVec;
		    if (list.containsElementNamed("flags")) {
			flagVec = list["flags"];
			for (Rcpp::StringVector::iterator itr = flagVec.begin(); itr != flagVec.end(); ++itr) {
			    std::string aflag = Rcpp::as<std::string>(*itr);
			    if (!aflag.compare("RP_SUFFIX"))
				flags = flags | Xapian::RP_SUFFIX;
			    else if ((!aflag.compare("RP_REPEATED")))
				flags = flags | Xapian::RP_REPEATED;
			    else
				flags = flags | Xapian::RP_DATE_PREFER_MDY;
			}
		    }

		    if (list.containsElementNamed("check.str")) {
			std::string aStr = Rcpp::as<std::string>(list["check.str"]);

			Xapian::DateRangeProcessor * date_proc =
			    new Xapian::DateRangeProcessor(aVal, aStr, flags, epoch_year_);
			queryparser.add_rangeprocessor(date_proc);
		    } else {
			Xapian::DateRangeProcessor * date_proc =
			    new Xapian::DateRangeProcessor(aVal, flags, epoch_year_);
			queryparser.add_rangeprocessor(date_proc);
		    }
		}
	    }
	}

#endif

	std::string aQuery = Rcpp::as<std::string>(query["queryString"]);
	returnQuery = queryparser.parse_query(aQuery);
	return returnQuery;
    }

    if (query.containsElementNamed("tname")) {
	string term = Rcpp::as<std::string>(query["tname"]);
	Xapian::termcount aWqf = 1;
	Xapian::termpos aPos = 0;
	if (query.containsElementNamed("wqf")) aWqf = Rcpp::as<int>(query["wqf"]);
	if (query.containsElementNamed("pos")) aPos = Rcpp::as<int>(query["pos"]);
	return Xapian::Query(term, aWqf, aPos);
    }

    if (query.containsElementNamed("OP")) {
	Xapian::Query::op aOP;
	string qOP = Rcpp::as<std::string>(query["OP"]);
	if (qOP == "OP_AND")
	    aOP = Xapian::Query::OP_AND;
	else if (qOP == "OP_OR")
	    aOP = Xapian::Query::OP_OR;
	else if (qOP == "OP_AND_MAYBE")
	    aOP = Xapian::Query::OP_AND_MAYBE;
	else if (qOP == "OP_AND_NOT")
	    aOP = Xapian::Query::OP_AND_NOT;
	else if (qOP == "OP_ELITE_SET")
	    aOP = Xapian::Query::OP_ELITE_SET;
	else if (qOP == "OP_FILTER")
	    aOP = Xapian::Query::OP_FILTER;
#if defined(XAPIAN_RANGE_PROCESSOR_SUPPORT)
	else if (qOP == "OP_MAX")
	    aOP = Xapian::Query::OP_MAX;
#endif
	else
	    aOP = Xapian::Query::OP_NEAR;

	if (query.containsElementNamed("left")) {
	    string left = Rcpp::as<std::string>(query["left"]);
	    string right = Rcpp::as<std::string>(query["right"]);
	    return Xapian::Query(aOP, left, right);
	}

	if (query.containsElementNamed("queries")) {
	    Rcpp::List queryL = query["queries"];
	    std::vector<Xapian::Query> aList;
	    for (Rcpp::List::iterator it = queryL.begin(); it != queryL.end(); ++it) {
		Rcpp::List list = it.operator*();
		aList.push_back(parseQuery(list));
	    }
	    return Xapian::Query(aOP, aList.begin(), aList.end());
	}

	if (query.containsElementNamed("query.left")) {
	    Rcpp::List qLeft = query["query.left"];
	    Rcpp::List qRight = query["query.right"];
	    return Xapian::Query(aOP, parseQuery(qLeft), parseQuery(qRight));
	}

	if (query.containsElementNamed("slot")) {
	    Xapian::valueno aSlot = Rcpp::as<int>(query["slot"]);

	    if (query.containsElementNamed("begin")) {
		string sBegin = Rcpp::as<std::string>(query["begin"]);
		string sEnd = Rcpp::as<std::string>(query["end"]);
		return Xapian::Query(aOP, aSlot, sBegin, sEnd);
	    }

	    if (query.containsElementNamed("value")) {
		std::string aVal = Rcpp::as<std::string>(query["value"]);
		return Xapian::Query(aOP, aSlot, aVal);
	    }
	}
    }

    return returnQuery;
}

std::vector<Xapian::ValueCountMatchSpy *>
parseSpy(Xapian::Enquire enquire, Rcpp::NumericVector & spy)
{
    std::vector<Xapian::ValueCountMatchSpy *> aList; // This should be a list of
    // lists-> caz there could be
    // multiple facets

    for (Rcpp::NumericVector::iterator itr = spy.begin(); itr != spy.end(); ++itr) {
	int slot = *itr;
	Xapian::ValueCountMatchSpy * aSpy = new Xapian::ValueCountMatchSpy(slot);
	enquire.add_matchspy(aSpy);
	aList.push_back(aSpy);
    }
    return aList;
}

void
parseEnquire(Xapian::Enquire enquire, Rcpp::List & enquireList)
{
  if(enquireList.containsElementNamed("weighting_scheme")){
    std::string scheme=Rcpp::as<std::string>(enquireList["weighting_scheme"]);
    Xapian::Registry *reg=new Xapian::Registry();
    const Xapian::Weight *weight=reg->get_weighting_scheme(scheme);
    enquire.set_weighting_scheme(*weight);
  }

    if (enquireList.containsElementNamed("docid_order")) {
	std::string order = Rcpp::as<std::string>(enquireList["docid_order"]);
	if (!order.compare("ASCENDING"))
	    enquire.set_docid_order(Xapian::Enquire::ASCENDING);
	else if (!order.compare("DESCENDING"))
	    enquire.set_docid_order(Xapian::Enquire::DESCENDING);
	else
	    enquire.set_docid_order(Xapian::Enquire::DONT_CARE);
    }

    if (enquireList.containsElementNamed("sortby")) {
	std::string sortby = Rcpp::as<std::string>(enquireList["sortby"]);

	if (!sortby.compare("relevance")) {
	    enquire.set_sort_by_relevance();
	} else {
	    bool reverse_sort_order;
	    try {
		reverse_sort_order = Rcpp::as<bool>(enquireList["reverse_sort_order"]);
	    } catch (...) {
		Rcpp::stop("reverse_sort_order parameter is not set or is invalid");
	    }

	    if (enquireList.containsElementNamed("keyMaker")) {

		Xapian::MultiValueKeyMaker * keyMaker = new Xapian::MultiValueKeyMaker();
		Rcpp::List keyList = enquireList["keyMaker"];

		for (Rcpp::List::iterator it = keyList.begin(); it != keyList.end(); ++it) {

		    Rcpp::List list = *it;
		    int slot;

		    try {
			slot = Rcpp::as<int>(list["slot"]);
		    } catch (...) {
			Rcpp::stop("slot parameter is not set or is invalid");
		    }

		    bool reverse = false;

		    if (list.containsElementNamed("reverse")) reverse = Rcpp::as<bool>(list["reverse"]);

		    keyMaker->add_value(slot, reverse);
		}

		if (!sortby.compare("key"))
		    enquire.set_sort_by_key(keyMaker, reverse_sort_order);
		else if (!sortby.compare("relevance_then_key"))
		    enquire.set_sort_by_relevance_then_key(keyMaker, reverse_sort_order);
		else if (!sortby.compare("key_then_relevance"))
		    enquire.set_sort_by_key_then_relevance(keyMaker, reverse_sort_order);

	    } else if (enquireList.containsElementNamed("valueNo")) {
		int valueNo = Rcpp::as<int>(enquireList["valueNo"]);

		if (!sortby.compare("value"))
		    enquire.set_sort_by_value(valueNo, reverse_sort_order);
		else if (!sortby.compare("relevance_then_value"))
		    enquire.set_sort_by_relevance_then_value(valueNo, reverse_sort_order);
		else if (!sortby.compare("value_then_relevance"))
		    enquire.set_sort_by_value_then_relevance(valueNo, reverse_sort_order);
	    }
	}
    }
}

//' Search wrapper
//'
//' @param dbpath	path to a Xapian database
//' @param queryList    a list of data to create a Xapian::Query
//' @param enquireList list of arguments for Xapian::Enquire
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
    bool returnSpy = false;
    Rcpp::NumericVector spyVec;
    if (enquireList.containsElementNamed("return.spy")) {
	spyVec = enquireList["return.spy"];
	spy = parseSpy(enquire, spyVec);
	returnSpy = true;
    }
    
    parseEnquire(enquire,enquireList);

    // first- defines starting point within result set
    // pagesize - defines number of records to retrieve
    Xapian::doccount first = 0;
    Xapian::doccount maxitems = 10;
    Xapian::doccount checkatleast = 0;

    if (enquireList.containsElementNamed("first")) first = Rcpp::as<int>(enquireList["first"]);
    if (enquireList.containsElementNamed("maxitems")) maxitems = Rcpp::as<int>(enquireList["maxitems"]);
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

	Rcpp::List out = convertToDataFrame(output);

	if (returnSpy) {
	    Rcpp::List outputSpy;
	    int spyNo = 0;
	    for (std::vector<Xapian::ValueCountMatchSpy *>::iterator itr = spy.begin(); itr != spy.end(); ++itr) {
		Xapian::ValueCountMatchSpy * aSpy = *itr;
		std::string aStr;
		Rcpp::List spyList;
		for (Xapian::TermIterator facet = aSpy->values_begin(); facet != aSpy->values_end(); ++facet) {
		    aStr = *facet + "; count: ";
		    int termFreq = facet.get_termfreq();
		    stringstream convert;
		    convert << termFreq;
		    aStr += convert.str();
		    spyList.push_back(aStr);
		}
		double spyIndex = spyVec[spyNo++];
		stringstream ss;
		ss << spyIndex;
		string str = ss.str();
		outputSpy[str] = spyList;
	    }
	    return Rcpp::List::create(Rcpp::Named("output") = out, Rcpp::Named("spies") = outputSpy);
	} else
	    return out;

    } else
	return NULL;
}

