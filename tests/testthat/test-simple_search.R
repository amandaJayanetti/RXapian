context("Simple Search")

test_that("search results are as expected", {
  
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
  
  df<-data.frame(ID_NUMBER,ITEM_NAME,TITLE)
  db<- tempfile(pattern="RXapian-")
  id<-c(0)
  index<-list(list(name="ITEM_NAME"),list(index=2, prefix ="S"))
  
  # indexing the data frame with xapian_index()
  
  xapian_index(dbpath=db,
               dataFrame=df,
               idColumn=id,
               indexFields=index)
  
  # preparing arguments for xapian_search()
  
  field1 <- list(prefix= "S",name ="title")
  queryList <- list(queryString ="clock", 
                    prefix.fields=list(field1), 
                    strategy= "STEM_SOME", 
                    stemmer ="en")
  
  # searching a xapian database with xapian_search()
  
  result <-xapian_search(dbpath=db, queryList= queryList)
  
  # comparing results 
  
  expect_equal(ID_NUMBER[3],as.character(result$a))
  expect_equal(ITEM_NAME[3],as.character(result$b))
  expect_equal(TITLE[3],as.character(result$c))
  })