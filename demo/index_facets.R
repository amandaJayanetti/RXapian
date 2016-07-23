# Xapian provides functionality which fascilitates dynamic generation of complete lists of values which feature in matching documents.
# This example shows how to ensure that the values you wish to use in facets are stored as document values.

library(RXapian)

# preparing arguments for xapian_index()
dataFile <- system.file("sampleData/100-objects-v1.csv", package="RXapian")

data <- read.csv(dataFile)

db<- tempfile(pattern="RXapianEx-")

# row value of this column will be used as a unique identifier to each document(row)
id<-c(0)

indexFields<-list(list(name="TITLE",prefix="S"),
                  list(name="DESCRIPTION",prefix="XD"))

# Add COLLECTION column of the data frame as a value in slot 0 and add MAKER column of the data frame as a value in slot 1
valueS<-list(list(name="COLLECTION",slot=c(0)),list(name="MAKER",slot=c(1)))

xapian_index(dbpath = db,
             dataFrame = data,
             idColumn = id,
             indexFields = indexFields,
             valueSlots = valueS,
             stemmer = "en")
