#!/bin/bash

binary="./build/hilbert-moore-treemap-layouts"

mkdir -p resources

echo "Calculator"
$binary -p --datasets datasets/calculator-master-metrics-files.txt        --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/calculator-statistics.csv
echo "CPP Rest SDK"
$binary -p --datasets datasets/cpprestsdk-master-metrics-files.txt        --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/cpprestsdk-statistics.csv
echo "glbinding"
$binary -p --datasets datasets/glbinding-master-metrics-files.txt         --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/glbinding-statistics.csv
echo "glfw"
$binary -p --datasets datasets/glfw-master-metrics-files.txt              --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/glfw-statistics.csv
echo "globjects"
$binary -p --datasets datasets/globjects-master-metrics-files.txt         --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/globjects-statistics.csv
echo "GSL"
$binary -p --datasets datasets/GSL-master-metrics-files.txt               --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/GSL-statistics.csv
echo "Opensource Portal"
$binary -p --datasets datasets/opensource-portal-master-metrics-files.txt --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/opensource-portal-statistics.csv
echo "STL"
$binary -p --datasets datasets/STL-master-metrics-files.txt               --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/STL-statistics.csv
echo "Tensorflow.js"
$binary -p --datasets datasets/tensorflow-tfjs-master-metrics-files.txt   --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/tensorflow-tfjs-statistics.csv
echo "thealgorithms-cpp"
$binary -p --datasets datasets/thealgorithms-cpp-master-metrics-files.txt --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/thealgorithms-cpp-statistics.csv
echo "VFS For Git"
$binary -p --datasets datasets/VFSForGit-master-metrics-files.txt         --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/VFSForGit-statistics.csv
echo "webgl-operate"
$binary -p --datasets datasets/webgl-operate-master-metrics-files.txt     --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/webgl-operate-statistics.csv

echo "Kubernetes"
$binary -p --dataset datasets/kubernetes-metrics.csv --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/kubernetes-statistics.csv
echo "Firefox"
$binary -p --dataset datasets/firefox-metrics.csv    --datasets-prefix datasets/ --weights RLoc --partition greedy > ./resources/firefox-statistics.csv
echo "Github"
$binary -p --dataset datasets/github-5-mio-selected-metrics.csv    --datasets-prefix datasets/ --weights stargazers_count --partition greedy > ./resources/github-statistics.csv

# Collect
./scripts/collect_statistics.py ./resources/calculator-statistics.csv ./resources/cpprestsdk-statistics.csv ./resources/glbinding-statistics.csv ./resources/glfw-statistics.csv ./resources/globjects-statistics.csv ./resources/GSL-statistics.csv ./resources/opensource-portal-statistics.csv ./resources/STL-statistics.csv ./resources/tensorflow-tfjs-statistics.csv ./resources/thealgorithms-cpp-statistics.csv ./resources/VFSForGit-statistics.csv ./resources/webgl-operate-statistics.csv ./resources/kubernetes-statistics.csv ./resources/firefox-statistics.csv ./resources/github-statistics.csv > ./resources/_summary.csv
