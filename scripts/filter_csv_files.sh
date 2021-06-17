#!/bin/bash

for filepath in $(find * -iname "*.csv" | grep -iv "filtered" | grep -iv "github")
do
  filename=$(basename "$filepath" ".csv")
  directory=$(dirname "$filepath")
  
  csvtool -t ';' -u ';' namedcol filename,RLoc "$filepath" > "$directory/${filename}-filtered.csv"
done
