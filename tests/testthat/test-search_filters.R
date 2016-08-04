context("Filtering Search Results")

test_that("Search results are filtered as expected", {
  
  # preparing arguments for xapian_index()
  
  ID_NUMBER <-c("1974-100",
                "1938-371",
                "1985-438")
  
  TITLE<-c("Ansonia Sunwatch",
           "Ivory diptych dial clock",
           "Copy  of a Dwerrihouse skeleton clock ")
  
  MATERIALS<-c("steel (metal)",
                "brass (copper, zinc alloy); silvered; glass",
                "brass (copper, zinc alloy); copper (alloy); steel (metal); plastic (unidentified); cotton (fibre)")
  
  df<-data.frame(ID_NUMBER,TITLE,MATERIALS)
  
  db<- tempfile(pattern="RXapian-")
  
  id<-c(0)
  
  index<-list(list(index=1, prefix ="S"))
  
  filterFields<-list(list(index=2,prefix="XM",separator=";"))
  
  
  # indexing the data frame with xapian_index()
  
  xapian_index(dbpath=db,
               dataFrame=df,
               idColumn=id,
               filterFields = filterFields,
               indexFields=index)
  
  # preparing arguments for xapian_search()
  
  preF<-list(list(name="TITLE", prefix="S"))
  
  query<-list(OP="OP_FILTER",
              query.left=list(queryString="clock",stemmer="en",prefix.fields=preF),
              query.right=list(OP="OP_OR",queries=list(list(tname="XMsteel (metal)"))))
  
  result<-xapian_search(dbpath=db, queryList = query)
  
  # comparing results 
  expect_equal(ID_NUMBER[3],as.character(result$a))
  expect_equal(TITLE[3],as.character(result$b))

})