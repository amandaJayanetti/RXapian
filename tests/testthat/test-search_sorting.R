context("Sorting Search Results")

test_that("Search results are sorted as expected", {
  
  # preparing arguments for xapian_index()
  
  id<-c(1,
        2,
        3)
  
  name<-c("State of Montana",
          "State of Iowa",
          "State of Texas")
  
  motto<-c("Oro y Plata (Spanish: Gold and Silver)",
           "Our liberties we prize and our rights we will maintain.",
           "Equality Before the Law")
  
  description<-c("This geographical fact is reflected in the state's name, derived from the Spanish word montaña (mountain).",
                 "It is located in the Midwestern United States and Great Lakes Region.",
                 "The name was applied by the Spanish to the Caddo themselves")
  admitted<-c(1889,
              1959,
              1845)
  
  data<-data.frame(id, name, motto, description, admitted)
  
  db<- tempfile(pattern="RXapian-")
  
  id<-c(0)
  
  vs1<-list(slot=c(1),serialise=TRUE,name="admitted")
  
  valueS<-list(vs1)
  
  indexFields<-list(list(name="name",prefix="S"),
                    list(name="description",prefix="XD"),
                    list(name="motto",prefix="XM"))
  
  xapian_index(dbpath = db,
               dataFrame = data,
               idColumn = id,
               indexFields = indexFields,
               valueSlots = valueS,
               stemmer = "en")
  
  # preparing arguments for xapian_search()
  
  enq<-list(sortby="value_then_relevance",valueNo=1,reverse_sort_order=FALSE)
  
  preF<-list(list(name="name", prefix="S"),
             list(name="description", prefix="XD"))
  
  query<-list(queryString="spanish", stemmer="en", prefix.fields=preF)
  
  result<-xapian_search(db, enq, query)
  
  # comparing results 
  
  expect_equal(name[3],as.character(result$b[1]))
  expect_equal(name[1],as.character(result$b[2]))
})