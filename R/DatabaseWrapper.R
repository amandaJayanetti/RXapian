#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export 
xapian_dbInfo <- function(dbpath){
  checkmate::assertCharacter(dbpath)
  databaseInfo(dbpath)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export
xapian_dbDoccount <-function(dbpath){
  checkmate::assertCharacter(dbpath)
  doccount(dbpath)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export
xapian_dbUuid<-function(dbpath){
  checkmate::assertCharacter(dbpath)
  uuid(dbpath)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export
xapian_dbAvlength<-function(dbpath){
  checkmate::assertCharacter(dbpath)
  avlength(dbpath)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export
xapian_dbDoclength_lower_bound<-function(dbpath){
  checkmate::assertCharacter(dbpath)
  uuid(dbpath)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export
xapian_dbDoclength_upper_bound<-function(dbpath){
  checkmate::assertCharacter(dbpath)
  doclength_upper_bound<(dbpath)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export
xapian_dbLastdocid<-function(dbpath){
  checkmate::assertCharacter(dbpath)
  lastdocid(dbpath)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export
xapian_dbHas_positions<-function(dbpath){
  checkmate::assertCharacter(dbpath)
  has_positions(dbpath)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export
xapian_dbsetMetadata <-function(dbpath,
                             key,
                             value){
  checkmate::assertCharacter(dbpath)
  checkmate::assertCharacter(key)
  checkmate::assertCharacter(value)
  setmetadata(dbpath,key,value)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export
xapian_dbMetadata <-function(dbpath,
                             key){
  checkmate::assertCharacter(dbpath)
  checkmate::assertCharacter(key)
  metadata(dbpath,key)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export
xapian_dbSpelling_suggestion<-function(dbpath,
                                       arg){
  checkmate::assertCharacter(dbpath)
  checkmate::assertCharacter(arg)
  spelling_suggestion(dbpath,arg)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export 
xapian_dbCollection_freq<-function(dbpath,
                                   tname){
  checkmate::assertCharacter(dbpath)
  checkmate::assertCharacter(tname)
  collection_freq(dbpath,tname)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export
xapian_dbValue_freq<-function(dbpath,
                              slot){
  checkmate::assertCharacter(dbpath)
  checkmate::assertCharacter(slot)
  value_freq(dbpath,slot)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export
xapian_dbValue_lower_bound<-function(dbpath,
                                     slot){
  checkmate::assertCharacter(dbpath)
  checkmate::assertCharacter(slot)
  value_lower_bound(dbpath,slot)
}

#' 
#'
#' @param dbpath	path to a Xapian database
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' 
#' }
#' @return 
#' @export
xapian_dbValue_upper_bound<-function(dbpath,
                                     slot){
  checkmate::assertCharacter(dbpath)
  checkmate::assertCharacter(slot)
  value_upper_bound(dbpath,slot)
}
