
#pragma once


#include <string>
#include <vector>


class Tree;


class TreeProcessing
{
public:
    TreeProcessing();

    Tree * removeZeroWeightNodes(Tree * tree, const std::string & weightAttribute, std::vector<std::string>* paths = nullptr);
    Tree * removeZeroWeightNodesAndDeleteSource(Tree * tree, const std::string & weightAttribute, std::vector<std::string>* paths = nullptr);
    Tree * flatten(Tree * tree, const std::string & weightAttribute, std::vector<std::string>* paths = nullptr);
    Tree * flattenAndDeleteSource(Tree * tree, const std::string & weightAttribute, std::vector<std::string>* paths = nullptr);
};
