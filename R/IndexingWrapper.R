#' indexing wrapper
#'
#' @param dbpath	    path to a Xapian database
#' @param dataFrame	  data frame to be indexed
#' @param idColumn	  index of a column in the data frame whose row value will be used as a unique identifier to each document(row)
#' @param indexFields	list of 'list of fields' that will be indexed using a Xapian::TermGenerator
#' @param filterFields	list with information on boolean terms to be added to Xapian::Document
#' @param stemmer 	the stemmer that should be applied to the Xapian::TermGenerator
#' @examples
#' \dontrun{
#' 
#' db<- c("path/to/database")
#' data<-read.csv("path/to/csv") or data<-data.frame(..)
#' id<-c(0)
#' indexFields<-list(list(index=2,prefix="S"), list(index=8,prefix="XD"))
#' filterFields<-list(list(index=6, prefix="XM", separator=";"))
#' xapian_index(db,data,id,indexFields,filterFields,"en")
#' 
#' }
#' @return none
#' @export
xapian_index <- function(dbpath, 
                         dataFrame, 
                         idColumn=NULL,
                         indexFields=NULL, 
                         filterFields=NULL, 
                         stemmer = c(
                           "english", "en",
                           "danish", "da",
                           "dutch", "nl",
                           "english_lovins", "lovins",
                           "english_porter", "porter",
                           "finnish", "fi",
                           "french", "fr",
                           "german", "de", "german2",
                           "hungarian", "hu",
                           "italian", "it",
                           "kraaij_pohlmann",
                           "norwegian", "nb", "nn", "no",
                           "portuguese", "pt",
                           "romanian", "ro",
                           "russian", "ru",
                           "spanish", "es",
                           "swedish", "sv",
                           "turkish", "tr"))
{
  
  checkmate::assertCharacter(dbpath)
  checkmate::assertDataFrame(dataFrame)
  checkmate::assertNumeric(idColumn)
  checkmate::assertList(indexFields)
  if(!is.null(filterFields))
    checkmate::assertList(filterFields)
  stemmer <- match.arg(stemmer)
  
  indexWrapper(dbpath,dataFrame,idColumn,indexFields,filterFields,stemmer);
}
