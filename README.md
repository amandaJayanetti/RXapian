## R bindings for Xapian search engine library.

Please make sure that you have Xapian installed. (Xapian can be installed in Ububtu with command: sudo apt-get install libxapian-dev)

###### Installing RXapian

```R

install.packages("devtools")
library(devtools)
install_github("amandaJayanetti/RXapian")

```
###### Indexing a data frame with RXapian
```R
# Example I
# preparing arguments for xapian_index
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
  
  library('RXapian')
  xapian_index(dbpath = db,dataFrame = data,idColumn = id,indexFields = indexFields, stemmer = "en")
  
  ```
  ```R
# Example II
  # preparing arguments for xapian_index
  ID_NUMBER <-c("1974-100",
                "1938-371",
                "1985-438")
  
  ITEM_NAME <-c("Pocket horizontal sundial",
                "diptych sundial",
                "Spring driven pendulum clock")
  
  TITLE<-c("Ansonia Sunwatch",
           "Ivory diptych dial",
           "Copy  of a Dwerrihouse skeleton clock ")
  
  MATERIALS <-c("brass (copper, zinc alloy); silvered; glass",
                "brass (copper, zinc alloy); steel (metal); cardboard; wood (unidentified)",
                "brass (copper, zinc alloy); steel (metal); wood (unidentified)")
  
  data<-data.frame(ID_NUMBER,ITEM_NAME,TITLE,MATERIALS)
  db<- tempfile(pattern="RXapianEx-")
  id<-c(0)
  indexFields<-list(list(index=2,prefix="S"))
  filterFields<-list(list(index=3,prefix="XM",separator=";"))
  
  xapian_index(db,data,id,indexFields,filterFields,"en")

  
  ```

