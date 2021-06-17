
#pragma once

#include <vector>
#include <utility>

#include "TreeNode.h"
#include "Buffer.h"

enum class PartitionAlg {
    Greedy,
    MinMax,
    Variance
};

std::vector<TreeNode::NodeRange> partitionMinMax(const TreeNode::NodeRange & range, const Buffer<float> &weights);
std::vector<TreeNode::NodeRange> partitionVariance(const TreeNode::NodeRange & range, const Buffer<float> &weights);
std::vector<TreeNode::NodeRange> partitionGreedy(const TreeNode::NodeRange & range, const Buffer<float> &weights);
