# Xapian uses the concept of spies to observe slots of matched documents during a search.
# This example shows how spies should be added for each slot you want the facets

library(RXapian)

db<-c("path/to/db")

enq<-list(return.spy=c(0,1),checkatleast=100)

preF<-list(list(name="TITLE", prefix="S"),
           list(name="DESCRIPTION", prefix="XD"))

query<-list(queryString="clock", stemmer="en", prefix.fields=preF)

result<-xapian_search(db, enq, query)

# result is a list of lists where result$output is data.frame with results for the searched query, 
# and result$spies is a list of spies for the required slot