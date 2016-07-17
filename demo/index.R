# This example shows how a data frame can be indexed with xapian_index function.

library(RXapian)

# preparing arguments for xapian_index()
dataFile <- system.file("sampleData/100-objects-v1.csv", package="RXapian")
data <- read.csv(dataFile)
db<- tempfile(pattern="RXapianEx-")
id<-c(0)
indexFields<-list(list(index=2,prefix="S"),list(index=8,prefix="XD"))

xapian_index(dbpath = db,dataFrame = data,idColumn = id,indexFields = indexFields,stemmer = "en")
