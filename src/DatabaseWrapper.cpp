#include <RcppCommon.h>

#include <xapian.h>

namespace Rcpp {

template <>
SEXP
wrap(const Xapian::Database & db);
template <>
Xapian::Database as(SEXP);
}

#include <Rcpp.h>

namespace Rcpp {

template <>
Xapian::Database
as(SEXP inp)
{
    Rcpp::CharacterVector dbpath(inp);
    std::string pathToDb = as<std::string>(dbpath);
    Xapian::WritableDatabase db(pathToDb, Xapian::DB_CREATE_OR_OPEN);
    return db;
}

//change
template <>
SEXP
wrap(const Xapian::Database & db)
{
    std::string uuid = db.get_uuid().c_str();
    int doccount = db.get_doccount();
    int avlength = db.get_avlength();
    int doc_low = db.get_doclength_lower_bound();
    int doc_up = db.get_doclength_upper_bound();
    int docid = db.get_lastdocid();
    bool hasPos = db.has_positions();

    Rcpp::List database = List::create(Named("uuid", uuid), Named("doccount", doccount), Named("avlength", avlength),
                                       Named("doc_low", doc_low), Named("doc_up", doc_up), Named("docid", docid),
                                       Named("hasPos", hasPos));

    return database;
}
}

//' DatabaseInfo wrapper
//'
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' databaseInfo(dbpath)
//' }
//'
//' @return list of information about the Xapian database at specified location
// [[Rcpp::export]]
Rcpp::List
databaseInfo(SEXP dbpath)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    return Rcpp::wrap(db);
}

//' Database setmetadata wrapper
//'
//' @param dbpath path to a Xapian Database
//' @param key    key of the metadata item to set
//' @param value  value of the metadata item to set
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' key<-c("k1")
//' value<-c("v1")
//' setmetadata(dbpath,key,value)
//' }
//'
//' @return none
// [[Rcpp::export]]
void
setmetadata(SEXP dbpath, SEXP key, SEXP value)
{
    std::string path = Rcpp::as<std::string>(dbpath);
    Xapian::WritableDatabase db(path, Xapian::DB_CREATE_OR_OPEN);
    std::string akey = Rcpp::as<std::string>(key);
    std::string aVal = Rcpp::as<std::string>(value);
    db.set_metadata(akey, aVal);
}

//' Database getmetadata wrapper
//'
//' @param dbpath path to a Xapian Database
//' @param key    key of the metadata item to access
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' key<-c("k1")
//' getmetadata(dbpath,key)
//' }
//'
//' @return user-specified metadata associated with the given key
// [[Rcpp::export]]
Rcpp::StringVector
getmetadata(SEXP dbpath, SEXP key)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    std::string akey = Rcpp::as<std::string>(key);
    return db.get_metadata(akey);
}

//' Database spelling_suggestion wrapper
//'
//' @param dbpath path to a Xapian Database
//' @param arg    potentially misspelled word
//' @examples
//' \dontrun{
//'  dbpath<- c("path/to/database")
//'  arg<-list(word="helllo", max_edit_distance = 3)
//'  spelling_suggestion(dbpath,arg)
//' }
//'
//' @return a spelling correction to a potentially misspelled word
// [[Rcpp::export]]
Rcpp::StringVector
spelling_suggestion(SEXP dbpath, SEXP arg)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    Rcpp::List alist = arg;
    unsigned int dist = 2;
    std::string word;
    try {
	word = Rcpp::as<std::string>(alist["word"]);
    } catch (...) {
	Rcpp::stop("Invalid argument format");
    }
    if (alist.containsElementNamed("max_edit_distance")) {
	dist = Rcpp::as<int>(alist["max_edit_distance"]);
    }
    std::string suggtn = db.get_spelling_suggestion(word, dist);
    return suggtn;
}

//' Database collection_freq wrapper
//'
//' @param dbpath path to a Xapian Database
//' @param tname  term whose collection frequency is being requested
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' tname<-c("test")
//' collection_freq(dbpath,tname)
//' }
//'
//' @return total number of occurrences of the given term
// [[Rcpp::export]]
Rcpp::NumericVector
collection_freq(SEXP dbpath, SEXP tname)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    std::string aTname = Rcpp::as<std::string>(tname);
    int freq = db.get_collection_freq(aTname);
    Rcpp::NumericVector collection_freq;
    collection_freq.push_back(freq);
    return collection_freq;
}

