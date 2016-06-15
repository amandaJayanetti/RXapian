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
#' res<-xapian_search(db,query)
#' 
#' }
#' @return data frame with results for the query string
#' @export
xapian_search <- function(dbpath=NULL,
                          queryList=NULL)
{
  checkmate::assertCharacter(dbpath)
  checkmate::assertList(queryList)
  
  result<-searchWrapper(dbpath,queryList)
  
}