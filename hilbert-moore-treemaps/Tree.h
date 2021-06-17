
#pragma once


#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <iosfwd>

#include "TreeNode.h"
#include "Buffer.h"




enum class IterationOrder : unsigned int
{
    ALL_NODES_TOP_DOWN,
    ALL_NODES_BOTTOM_UP,
    LEAVES,
    PARENTS_TOP_DOWN,
    PARENTS_BOTTOM_UP
};


class Tree
{
public:
    static const std::uint32_t invalidIndex;

    Tree();
	virtual ~Tree();

    std::uint32_t size() const;

    std::uint32_t numberOfParents() const;

    std::uint32_t numberOfLeaves() const;
    void setNumberOfLeaves(std::uint32_t count);

    std::uint32_t depth() const;

    TreeNode * at(std::uint32_t index);
    const TreeNode * at(std::uint32_t index) const;
    TreeNode * operator[](std::uint32_t index);
    const TreeNode * operator[](std::uint32_t index) const;

    TreeNode::NodeRange nodesAsRange() const;
    TreeNode::NodeRange childrenAsRange(const TreeNode * parent) const;

    const TreeNode* root() const;
    TreeNode* root();

    template <typename Callback>
    void parentsDo(Callback callback) const;
    template <typename Callback>
    void reverseParentsDo(Callback callback) const;
    template <typename Callback>
    void leavesDo(Callback callback) const;
    template <typename Callback>
    void nodesDo(Callback callback) const;
    template <typename Callback>
    void reverseNodesDo(Callback callback) const;

    template <typename Callback>
    void nodesDepthFirstDo(Callback callback) const;
    template <typename Callback>
    void nodesDepthFirstDo(const TreeNode * node, Callback callback) const;
    template <typename Callback>
    void childrenDo(const TreeNode * node, Callback callback) const;

    bool hasAttribute(const std::string & name) const;
    void addAttribute(const std::string & name);
    Buffer<float> & attributeValues(const std::string & name);
    const Buffer<float> & attributeValues(const std::string & name) const;

    std::vector<std::pair<std::uint32_t, std::uint32_t>> & slices();
    const std::vector<std::pair<std::uint32_t, std::uint32_t>> & slices() const;

    const Buffer<TreeNode> & nodes() const;
    Buffer<TreeNode> & nodes();
    void setNodes(const Buffer<TreeNode> & nodes);
    void setNodes(Buffer<TreeNode> && nodes);

    void outputStatistics(std::ostream & stream) const;

    template <typename Callback, IterationOrder Order>
    void slicesDo(Callback && callback) const;

    template <typename Callback>
    void slicesDo(Callback && callback, IterationOrder order) const;

protected:
    std::uint32_t m_numberOfLeaves;

    Buffer<TreeNode> m_nodes;
    std::map<std::string, Buffer<float>> m_attributes;

    std::vector<std::pair<std::uint32_t, std::uint32_t>> m_slices;

    template <typename Callback>
    void sliceDo(std::uint32_t index, Callback callback) const;
};


#include "Tree.inl"
