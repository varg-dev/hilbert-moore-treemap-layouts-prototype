
#include "TreemapLayouter.h"

#include "Rect.h"
#include "Tree.h"
#include "Buffer.h"

#include "SingleLevelHilbertMoore.h"

namespace
{


const auto rootRect = Rect(0.0f, 0.0f, 1.0f, 1.0f);


}


TreemapLayouter::TreemapLayouter()
: m_algorithm(RectangularLayoutAlgorithm::Hilbert_Greedy)
{
}

TreemapLayouter::~TreemapLayouter()
{
}

void TreemapLayouter::initialize()
{
}

void TreemapLayouter::deinitialize()
{
}

void TreemapLayouter::setAlgorithm(RectangularLayoutAlgorithm algorithm)
{
    m_algorithm = algorithm;
}

void TreemapLayouter::process(const Tree * tree, const Buffer<float> & weights,
    Buffer<Rect> & layout)
{
    layout[tree->root()] = rootRect;

    if (tree->size() <= 1)
    {
        return;
    }

    // Weight prefix sum, double precision not enough for mid-sized datasets
    // Buffer<double> prefixSum(std::vector<double>(weights.size()));
    // std::partial_sum(weights.begin(), weights.end(), prefixSum.begin());

    tree->parentsDo([this, &tree, &weights, &layout](const TreeNode * parent) {
        singleNodeProcess(tree, parent, weights, layout);
    });
}

void TreemapLayouter::singleNodeProcess(const Tree *tree, const TreeNode * parent, const Buffer<float> &weights, Buffer<Rect> &layout)
{
    const auto & workspace = layout[parent];

    switch (m_algorithm)
    {
    // Hilbert & Moore
    case RectangularLayoutAlgorithm::Moore_Greedy:
        SingleLevelHilbertMoore::layoutMoore(parent, workspace, tree, weights, layout, PartitionAlg::Greedy);
        break;
    case RectangularLayoutAlgorithm::Moore_MinMax:
        SingleLevelHilbertMoore::layoutMoore(parent, workspace, tree, weights, layout, PartitionAlg::MinMax);
        break;
    case RectangularLayoutAlgorithm::Moore_MinVariance:
        SingleLevelHilbertMoore::layoutMoore(parent, workspace, tree, weights, layout, PartitionAlg::Variance);
        break;
    case RectangularLayoutAlgorithm::Hilbert_Greedy:
        SingleLevelHilbertMoore::layoutHilbert(parent, workspace, tree, weights, layout, PartitionAlg::Greedy);
        break;
    case RectangularLayoutAlgorithm::Hilbert_MinMax:
        SingleLevelHilbertMoore::layoutHilbert(parent, workspace, tree, weights, layout, PartitionAlg::MinMax);
        break;
    case RectangularLayoutAlgorithm::Hilbert_MinVariance:
        SingleLevelHilbertMoore::layoutHilbert(parent, workspace, tree, weights, layout, PartitionAlg::Variance);
        break;

    default:
        break;
    }
}
