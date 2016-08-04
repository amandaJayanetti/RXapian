context("Search ranges")

test_that("search results are as expected", {
  
  # preparing arguments for xapian_index()
  
  ID_NUMBER <-c("1974-100",
                "1938-371",
                "1985-438")
  
  TITLE<-c("Ansonia Sunwatch",
           "Ivory diptych dial",
           "Copy  of a Dwerrihouse skeleton clock ")
  
  DATE_MADE <-c("1922-1939",
                "1931-1939",
                "1865-1875")
  
  MEASUREMENTS <-c("overall: 140 mm x 124 mm x 152 mm",
                   "photograph (excluding mount): 285 x 230 mm",
                   "overall diameter: 50 mm")
  
  data<-data.frame(ID_NUMBER,TITLE,DATE_MADE,MEASUREMENTS)
  
  # extract and modify DATE_MADE column of data frame
  dateCol<-data$DATE_MADE
  dateVec<-as.double(gsub("([0-9]+).*$", "\\1",dateCol))
  
  # extract and modify Measurements column of data frame
  library(stringr)
  m<-data$MEASUREMENTS
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
  
  db<- tempfile(pattern="RXapian-")
  
  id<-c(0)
  
  indexFields<-list(list(index=1,prefix="S"))
  
  vs0<-list(slot=c(0),serialise=TRUE,values=mVec,type=c("double"))
  
  vs1<-list(slot=c(1),serialise=TRUE,values=dateVec,type=c("double"))
  
  valueS<-list(vs0,vs1)
  
  xapian_index(dbpath = db,
               dataFrame = data,
               idColumn = id,
               indexFields = indexFields,
               valueSlots = valueS,
               stemmer = "en")
  
  # preparing arguments for xapian_search()
  
  vrp1<-list(type="proc.num", value=c(0), check.str="mm", flags=c("RP_SUFFIX"))
  
  vrp2<-list(type="proc.num", value=c(1), check.str="")
  
  vrp<-list(vrp1,vrp2)
  
  preF<-list(list(name="title", prefix="S"))
  
  query<-list(queryString="..50mm", stemmer="en", prefix.fields=preF, VRP=vrp)
  
  # searching a xapian database with xapian_search()
  
  result<-xapian_search(dbpath=db, queryList = query)
  
  # comparing results 
  
  expect_equal(ID_NUMBER[3],as.character(result$a))
  expect_equal(TITLE[3],as.character(result$b))
  
})