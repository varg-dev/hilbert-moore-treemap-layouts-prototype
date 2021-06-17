
#include "Tree.h"

#include <cassert>
#include <iostream>


const std::uint32_t Tree::invalidIndex = static_cast<std::uint32_t>(-1);

Tree::Tree()
: m_numberOfLeaves(0)
{
}

Tree::~Tree()
{
}

std::uint32_t Tree::size() const
{
    return m_nodes.size();
}

std::uint32_t Tree::numberOfParents() const
{
    return size() - numberOfLeaves();
}

void Tree::setNumberOfLeaves(std::uint32_t count)
{
    m_numberOfLeaves = count;
}

std::uint32_t Tree::numberOfLeaves() const
{
    return m_numberOfLeaves;
}

std::uint32_t Tree::depth() const
{
    return m_slices.size();
}

TreeNode * Tree::at(std::uint32_t index)
{
    if (index >= m_nodes.size())
    {
        return nullptr;
    }

    return &m_nodes.at(index);
}

const TreeNode * Tree::at(std::uint32_t index) const
{
    if (index >= m_nodes.size())
    {
        return nullptr;
    }

    return &m_nodes.at(index);
}

TreeNode * Tree::operator[](std::uint32_t index)
{
    if (index >= m_nodes.size())
    {
        return nullptr;
    }

    return &m_nodes.at(index);
}

const TreeNode * Tree::operator[](std::uint32_t index) const
{
    if (index >= m_nodes.size())
    {
        return nullptr;
    }

    return &m_nodes.at(index);
}

TreeNode::NodeRange Tree::nodesAsRange() const
{
    return { root(), root() + size() };
}

TreeNode::NodeRange Tree::childrenAsRange(const TreeNode * parent) const
{
    const auto firstChild = at(parent->firstChild());

    return { firstChild, firstChild+parent->numberOfChildren() };
}

const TreeNode* Tree::root() const
{
    return &m_nodes.front();
}

TreeNode* Tree::root()
{
    return &m_nodes.front();
}

bool Tree::hasAttribute(const std::string & name) const
{
    return m_attributes.find(name) != m_attributes.end();
}

void Tree::addAttribute(const std::string & name)
{
    m_attributes.emplace(name, Buffer<float>(std::vector<float>()));
}

Buffer<float> & Tree::attributeValues(const std::string & name)
{
    return m_attributes.at(name);
}

const Buffer<float> & Tree::attributeValues(const std::string & name) const
{
    return m_attributes.at(name);
}

std::vector<std::pair<std::uint32_t, std::uint32_t>> & Tree::slices()
{
    return m_slices;
}

const std::vector<std::pair<std::uint32_t, std::uint32_t>> & Tree::slices() const
{
    return m_slices;
}

const Buffer<TreeNode> & Tree::nodes() const
{
    return m_nodes;
}

Buffer<TreeNode> & Tree::nodes()
{
    return m_nodes;
}

void Tree::setNodes(const Buffer<TreeNode> & nodes)
{
    m_nodes = nodes;
}

void Tree::setNodes(Buffer<TreeNode> && nodes)
{
    m_nodes = std::move(nodes);
}

void Tree::outputStatistics(std::ostream & stream) const
{
    stream << "Number of nodes: " << size() << '\n';
    stream << "Number of parents: " << numberOfParents() << '\n';
    stream << "Number of leaves: " << numberOfLeaves() << '\n';
    stream << "Maximum depth: " << depth() << '\n';
    stream.flush();
}
