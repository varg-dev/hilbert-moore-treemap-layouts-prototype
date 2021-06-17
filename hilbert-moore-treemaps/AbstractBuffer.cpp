
#include "AbstractBuffer.h"

#include "TreeNode.h"


AbstractBuffer::AbstractBuffer()
{
}

std::uint32_t AbstractBuffer::indexOf(const TreeNode *node) const
{
    return node->index();
}
