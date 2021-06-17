#!/bin/bash

binary="./build/hilbert-moore-treemap-layouts"

mkdir -p resources

for algorithm in hilbert moore
do
  echo "Measuring algorithm $algorithm"
  for partition in greedy minmax minvariance
  do
    echo "Measuring partition $partition"

    echo "Calculator"
    $binary -lm --datasets datasets/calculator-master-metrics-files.txt        --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/calculator-$algorithm-$partition-layout-metrics.csv
    echo "CPP Rest SDK"
    $binary -lm --datasets datasets/cpprestsdk-master-metrics-files.txt        --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/cpprestsdk-$algorithm-$partition-layout-metrics.csv
    echo "glbinding"
    $binary -lm --datasets datasets/glbinding-master-metrics-files.txt         --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/glbinding-$algorithm-$partition-layout-metrics.csv
    echo "glfw"
    $binary -lm --datasets datasets/glfw-master-metrics-files.txt              --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/glfw-$algorithm-$partition-layout-metrics.csv
    echo "globjects"
    $binary -lm --datasets datasets/globjects-master-metrics-files.txt         --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/globjects-$algorithm-$partition-layout-metrics.csv
    echo "GSL"
    $binary -lm --datasets datasets/GSL-master-metrics-files.txt               --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/GSL-$algorithm-$partition-layout-metrics.csv
    echo "Opensource Portal"
    $binary -lm --datasets datasets/opensource-portal-master-metrics-files.txt --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/opensource-portal-$algorithm-$partition-layout-metrics.csv
    echo "STL"
    $binary -lm --datasets datasets/STL-master-metrics-files.txt               --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/STL-$algorithm-$partition-layout-metrics.csv
    echo "Tensorflow.js"
    $binary -lm --datasets datasets/tensorflow-tfjs-master-metrics-files.txt   --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/tensorflow-tfjs-$algorithm-$partition-layout-metrics.csv
    echo "thealgorithms-cpp"
    $binary -lm --datasets datasets/thealgorithms-cpp-master-metrics-files.txt --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/thealgorithms-cpp-$algorithm-$partition-layout-metrics.csv
    echo "VFS For Git"
    $binary -lm --datasets datasets/VFSForGit-master-metrics-files.txt         --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/VFSForGit-$algorithm-$partition-layout-metrics.csv
    echo "webgl-operate"
    $binary -lm --datasets datasets/webgl-operate-master-metrics-files.txt     --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/webgl-operate-$algorithm-$partition-layout-metrics.csv
    echo "Firefox multiple revisions"
    $binary -lm --datasets datasets/mozilla-gecko-dev-metrics-files.txt     --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/mozilla-gecko-dev-$algorithm-$partition-layout-metrics.csv
    
    echo "Kubernetes"
    $binary -lm --dataset datasets/kubernetes-metrics.csv --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/kubernetes-$algorithm-$partition-layout-metrics.csv
    echo "Firefox"
    $binary -lm --dataset datasets/firefox-metrics.csv    --datasets-prefix datasets/ --weights RLoc --algorithm $algorithm --partition $partition > ./resources/firefox-$algorithm-$partition-layout-metrics.csv
    echo "Github"
    $binary -lm --dataset datasets/github-5-mio-selected-metrics.csv    --datasets-prefix datasets/ --weights stargazers_count --algorithm $algorithm --partition $partition > ./resources/github-$algorithm-$partition-layout-metrics.csv
  done
done

./scripts/collect_layout_metrics.py resources/*-layout-metrics.csv -o resources/layout_metrics > resources/all_layout_metric_data.csv
