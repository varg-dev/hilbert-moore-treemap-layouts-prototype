
#include "AttributeSumAggregation.h"

#include <cmath>
#include <numeric>
#include <algorithm>

#include <glm/common.hpp>
#include <glm/exponential.hpp>

#include "Tree.h"
#include "TreeNode.h"


namespace
{


inline float sum_accum(const Buffer<float> & buffer, const Tree * /*tree*/, const TreeNode * /*parent*/, const TreeNode * first, const TreeNode * end)
{
    auto sum = 0.0f;

    for (auto current = first; current != end; ++current)
    {
        sum += buffer[current];
    }

    return sum;
}


} // namespace


AttributeSumAggregation::AttributeSumAggregation()
{
}

void AttributeSumAggregation::initialize()
{
}

void AttributeSumAggregation::deinitialize()
{
}

void AttributeSumAggregation::process(const Tree * tree, Buffer<float> & values)
{
    tree->reverseParentsDo([&](const TreeNode * node) {
        if (node->isLeaf())
        {
            return;
        }

        values[node] = sum_accum(values, tree, node, tree->at(node->firstChild()), tree->at(node->firstChild()) + node->numberOfChildren());
    });
}
