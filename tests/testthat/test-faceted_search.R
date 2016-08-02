context("Faceted Search")

test_that("facets returned are as expected", {

# preparing arguments for xapian_index()

ID_NUMBER <-c("1974-100",
              "1938-371",
              "1985-438")

TITLE<-c("Ansonia Sunwatch",
         "Ivory diptych dial clock",
         "Copy  of a Dwerrihouse skeleton clock ")

MAKER<-c("Ansonia clock Co.",
         "Self Winding Clock Company",
         "Peyer Favarger, Hipp dial movement")

COLLECTION<-c("SCM - Time Measurement",
              "ringdial diameter: 216 mm",
              " brass (alloy)")

df<-data.frame(ID_NUMBER,TITLE, MAKER, COLLECTION)

db<- tempfile(pattern="RXapian-")

id<-c(0)

index<-list(list(index=2, prefix ="S"))

valueS<-list(list(name="COLLECTION",slot=c(0)),list(name="MAKER",slot=c(1)))

# indexing the data frame with xapian_index()

xapian_index(dbpath=db,
             dataFrame=df,
             idColumn=id,
             valueSlots = valueS,
             indexFields=index)

# preparing arguments for xapian_search()

enq<-list(return.spy=c(0,1),checkatleast=100)

preF<-list(list(name="title", prefix="S"))

query<-list(queryString="clock", stemmer="en", prefix.fields=preF)

out<-xapian_search(db, enq, query)

# comparing results 

expect_equal("SCM - Time Measurement; count: 1",as.character(out$spies$'0'[1]))
expect_equal("ringdial diameter: 216 mm; count: 1",as.character(out$spies$'0'[2]))

expect_equal("Ansonia clock Co.; count: 1",as.character(out$spies$'1'[1]))
expect_equal("Self Winding Clock Company; count: 1",as.character(out$spies$'1'[2]))

})