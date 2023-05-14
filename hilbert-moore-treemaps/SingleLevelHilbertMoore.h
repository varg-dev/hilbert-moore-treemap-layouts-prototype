
#pragma once


#include <vector>
#include <array>

#include "Buffer.h"
#include "Rect.h"
#include "WeightPartitioning.h"


class Tree;


class SingleLevelHilbertMoore
{
public:
    static void layoutHilbert(const TreeNode *parent, const Rect & rect, const Tree * tree, const Buffer<float> & weights, Buffer<Rect> & layout, PartitionAlg alg, bool useOrientation, float targetAR);
    static void layoutMoore(const TreeNode * parent, const Rect & rect, const Tree * tree, const Buffer<float> & weights, Buffer<Rect> & layout, PartitionAlg alg, bool useOrientation, float targetAR);
};
