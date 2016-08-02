# This example show how to index a Xapian::Database to support range queries

library(RXapian)

# preparing arguments for xapian_index()
dataFile <- system.file("sampleData/100-objects-v1.csv", package="RXapian")

data <- read.csv(dataFile)

# extract and modify DATE_MADE column of data frame
dateCol<-data$DATE_MADE
dateVec<-as.double(gsub("([0-9]+).*$", "\\1",dateCol))

# extract and modify Measurements column of data frame
library(stringr)
m <- data$MEASUREMENTS
y <- str_extract_all(m ,"\\(?[0-9.]+\\)?")
mVec<-c()
for(element in y){
  if(length(element)==0)
    mVec<-append(mVec,NA)
  else{
    ele<-as.double(element)
    mVec<-append(mVec,round(max(ele)))
  }
}

db<- tempfile(pattern="RXapianEx-")

# row value of this column will be used as a unique identifier to each document(row)
id<-c(0)

indexFields<-list(list(name="TITLE",prefix="S"),
                  list(name="DESCRIPTION",prefix="XD"))

vs0<-list(slot=c(0),serialise=TRUE,values=mVec,type=c("double"))

vs1<-list(slot=c(1),serialise=TRUE,values=dateVec,type=c("double"))

# this argument is required to facilitate 'range queries' at search time
valueS<-list(vs0,vs1)

xapian_index(dbpath = db,
             dataFrame = data,
             idColumn = id,
             indexFields = indexFields,
             valueSlots = valueS,
             stemmer = "en")
