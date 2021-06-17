
#pragma once


#include <cstdint>


class TreeNode;


class AbstractBuffer
{
public:
    AbstractBuffer();

    std::uint32_t indexOf(const TreeNode *node) const;
};
