## R bindings for Xapian search engine library.

###### Installing RXapian

```R

install.packages("devtools")
library(devtools)
install_github("amandaJayanetti/xapianExamples")

```
###### Indexing a data frame with RXapian
```R
# preparing arguments for xapian_index()
  ID_NUMBER <-c("1974-100",
                "1938-371",
                "1985-438")
  
  ITEM_NAME <-c("Pocket horizontal sundial",
                "diptych sundial",
                "Spring driven pendulum clock")
  
  TITLE<-c("Ansonia Sunwatch",
           "Ivory diptych dial",
           "Copy  of a Dwerrihouse skeleton clock ")
  
  data<-data.frame(ID_NUMBER,ITEM_NAME,TITLE)
  db<- tempfile(pattern="RXapianEx-")
  id<-c(0)
  indexFields<-list(list(index=2,prefix="S"))
  
  xapian_index(dbpath = db,dataFrame = data,idColumn = id,indexFields = indexFields, stemmer = "en")
  
  ```

