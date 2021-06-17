
#pragma once


#include "TreeNode.h"


TreeNode::TreeNode()
: m_index(static_cast<std::uint32_t>(-1))
, m_depthAndHeight(0)
, m_firstChild(static_cast<std::uint32_t>(-1))
, m_numberOfChildren(0)
, m_parent(static_cast<std::uint32_t>(-1))
{
}

TreeNode::TreeNode(std::uint32_t index, std::uint32_t depth, std::uint32_t parent)
: m_index(index)
, m_depthAndHeight(depth << 16u | 0)
, m_firstChild(static_cast<std::uint32_t>(-1))
, m_numberOfChildren(0)
, m_parent(parent)
{
}

TreeNode::TreeNode(const TreeNode & node)
: m_index(node.index())
, m_depthAndHeight(node.depthAndHeight())
, m_firstChild(node.firstChild())
, m_numberOfChildren(node.numberOfChildren())
, m_parent(node.parent())
{
}

TreeNode::TreeNode(TreeNode && node)
: m_index(node.index())
, m_depthAndHeight(node.depthAndHeight())
, m_firstChild(node.firstChild())
, m_numberOfChildren(node.numberOfChildren())
, m_parent(node.parent())
{
}

TreeNode::~TreeNode()
{
}

std::uint32_t TreeNode::index() const
{
    return m_index;
}

void TreeNode::setIndex(std::uint32_t index)
{
    m_index = index;
}

bool TreeNode::isRoot() const
{
    return m_index == 0;
}

std::uint32_t TreeNode::parent() const
{
    return m_parent;
}

void TreeNode::setParent(std::uint32_t parent)
{
    m_parent = parent;
}

void TreeNode::setDepth(std::uint16_t depth)
{
    setDepthAndHeight(depth << 16u | height());
}

void TreeNode::setHeight(std::uint16_t height)
{
    setDepthAndHeight(depth() << 16u | height);
}

void TreeNode::setDepthAndHeight(std::uint32_t depthAndHeight)
{
    m_depthAndHeight = depthAndHeight;
}

void TreeNode::setFirstChild(std::uint32_t nodeIndex)
{
    m_firstChild = nodeIndex;
}

void TreeNode::setNumberOfChildren(std::uint32_t count)
{
    m_numberOfChildren = count;
}

std::uint32_t TreeNode::depth() const
{
    return m_depthAndHeight >> 16u;
}

std::uint32_t TreeNode::height() const
{
    return m_depthAndHeight & 0xFFFF;
}

std::uint32_t TreeNode::depthAndHeight() const
{
    return m_depthAndHeight;
}

TreeNode & TreeNode::operator=(const TreeNode & node)
{
    m_index = node.index();
    m_depthAndHeight = node.depthAndHeight();
    m_firstChild = node.firstChild();
    m_numberOfChildren = node.numberOfChildren();
    m_parent = node.parent();

    return *this;
}

TreeNode & TreeNode::operator=(TreeNode && node)
{
    m_index = node.index();
    m_depthAndHeight = node.depthAndHeight();
    m_firstChild = node.firstChild();
    m_numberOfChildren = node.numberOfChildren();
    m_parent = node.parent();

    return *this;
}

bool TreeNode::isLeaf() const
{
    return m_firstChild == static_cast<std::uint32_t>(-1);
}

std::uint32_t TreeNode::numberOfChildren() const
{
    return m_numberOfChildren;
}

std::uint32_t & TreeNode::numberOfChildren()
{
    return m_numberOfChildren;
}

std::uint32_t TreeNode::firstChild() const
{
    return m_firstChild;
}

TreeNode::operator glm::ivec4() const
{
    return glm::ivec4(m_index, m_depthAndHeight, m_firstChild, m_numberOfChildren);
}
