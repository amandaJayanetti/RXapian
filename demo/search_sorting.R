# By default, Xapian orders search results by decreasing relevance score. 
# However, it also allows results to be ordered by other criteria, or a combination of other criteria and relevance score.
# In this example, relevance is used for ordering any groups of documents for which the value is the same.

library(RXapian)

db<-c("path/to/db")

enq<-list(sortby="value_then_relevance",valueNo=1,reverse_sort_order=FALSE)

preF<-list(list(name="name", prefix="S"),
           list(name="description", prefix="XD"))

query<-list(queryString="spanish", stemmer="en", prefix.fields=preF)

result<-xapian_search(db, enq, query)

# result is a dataframe with results for the searched query ordered such that
# relevance is used for ordering any groups of documents for which the value is the same