#' search wrapper
#'
#' @param dbpath	path to a Xapian database
#' @param queryList a list 
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#'
#' query<-list( queryString="watch", 
#'              prefix.fields=list( list(prefix="S",name="title"), list(prefix="XD",name="description")), 
#'              stemmer="en")
#'
#' enq<-list(matchspy=c(0,1))
#'
#' res<-xapian_search(db,enq,query)
#' 
#' }
#' @return data frame with results for the query string
#' @export
xapian_search <- function(dbpath=NULL,
                          enquireList=NULL,
                          queryList=NULL)
{
  checkmate::assertCharacter(dbpath)
  checkmate::assertList(queryList)
  if(!is.null(enquireList))
    checkmate::assertList(enquireList)
  
  result<-searchWrapper(dbpath,enquireList,queryList)
  
}