#!/usr/bin/env python3

import argparse
import os


class Stats:
    nodes = 0
    parents = 0
    leaves = 0
    depth = 0

    def __str__(self):
        return ",".join(str(x) for x in (self.nodes, self.parents, self.leaves, self.depth))


MAPPING = {
    "Number of nodes: ": "nodes",
    "Number of parents: ": "parents",
    "Number of leaves: ": "leaves",
    "Maximum depth: ": "depth",
}
KEYS = MAPPING.keys()


parser = argparse.ArgumentParser("Prints a CSV summary for multiple statistic files")
parser.add_argument("files", nargs="+", help="files to process")

args = parser.parse_args()

print("Dataset,Nodes,Parents,Leaves,Depth")
for filepath in args.files:
    dataset = os.path.basename(filepath).replace("-statistics.csv", "")
    with open(filepath, "r") as file_handle:
        stats = Stats()
        current_stats = Stats()
        for line in file_handle:
            for k in KEYS:
                if k in line:
                    setattr(current_stats, MAPPING[k], int(line.replace(k, "")))
            if "depth" in line and current_stats.leaves > stats.leaves:
                stats = current_stats
                current_stats = Stats()
        if current_stats.leaves > stats.leaves:
            stats = current_stats
        print(dataset, stats, sep=",")
