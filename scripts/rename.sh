#!/bin/bash

for filepath in $(find * -iname "*-filtered.csv")
do
  filename=$(basename "$filepath" "-filtered.csv")
  directory=$(dirname "$filepath")
  
  mv "$filepath" "$directory/${filename}.csv"
done
