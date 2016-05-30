xapian_index <- function(dbpath, 
                         dataFrame, 
                         idColumn=NULL,
                         indexFields=NULL, 
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
  stemmer <- match.arg(stemmer)
  
  index(dbpath,dataFrame,idColumn,indexFields,stemmer);
}
