
#pragma once


#include <cstddef>
#include <functional>
#include <string>
#include <vector>
#include <cstdint>

#include <glm/vec4.hpp>


#ifndef LAYOUTER_INLINE
#ifndef NDEBUG
#define LAYOUTER_INLINE
#else
#define LAYOUTER_INLINE inline
#endif
#endif


class Tree;


class TreeNode
{
public:
    struct NodeRange
    {
        const TreeNode* first = nullptr;
        const TreeNode* last = nullptr; // exclusive

        LAYOUTER_INLINE NodeRange(const TreeNode* first, const TreeNode* last)
        : first(first)
        , last(last)
        {
            assert(first != nullptr);
            assert(last != nullptr);
            assert(first <= last);
        }

        LAYOUTER_INLINE bool empty() const
        {
            assert(first <= last);

            return first == last;
        }

        LAYOUTER_INLINE std::size_t size() const
        {
            assert(first <= last);

            return last - first;
        }
    };

public:
    inline TreeNode();
    inline TreeNode(std::uint32_t index, std::uint32_t depth, std::uint32_t parent);
    inline TreeNode(const TreeNode & node);
    inline TreeNode(TreeNode && node);

    inline ~TreeNode();

    inline TreeNode & operator=(const TreeNode & node);
    inline TreeNode & operator=(TreeNode && node);

    inline bool isLeaf() const;
    inline bool isRoot() const;

    inline std::uint32_t index() const;
    inline void setIndex(std::uint32_t index);

    inline std::uint32_t parent() const;
    inline void setParent(std::uint32_t index);

    inline std::uint32_t depth() const;
    inline void setDepth(std::uint16_t depth);

    inline std::uint32_t height() const;
    inline void setHeight(std::uint16_t height);

    inline std::uint32_t depthAndHeight() const;
    inline void setDepthAndHeight(std::uint32_t depthAndHeight);

    inline std::uint32_t firstChild() const;
    inline void setFirstChild(std::uint32_t nodeIndex);

    inline std::uint32_t numberOfChildren() const;
    inline std::uint32_t & numberOfChildren();
    inline void setNumberOfChildren(std::uint32_t count);

    inline operator glm::ivec4() const;

protected:
    std::uint32_t m_index;
    std::uint32_t m_depthAndHeight;
    std::uint32_t m_firstChild;
    std::uint32_t m_numberOfChildren;
    std::uint32_t m_parent;
};


#include "TreeNode.inl"
