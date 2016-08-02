# Some columns in a data frame may not contain unconstrained text; 
# for example, rows in such columns may contain only a few specific values or identifiers. 
# This example shows how the content from such columns could be indexed using filterFields parameter, 
# such that at search time results can be restricted to only those matching a particular value/identifier.

library(RXapian)

# preparing arguments for xapian_index()
dataFile <- system.file("sampleData/100-objects-v1.csv", package="RXapian")

data <- read.csv(dataFile)

db<- tempfile(pattern="RXapianEx-")

# row value of this column will be used as a unique identifier to each document(row)
id<-c(0)

indexFields<-list(list(name="TITLE",prefix="S"),
                  list(name="DESCRIPTION",PREFIX="XD"))

# this argument is required to facilitate 'filtering documents' at search time
filterFields<-list(list(name="MATERIALS",prefix="XM",separator=";"))

xapian_index(dbpath = db,
             dataFrame = data,
             idColumn = id,
             indexFields = indexFields,
             filterFields = filterFields,
             stemmer = "en")



