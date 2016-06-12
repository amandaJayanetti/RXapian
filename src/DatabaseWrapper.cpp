#include <RcppCommon.h>

#include <xapian.h>

namespace Rcpp {

template<> SEXP wrap(const Xapian::Database &db);
template<> Xapian::Database as(SEXP);

}

#include <Rcpp.h>

namespace Rcpp{

template<> Xapian::Database as(SEXP inp) { 
  Rcpp::CharacterVector dbpath(inp); 
  std::string pathToDb=as<std::string>(dbpath);
  Xapian::WritableDatabase db(pathToDb, Xapian::DB_CREATE_OR_OPEN);
  return db;
}

//change
template<> SEXP wrap(const Xapian::Database &db) {
  std::string uuid=db.get_uuid().c_str();
  int doccount=db.get_doccount();
  int avlength=db.get_avlength();
  int doc_low=db.get_doclength_lower_bound();
  int doc_up=db.get_doclength_upper_bound();
  int docid=db.get_lastdocid();
  bool hasPos=db.has_positions();
  
  Rcpp::List database= List::create(Named("uuid", uuid),
                                    Named("doccount", doccount),
                                    Named("avlength", avlength),
                                    Named("doc_low", doc_low),
                                    Named("doc_up", doc_up),
                                    Named("docid", docid),
                                    Named("hasPos", hasPos));
  
  return database;
}

}

//' DatabaseInfo Wrapper
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' dbpath<- c("path/to/database")
//' databaseInfo(dbpath)
//' 
//' }
//' 
//' @return list of information about the Xapian database at specified location
// [[Rcpp::export]]
Rcpp::List databaseInfo(SEXP dbpath){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  return Rcpp::wrap(db);
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' dbpath<- c("path/to/database")
//' }
//' 
//' @return 
// [[Rcpp::export]]
void setmetadata(SEXP dbpath,SEXP key,SEXP value){
  std::string path=Rcpp::as<std::string>(dbpath);
  Xapian::WritableDatabase db(path, Xapian::DB_CREATE_OR_OPEN);
  std::string akey=Rcpp::as<std::string>(key);
  std::string aVal=Rcpp::as<std::string>(value);
  db.set_metadata(akey,aVal);
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::StringVector metadata(SEXP dbpath,SEXP key){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  std::string akey=Rcpp::as<std::string>(key);
  return db.get_metadata(akey);
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::StringVector spelling_suggestion(SEXP dbpath,SEXP arg){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  Rcpp::List alist=arg;
  unsigned int dist=2;
  std::string word;
  try{
    word=Rcpp::as<std::string>(alist["word"]);
  }
  catch(...){
    Rcpp::stop("Invalid argument format");
  }
  if(alist.containsElementNamed("max_edit_distance")){
    dist=Rcpp::as<int>(alist["max_edit_distance"]);
  }
  std::string suggtn=db.get_spelling_suggestion(word,dist);
  return suggtn;
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::NumericVector collection_freq(SEXP dbpath,SEXP tname){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  std::string aTname=Rcpp::as<std::string>(tname);
  int freq=db.get_collection_freq(aTname);
  return freq;
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::NumericVector value_freq(SEXP dbpath,SEXP slot){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  int aSlot=Rcpp::as<int>(slot);
  int freq=db.get_value_freq(aSlot);
  return freq;
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::NumericVector value_lower_bound(SEXP dbpath,SEXP slot){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  int aSlot=Rcpp::as<int>(slot);
  std::string val=db.get_value_lower_bound(aSlot);
  return val;
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::NumericVector value_upper_bound(SEXP dbpath,SEXP slot){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  int aSlot=Rcpp::as<int>(slot);
  std::string val=db.get_value_upper_bound(aSlot);
  return val;
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::StringVector uuid(SEXP dbpath){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  return db.get_uuid();
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::NumericVector doccount(SEXP dbpath){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  return db.get_doccount();
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::NumericVector avlength(SEXP dbpath){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  return db.get_avlength();
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::NumericVector doclength_lower_bound(SEXP dbpath){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  return db.get_doclength_lower_bound();
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::NumericVector doclength_upper_bound(SEXP dbpath){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  return db.get_doclength_upper_bound();
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::NumericVector lastdocid(SEXP dbpath){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  return db.get_lastdocid();
}

//' 
//' 
//' @param dbpath path to a Xapian Database
//' @examples
//' \dontrun{
//' 
//' }
//' 
//' @return 
// [[Rcpp::export]]
Rcpp::LogicalVector has_positions(SEXP dbpath){
  Xapian::Database db=Rcpp::as<Xapian::Database>(dbpath);
  return db.has_positions();
}


