# Some columns in a data frame may not contain unconstrained text;
# for example, rows in such columns may contain only a few specific values or identifiers. 
# This example shows how to use such fields to restrict the results to only those matching a particular value, 
# rather than treating them as unstructured "free text".

library(RXapian)

# As the first step, index the database as shown in index_ranges example

db<-c("path/to/db")

preF<-list(list(name="title", prefix="S"),
           list(name="description", prefix="XD"))

query<-list(OP="OP_FILTER",
            query.left=list(queryString="clock",stemmer="en",prefix.fields=preF),
            query.right=list(OP="OP_OR",queries=list(list(tname="XMsteel (metal)"))))

result<-xapian_search(dbpath=db, queryList = query)

# result is a dataframe with results for the searched query filtered to include
# only those containing 'steel (metal)' in the Materials column