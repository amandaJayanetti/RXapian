#' DatabaseInfo
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' xapian_dbInfo(db)
#' }
#' @return list of information about the Xapian database at specified location
#' @export 
xapian_dbInfo <- function(dbpath){
  checkmate::assertCharacter(dbpath)
  databaseInfo(dbpath)
}

#' Database doccount
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' xapian_dbDoccount(db)
#' }
#' @return number of documents in the database
#' @export
xapian_dbDoccount <-function(dbpath){
  checkmate::assertCharacter(dbpath)
  doccount(dbpath)
}

#' Database uuid
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' xapian_dbUuid(db)
#' }
#' @return a UUID for the database
#' @export
xapian_dbUuid<-function(dbpath){
  checkmate::assertCharacter(dbpath)
  uuid(dbpath)
}

#' Database avlength
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' xapian_dbAvlength(db)
#' }
#' @return average length of the documents in the database
#' @export
xapian_dbAvlength<-function(dbpath){
  checkmate::assertCharacter(dbpath)
  avlength(dbpath)
}

#' Database doclength_lower_bound
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' xapian_dbDoclength_lower_bound(db)
#' }
#' @return a lower bound on the length of a document in the database
#' @export
xapian_dbDoclength_lower_bound<-function(dbpath){
  checkmate::assertCharacter(dbpath)
  doclength_lower_bound(dbpath)
}

#' Database doclength_upper_bound
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' xapian_dbDoclength_upper_bound(db)
#' }
#' @return an upper bound on the length of a document in the database
#' @export
xapian_dbDoclength_upper_bound<-function(dbpath){
  checkmate::assertCharacter(dbpath)
  doclength_upper_bound(dbpath)
}

#' Database lastdocid
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' xapian_dbLastdocid(db)
#' }
#' @return highest document id which has been used in the database
#' @export
xapian_dbLastdocid<-function(dbpath){
  checkmate::assertCharacter(dbpath)
  lastdocid(dbpath)
}

#' Database has_positions
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' xapian_dbHas_positions(db)
#' }
#' @return whether this database has any positional information
#' @export
xapian_dbHas_positions<-function(dbpath){
  checkmate::assertCharacter(dbpath)
  has_positions(dbpath)
}

#' Database setmetadata
#'
#' @param dbpath	path to a Xapian database
#' @param key    key of the metadata item to set
#' @param value  value of the metadata item to set
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' key<-c("k1")
#' value<-c("v1")
#' xapian_dbsetMetadata(db,key,value)
#' }
#' @return none
#' @export
xapian_dbsetMetadata <-function(dbpath,
                             key,
                             value){
  checkmate::assertCharacter(dbpath)
  checkmate::assertCharacter(key)
  checkmate::assertCharacter(value)
  setmetadata(dbpath,key,value)
}

#' Database Metadata 
#'
#' @param dbpath	path to a Xapian database
#' @param key    key of the metadata item to access
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' key<-c("k1")
#' xapian_dbMetadata(db,key)
#' }
#' @return user-specified metadata associated with the given key
#' @export
xapian_dbMetadata <-function(dbpath,
                             key){
  checkmate::assertCharacter(dbpath)
  checkmate::assertCharacter(key)
  getmetadata(dbpath,key)
}

#' Database spelling_suggestion
#'
#' @param dbpath	path to a Xapian database
#' @param arg    potentially misspelled word
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' arg<-list(word="helllo", max_edit_distance = 3)
#' xapian_dbSpelling_suggestion(db,arg)
#' }
#' @return a spelling correction to a potentially misspelled word
#' @export
xapian_dbSpelling_suggestion<-function(dbpath,
                                       arg){
  checkmate::assertCharacter(dbpath)
  checkmate::assertList(arg)
  spelling_suggestion(dbpath,arg)
}

#' Database collection_freq
#'
#' @param dbpath	path to a Xapian database
#' @param tname  term whose collection frequency is being requested
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' tname<-c("test")
#' xapian_dbCollection_freq(db,tname)
#' }
#' @return total number of occurrences of the given term
#' @export 
xapian_dbCollection_freq<-function(dbpath,
                                   tname){
  checkmate::assertCharacter(dbpath)
  checkmate::assertCharacter(tname)
  collection_freq(dbpath,tname)
}

#' Database value_freq
#'
#' @param dbpath	path to a Xapian database
#' @param slot   value slot to examine
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' slot<-c(1)
#' xapian_dbValue_freq(db,slot)
#' }
#' @return frequency of the given value slot
#' @export
xapian_dbValue_freq<-function(dbpath,
                              slot){
  checkmate::assertCharacter(dbpath)
  checkmate::assertNumber(slot)
  value_freq(dbpath,slot)
}

#' Database value_lower_bound
#'
#' @param dbpath	path to a Xapian database
#' @param slot   value slot to examine
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' slot<-c(1)
#' xapian_dbValue_lower_bound(db,slot)
#' }
#' @return a lower bound on the values stored in the given value slot
#' @export
xapian_dbValue_lower_bound<-function(dbpath,
                                     slot){
  checkmate::assertCharacter(dbpath)
  checkmate::assertNumber(slot)
  value_lower_bound(dbpath,slot)
}

#' Database value_upper_bound
#'
#' @param dbpath	path to a Xapian database
#' @param slot   value slot to examine
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' slot<-c(1)
#' xapian_dbValue_upper_bound(db,slot)
#' }
#' @return an upper bound on the values stored in the given value slot
#' @export
xapian_dbValue_upper_bound<-function(dbpath,
                                     slot){
  checkmate::assertCharacter(dbpath)
  checkmate::assertNumber(slot)
  value_upper_bound(dbpath,slot)
}

#' xapian_delete
#'
#' @param dbpath path to a Xapian database
#' @param docid the document ID of the document to be removed
#' @examples
#' \dontrun{
#' db<- c("path/to/database")
#' docid <- 20
#' xapian_delete(db,docid)
#' }
#' @return none
#' @export
xapian_delete<-function(dbpath,
                        docid){
  checkmate::assertCharacter(dbpath)
  checkmate::assertNumber(docid)
  deleteWrapper(dbpath,docid)
}
