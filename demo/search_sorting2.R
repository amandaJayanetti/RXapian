# By default, Xapian orders search results by decreasing relevance score. 
# However, it also allows results to be ordered by other criteria, or a combination of other criteria and relevance score.
# In this example, results for the searched query is ordered by year of entry to the union and then by decreasing population.

library(RXapian)

db<-c("path/to/db")

keyMaker=list(list(slot=1,reverse=FALSE),list(slot=3,reverse=TRUE))

enq<-list(sortby="key_then_relevance",keyMaker=keyMaker,reverse_sort_order=FALSE)

preF<-list(list(name="name", prefix="S"),
           list(name="description", prefix="XD"))

query<-list(queryString="State", stemmer="en", prefix.fields=preF)

result<-xapian_search(db, enq, query)

# result is a dataframe with results for the searched query ordered by year of entry to the union 
# and then by decreasing population
