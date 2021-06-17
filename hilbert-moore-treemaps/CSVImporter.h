
#pragma once


#include <string>
#include <vector>


class Tree;
class TreeNode;


class CSVImporter
{
public:
    CSVImporter();
    CSVImporter(bool enableVerboseOutput);

    Tree * load(const std::string& filename, const std::string & weightAttribute, std::vector<std::string>* paths = nullptr);

protected:
    bool m_enableVerboseOutput;
};
