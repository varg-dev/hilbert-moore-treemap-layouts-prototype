#!/usr/bin/env python3

import argparse
import os


INPUTS = ["Algorithm+Partitioning", "Dataset"]
METRICS = ["AAR", "ADC", "RDC", "LD", "RPC", "AAD"]
ALGS = ["hilbert+greedy", "hilbert+minmax", "hilbert+variance", "moore+greedy", "moore+minmax", "moore+variance"]
ONLY_AAR = ["Kubernetes", "Firefox", "Github"]


parser = argparse.ArgumentParser("Prints a CSV summary for multiple layout metric files")
parser.add_argument("files", nargs="+", help="files to process")
parser.add_argument("-o", "--output", help="output folder for metric-wise files")

args = parser.parse_args()


all_metric_data = {metric: {} for metric in METRICS}

print(",".join(INPUTS + METRICS))
for filepath in args.files:
    split_base_name = os.path.basename(filepath).split("-")
    partition = split_base_name[-3]
    algorithm = split_base_name[-4]
    dataset = "-".join(split_base_name[:-4])
    pseudo_alg = "{}+{}".format(algorithm, partition)

    with open(filepath, "r") as file_handle:
        for line in file_handle:
            if line.startswith("Sum,"):
                metric_values = ["{:.6f}".format(abs(float(x))) for x in line.strip().split(",")[1:]]
                print(*([pseudo_alg, dataset] + metric_values), sep=",")

                for i, metric in enumerate(METRICS):
                    if metric != "AAR" and dataset in ONLY_AAR:
                        continue
                    metric_data = all_metric_data[metric]
                    if pseudo_alg not in metric_data:
                        metric_data[pseudo_alg] = []
                    metric_data[pseudo_alg].append(metric_values[i])

if args.output:
    if not os.path.isdir(args.output):
        os.makedirs(args.output)
    for metric in METRICS:
        with open(os.path.join(args.output, "data-{}.csv".format(metric)), "w") as file_handle:
            file_handle.write(",".join(["Algorithm"] + ALGS) + "\n")
            data = [all_metric_data[metric][alg] for alg in ALGS]
            file_handle.write(",".join([metric] + [str(sum(map(float, x)) / len(x)) for x in data]) + "\n")
