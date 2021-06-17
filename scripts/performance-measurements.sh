#!/bin/bash

binary="./build/hilbert-moore-treemap-layouts"

mkdir -p resources

for algorithm in greedy minmax minvariance
do
  echo "Measuring algorithm $algorithm"

  echo "Calculator"
  $binary -pm --datasets datasets/calculator-master-metrics-files.txt        --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/calculator-$algorithm-timings.csv
  echo "CPP Rest SDK"
  $binary -pm --datasets datasets/cpprestsdk-master-metrics-files.txt        --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/cpprestsdk-$algorithm-timings.csv
  echo "glbinding"
  $binary -pm --datasets datasets/glbinding-master-metrics-files.txt         --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/glbinding-$algorithm-timings.csv
  echo "glfw"
  $binary -pm --datasets datasets/glfw-master-metrics-files.txt              --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/glfw-$algorithm-timings.csv
  echo "globjects"
  $binary -pm --datasets datasets/globjects-master-metrics-files.txt         --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/globjects-$algorithm-timings.csv
  echo "GSL"
  $binary -pm --datasets datasets/GSL-master-metrics-files.txt               --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/GSL-$algorithm-timings.csv
  echo "Opensource Portal"
  $binary -pm --datasets datasets/opensource-portal-master-metrics-files.txt --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/opensource-portal-$algorithm-timings.csv
  echo "STL"
  $binary -pm --datasets datasets/STL-master-metrics-files.txt               --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/STL-$algorithm-timings.csv
  echo "Tensorflow.js"
  $binary -pm --datasets datasets/tensorflow-tfjs-master-metrics-files.txt   --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/tensorflow-tfjs-$algorithm-timings.csv
  echo "thealgorithms-cpp"
  $binary -pm --datasets datasets/thealgorithms-cpp-master-metrics-files.txt --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/thealgorithms-cpp-$algorithm-timings.csv
  echo "VFS For Git"
  $binary -pm --datasets datasets/VFSForGit-master-metrics-files.txt         --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/VFSForGit-$algorithm-timings.csv
  echo "webgl-operate"
  $binary -pm --datasets datasets/webgl-operate-master-metrics-files.txt     --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/webgl-operate-$algorithm-timings.csv
  
  echo "Kubernetes"
  $binary -pm --dataset datasets/kubernetes-metrics.csv                      --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/kubernetes-$algorithm-timings.csv
  echo "Firefox single revision"
  $binary -pm --dataset datasets/firefox-metrics.csv                         --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/firefox-$algorithm-timings.csv
  echo "Github"
  $binary -pm --dataset datasets/github-5-mio-selected-metrics.csv           --datasets-prefix datasets/ --weights stargazers_count --partition $algorithm > ./resources/github-$algorithm-timings.csv
done
