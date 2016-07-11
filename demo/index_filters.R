# Some columns in a data frame may not contain unconstrained text; 
# for example, rows in such columns may contain only a few specific values or identifiers. 
# This example shows how the content from such columns could be indexed using filterFields parameter, 
# such that at search time results can be restricted to only those matching a particular value/identifier.

library(RXapian)

# preparing arguments for xapian_index()
dataFile <- system.file("sampleData/100-objects-v1.csv", package="RXapian")
data <- read.csv(dataFile)
db<- tempfile(pattern="RXapianEx-")
id<-c(0)
indexFields<-list(list(index=2,prefix="S"))
filterFields<-list(list(index=3,prefix="XM",separator=";"))

xapian_index(db,data,id,indexFields,filterFields,"en")
