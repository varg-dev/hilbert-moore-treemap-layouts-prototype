
#include "TreemapLayouter.h"

#include "Rect.h"
#include "Tree.h"
#include "Buffer.h"

#include "SingleLevelHilbertMoore.h"

namespace
{


const auto s_rootRect = Rect(0.0f, 0.0f, 1.0f, 1.0f);

Rect rootRect(float rootAR) {
    return Rect(s_rootRect.left(), s_rootRect.bottom(), s_rootRect.width() * rootAR, s_rootRect.height());
}


}


TreemapLayouter::TreemapLayouter()
: m_algorithm(RectangularLayoutAlgorithm::Hilbert_Greedy)
, m_rootAR(1.0f)
, m_targetAR(1.0f)
, m_useOrientation(true)
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

void TreemapLayouter::setRootAR(float ar)
{
    m_rootAR = ar;
}

void TreemapLayouter::setTargetAR(float ar)
{
    m_targetAR = ar;
}

void TreemapLayouter::useOrientation(bool orient)
{
    m_useOrientation = orient;
}

void TreemapLayouter::process(const Tree * tree, const Buffer<float> & weights,
    Buffer<Rect> & layout)
{
    layout[tree->root()] = rootRect(m_rootAR);

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
        SingleLevelHilbertMoore::layoutMoore(parent, workspace, tree, weights, layout, PartitionAlg::Greedy, m_useOrientation, m_targetAR);
        break;
    case RectangularLayoutAlgorithm::Moore_MinMax:
        SingleLevelHilbertMoore::layoutMoore(parent, workspace, tree, weights, layout, PartitionAlg::MinMax, m_useOrientation, m_targetAR);
        break;
    case RectangularLayoutAlgorithm::Moore_MinVariance:
        SingleLevelHilbertMoore::layoutMoore(parent, workspace, tree, weights, layout, PartitionAlg::Variance, m_useOrientation, m_targetAR);
        break;
    case RectangularLayoutAlgorithm::Hilbert_Greedy:
        SingleLevelHilbertMoore::layoutHilbert(parent, workspace, tree, weights, layout, PartitionAlg::Greedy, m_useOrientation, m_targetAR);
        break;
    case RectangularLayoutAlgorithm::Hilbert_MinMax:
        SingleLevelHilbertMoore::layoutHilbert(parent, workspace, tree, weights, layout, PartitionAlg::MinMax, m_useOrientation, m_targetAR);
        break;
    case RectangularLayoutAlgorithm::Hilbert_MinVariance:
        SingleLevelHilbertMoore::layoutHilbert(parent, workspace, tree, weights, layout, PartitionAlg::Variance, m_useOrientation, m_targetAR);
        break;

    default:
        break;
    }
}
