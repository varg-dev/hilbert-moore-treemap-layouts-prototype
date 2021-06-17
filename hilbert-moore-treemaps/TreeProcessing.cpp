
#include "TreeProcessing.h"

#include <numeric>

#include "Tree.h"
#include "TreeNode.h"


TreeProcessing::TreeProcessing()
{
}


Tree * TreeProcessing::removeZeroWeightNodesAndDeleteSource(Tree *tree, const std::string &weightAttribute, std::vector<std::string> *paths)
{
    const auto result = removeZeroWeightNodes(tree, weightAttribute, paths);

    delete tree;

    return result;
}


Tree * TreeProcessing::removeZeroWeightNodes(Tree * tree, const std::string & weightAttribute, std::vector<std::string>* paths)
{
    if (tree == nullptr || tree->size() < 1)
    {
        return nullptr;
    }

    auto prunedTree = new Tree();
    prunedTree->addAttribute(weightAttribute);

    const auto & weightValues = tree->attributeValues(weightAttribute);

    constexpr auto zeroWeightThreshold = 0.0f;

    std::vector<std::size_t> subtreeSize(tree->size());
    auto numberOfLeaves = std::size_t(0);
    tree->reverseNodesDo([& subtreeSize, &weightValues, &numberOfLeaves](const TreeNode * node) {
        if (node->isLeaf())
        {
            numberOfLeaves += static_cast<std::size_t>(weightValues[node] > zeroWeightThreshold);
            subtreeSize[node->index()] = static_cast<std::size_t>(weightValues[node] > zeroWeightThreshold);
        }
        else
        {
            subtreeSize[node->index()] = std::accumulate(subtreeSize.begin() + node->firstChild(), subtreeSize.begin() + node->firstChild() + node->numberOfChildren(), std::size_t(0));

            if (subtreeSize[node->index()] > 0)
            {
                ++subtreeSize[node->index()];
            }
        }
    });

    if (subtreeSize.front() < 1)
    {
        return nullptr;
    }

    std::vector<TreeNode> nodes(subtreeSize.front());
    std::vector<TreeNode *> previousNodes(nodes.size());
    auto & prunedWeightValues = prunedTree->attributeValues(weightAttribute);
    prunedWeightValues.setVector(std::vector<float>(nodes.size()));
    previousNodes[0] = tree->at(0);
    prunedTree->setNumberOfLeaves(numberOfLeaves);

    auto nextNodeId = std::size_t(1);
    auto & slices = prunedTree->slices();
    auto startOfNextSlice = std::size_t(1);
    slices.emplace_back(0, 0);
    for (auto i = std::size_t(0); i < nodes.size(); ++i)
    {
        if (startOfNextSlice == i)
        {
            slices.emplace_back(startOfNextSlice, nextNodeId-1);
            startOfNextSlice = nextNodeId;
        }
        const auto previousNode = previousNodes[i];

        prunedWeightValues.at(i) = weightValues.at(previousNode);

        nodes[i].setIndex(i);
        nodes[i].setDepth(slices.size()-1);

        if (previousNode->isLeaf())
        {
            continue;
        }

        nodes[i].setFirstChild(nextNodeId);
        for (auto childI = previousNode->firstChild(); childI < previousNode->firstChild() + previousNode->numberOfChildren(); ++childI)
        {
            if (subtreeSize[childI] == 0)
            {
                continue;
            }

            previousNodes[nextNodeId] = tree->at(childI);
            nodes[nextNodeId].setParent(i);
            ++nextNodeId;
        }
        nodes[i].setNumberOfChildren(nextNodeId - nodes[i].firstChild());
    }
    slices.back().second = nextNodeId-1;
    prunedTree->setNodes(Buffer<TreeNode>(std::move(nodes)));

    if (paths)
    {
        assert(paths->size() == tree->size());
        for (int i = 0; i < prunedTree->size(); ++i) {
            assert(previousNodes[i]->index() >= i);
            paths->operator[](i) = paths->operator[](previousNodes[i]->index());
        }
        paths->resize(prunedTree->size());
    }

    return prunedTree;
}


Tree * TreeProcessing::flattenAndDeleteSource(Tree * tree, const std::string & weightAttribute, std::vector<std::string>* paths)
{
    const auto result = flatten(tree, weightAttribute, paths);

    delete tree;

    return result;
}


// TODO: update paths
Tree * TreeProcessing::flatten(Tree * tree, const std::string & weightAttribute, std::vector<std::string>* /*paths*/)
{
    auto flattedTree = new Tree();
    flattedTree->setNumberOfLeaves(tree->numberOfLeaves());
    flattedTree->addAttribute(weightAttribute);

    const auto & weightValues = tree->attributeValues(weightAttribute);

    std::vector<TreeNode> nodes;
    auto & slices = flattedTree->slices();
    auto & flattedWeightValues = flattedTree->attributeValues(weightAttribute);
    flattedWeightValues.setVector(std::vector<float>(tree->numberOfLeaves() + 1));

    slices.resize(2);
    slices.front() = { 0, 0 };
    slices.back() = { 1, tree->numberOfLeaves() };

    nodes.emplace_back(0, 0, -1);

    auto & root = nodes.front();

    tree->leavesDo([&nodes, &root, &weightValues, &flattedWeightValues](const TreeNode * leaf) {
        nodes.emplace_back(nodes.size(), 1, 0);
        flattedWeightValues[nodes.back().index()] = weightValues[leaf];
    });
    nodes[0].setFirstChild(1);
    nodes[0].setNumberOfChildren(tree->numberOfLeaves());
    flattedWeightValues[0] = 0.0f;
    flattedTree->setNodes(Buffer<TreeNode>(std::move(nodes)));

    return flattedTree;
}
