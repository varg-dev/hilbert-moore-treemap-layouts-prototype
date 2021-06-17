#!/bin/bash

mkdir -p resources

binary="./build/hilbert-moore-treemap-layouts"
algorithm="minvariance"

$binary -es --datasets datasets/calculator-master-metrics-files.txt        --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/calculator-layout.svg
$binary -es --datasets datasets/cpprestsdk-master-metrics-files.txt        --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/cpprestsdk-layout.svg
$binary -es --datasets datasets/glbinding-master-metrics-files.txt         --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/glbinding-layout.svg
$binary -es --datasets datasets/glfw-master-metrics-files.txt              --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/glfw-layout.svg
$binary -es --datasets datasets/globjects-master-metrics-files.txt         --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/globjects-layout.svg
$binary -es --datasets datasets/GSL-master-metrics-files.txt               --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/GSL-layout.svg
$binary -es --datasets datasets/opensource-portal-master-metrics-files.txt --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/opensource-portal-layout.svg
$binary -es --datasets datasets/STL-master-metrics-files.txt               --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/STL-layout.svg
$binary -es --datasets datasets/tensorflow-tfjs-master-metrics-files.txt   --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/tensorflow-tfjs-layout.svg
$binary -es --datasets datasets/thealgorithms-cpp-master-metrics-files.txt --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/thealgorithms-cpp-layout.svg
$binary -es --datasets datasets/VFSForGit-master-metrics-files.txt         --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/VFSForGit-layout.svg
$binary -es --datasets datasets/webgl-operate-master-metrics-files.txt     --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/webgl-operate-layout.svg

$binary -es --dataset datasets/kubernetes-metrics.csv --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/kubernetes-layout.svg
$binary -es --dataset datasets/firefox-metrics.csv    --datasets-prefix datasets/ --weights RLoc --partition $algorithm > ./resources/firefox-layout.svg
