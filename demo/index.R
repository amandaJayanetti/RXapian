# This example shows how a data frame can be indexed with xapian_index function.

library(RXapian)

# preparing arguments for xapian_index()
dataFile <- system.file("sampleData/100-objects-v1.csv", package="RXapian")

data <- read.csv(dataFile)

db<- tempfile(pattern="RXapianEx-")

# row value of this column will be used as a unique identifier to each document(row)
id<-c(0)

indexFields<-list(list(name="TITLE",prefix="S"),list(name="DESCRIPTION",prefix="XD"))

xapian_index(dbpath = db,dataFrame = data,idColumn = id,indexFields = indexFields,stemmer = "en")
