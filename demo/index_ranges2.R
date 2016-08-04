# This example show how to index a Xapian::Database to support range queries

library(RXapian)

db<- tempfile(pattern="RXapianEx-")

# preparing arguments for xapian_index()
dataFile <- system.file("sampleData/states.csv", package="RXapian")

data <- read.csv(dataFile)

# add a new column with a unique value for each row of the data frame (this column will be used as the idColumn)
data$id<-c(1:51)

# row value of this column will be used as a unique identifier to each document(row)
id<-c(8)

# preparing a vector to override frame values for admitted
adm<-data$admitted
ch<-as.character(adm)
vec<-c()
for(element in ch){
  w<-as.list(strsplit(element, ",")[[1]])
  f<-as.character(w[2])
  l<-as.list(strsplit(f, " ")[[1]])
  qq<-as.numeric(l[2])
  vec<-append(vec,qq)
}

# preparing a vector to override frame values for population
pop<-data$population
pVec<-c()
for(element in pop){
  ele<-as.list(strsplit(element, " ")[[1]])
  first<-as.character(ele[1])
  pVec<-append(pVec,first)
}
popVec<-as.numeric(gsub(",","",pVec))
popVec[33]<-c(6346105) # above modifications cause popVec[33] to be NA
popVec[41]<-c(12702379) # above modifications cause popVec[41] to be NA


vs1<-list(slot=c(1),serialise=TRUE,values=vec,type=c("double"))
vs2<-list(slot=c(3),serialise=TRUE,values=popVec,type=c("double"))

valueS<-list(vs1,vs2)

indexFields<-list(list(name="name",prefix="S"),
                  list(name="description",prefix="XD"),
                  list(name="motto",prefix="XM"))

xapian_index(dbpath = db,
             dataFrame = data,
             idColumn = id,
             indexFields = indexFields,
             valueSlots = valueS,
             stemmer = "en")