# This example shows how to search for objects, instances etc. occuring within a particular 'range'

library(RXapian)

# As the first step, index the database as shown in index_ranges example

db<-c("path/to/db")

vrp1<-list(type="proc.num", value=c(0), check.str="mm", flags=c("RP_SUFFIX"))

vrp2<-list(type="proc.num", value=c(1), check.str="")

vrp<-list(vrp1,vrp2)

preF<-list(list(name="title", prefix="S"),
           list(name="description", prefix="XD"))

query<-list(queryString="..50mm", stemmer="en", prefix.fields=preF, VRP=vrp)

result<-xapian_search(dbpath=db, queryList = query)

# result is a dataframe with results for the searched query restricted to
# everything at most 50mm in its longest dimension