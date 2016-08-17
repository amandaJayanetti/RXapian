context("Delete document by ID")

test_that("documents are deleted as expected", {
  
  # preparing arguments for xapian_index()
  
  ID_NUMBER <-c("1974-100",
                "1938-371",
                "1985-438")
  
  TITLE <-c("Ansonia Sunwatch",
           "Ivory diptych dial clock",
           "Copy  of a Dwerrihouse skeleton clock ")
  
  MAKER <-c("Ansonia clock Co.",
           "Self Winding Clock Company",
           "Peyer Favarger, Hipp dial movement")
  
  df <-data.frame(ID_NUMBER,TITLE,MAKER)
  
  db <-tempfile(pattern="RXapian-")
  
  id <-c(0)
  
  index <-list(list(index=2, prefix ="S"))
  
  # indexing the data frame with xapian_index()
  
  xapian_index(dbpath=db,
               dataFrame=df,
               idColumn=id,
               indexFields=index)
  
  # preparing arguments for xapian_search()
  
  preF <-list(list(name="title", prefix="S"))
  
  query <-list(queryString="clock", stemmer="en", prefix.fields=preF)
  
  result <-xapian_search(dbpath=db, queryList= query)
  
  expect_equal(ID_NUMBER[1],as.character(result$a[1]))
  expect_equal(ID_NUMBER[2],as.character(result$a[2]))
  
  # deleting document with docid=1
  xapian_delete(db,docid = 1)
  
  # querying the database again
  result <-xapian_search(dbpath=db, queryList= query)
  
  # document corresponding to ID_NUMBER[1] no longer exists in the database.
  expect_equal(ID_NUMBER[2],as.character(result$a[1]))
  
})