//' Database value_freq wrapper
//'
//' @param dbpath path to a Xapian Database
//' @param slot   value slot to examine
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' slot<-c(1)
//' value_freq(dbpath,slot)
//' }
//'
//' @return frequency of the given value slot
// [[Rcpp::export]]
Rcpp::NumericVector
value_freq(SEXP dbpath, SEXP slot)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    int aSlot = Rcpp::as<int>(slot);
    int freq = db.get_value_freq(aSlot);
    Rcpp::NumericVector value_freq;
    value_freq.push_back(freq);
    return value_freq;
}

//' Database value_lower_bound wrapper
//'
//' @param dbpath path to a Xapian Database
//' @param slot   value slot to examine
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' slot<-c(1)
//' value_lower_bound(dbpath,slot)
//' }
//'
//' @return  a lower bound on the values stored in the given value slot
// [[Rcpp::export]]
Rcpp::StringVector
value_lower_bound(SEXP dbpath, SEXP slot)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    int aSlot = Rcpp::as<int>(slot);
    std::string val = db.get_value_lower_bound(aSlot);
    return val;
}

//' Database value_upper_bound wrapper
//'
//' @param dbpath path to a Xapian Database
//' @param slot   value slot to examine
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' slot<-c(1)
//' value_upper_bound(dbpath,slot)
//' }
//'
//' @return an upper bound on the values stored in the given value slot
// [[Rcpp::export]]
Rcpp::StringVector
value_upper_bound(SEXP dbpath, SEXP slot)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    int aSlot = Rcpp::as<int>(slot);
    std::string val = db.get_value_upper_bound(aSlot);
    return val;
}

//' Database uuid wrapper
//'
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' uuid(dbpath)
//' }
//'
//' @return a UUID for the database
// [[Rcpp::export]]
Rcpp::StringVector
uuid(SEXP dbpath)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    return db.get_uuid();
}

//' Database doccount wrapper
//'
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' doccount(dbpath)
//' }
//'
//' @return number of documents in the database
// [[Rcpp::export]]
Rcpp::NumericVector
doccount(SEXP dbpath)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    Rcpp::NumericVector doccount;
    doccount.push_back(db.get_doccount());
    return doccount;
}

//' Database avlength wrapper
//'
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' avlength(dbpath)
//' }
//'
//' @return average length of the documents in the database
// [[Rcpp::export]]
Rcpp::NumericVector
avlength(SEXP dbpath)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    Rcpp::NumericVector avlength;
    avlength.push_back(db.get_avlength());
    return avlength;
}

//' Database doclength_lower_bound wrapper
//'
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' doclength_lower_bound(dbpath)
//' }
//'
//' @return a lower bound on the length of a document in the database
// [[Rcpp::export]]
Rcpp::NumericVector
doclength_lower_bound(SEXP dbpath)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    Rcpp::NumericVector doclength_lower_bound;
    doclength_lower_bound.push_back(db.get_doclength_lower_bound());
    return doclength_lower_bound;
}

//' Database doclength_upper_bound wrapper
//'
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' doclength_upper_bound(dbpath)
//' }
//'
//' @return an upper bound on the length of a document in the database
// [[Rcpp::export]]
Rcpp::NumericVector
doclength_upper_bound(SEXP dbpath)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    Rcpp::NumericVector doclength_upper_bound;
    doclength_upper_bound.push_back(db.get_doclength_upper_bound());
    return doclength_upper_bound;
}

//' Database lastdocid wrapper
//'
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' lastdocid(dbpath)
//' }
//'
//' @return highest document id which has been used in the database
// [[Rcpp::export]]
Rcpp::NumericVector
lastdocid(SEXP dbpath)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    Rcpp::NumericVector lastdocid;
    lastdocid.push_back(db.get_lastdocid());
    return lastdocid;
}

//' Database has_positions wrapper
//'
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' has_positions(dbpath)
//' }
//'
//' @return whether this database has any positional information
// [[Rcpp::export]]
Rcpp::LogicalVector
has_positions(SEXP dbpath)
{
    Xapian::Database db = Rcpp::as<Xapian::Database>(dbpath);
    return db.has_positions();
}

//' xapian_delete wrapper
//' 
//' @param dbpath path to a Xapian Database
//' @param docid the document ID of the document to be removed
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' docid <- 20
//' deleteWrapper(dbpath,docid)
//' }
//' 
//' @return none
// [[Rcpp::export]]
void deleteWrapper(SEXP dbpath, SEXP docid){
  std::string path=Rcpp::as<std::string>(dbpath);
  Xapian::WritableDatabase db(path, Xapian::DB_OPEN);
  Xapian::docid id=Rcpp::as<int>(docid);
  db.delete_document(id);
}