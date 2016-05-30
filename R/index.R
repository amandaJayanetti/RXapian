xapian_store <- function()
{
  # preparing arguments for xapian_index()
  ID_NUMBER <-c("1974-100",
                "1938-371",
                "1985-438")
  
  ITEM_NAME <-c("Pocket horizontal sundial",
                "diptych sundial",
                "Spring driven pendulum clock")
  
  TITLE<-c("Ansonia Sunwatch",
           "Ivory diptych dial",
           "Copy  of a Dwerrihouse skeleton clock ")
  
  data<-data.frame(ID_NUMBER,ITEM_NAME,TITLE)
  db<- tempfile(pattern="RXapianEx-")
  id<-c(0)
  indexFields<-list(list(index=2,prefix="S"),list(index=8,prefix="XD"))
  xapian_index(dbpath = db,dataFrame = data,idColumn = id,indexFields = indexFields,stemmer = "en")

}
