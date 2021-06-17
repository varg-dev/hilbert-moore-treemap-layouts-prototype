
#pragma once


#include <vector>
#include <type_traits>

#include "AbstractBuffer.h"


class Tree;
class TreeNode;


template <typename T>
class Buffer : public AbstractBuffer
{
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    using size_type = typename std::vector<T>::size_type;
    using reference = typename std::vector<T>::reference;
    using const_reference = typename std::vector<T>::const_reference;

public:
    Buffer();
    Buffer(const std::vector<T>& data);
    Buffer(std::vector<T>&& data);

    auto begin() -> iterator;
    auto begin() const -> const_iterator;
    auto end() -> iterator;
    auto end() const -> const_iterator;

    auto size() const -> size_type;
    void resize(size_type size);

    bool empty() const;
    void clear();

    auto operator[](const TreeNode * node) -> reference;
    auto operator[](const TreeNode * node) const -> const_reference;

    auto at(const TreeNode * node) -> reference;
    auto at(const TreeNode * node) const -> const_reference;

    template<typename IndexType, typename std::enable_if<std::is_integral<IndexType>::value>::type * = nullptr>
    auto operator[](IndexType index) -> reference;
    template<typename IndexType, typename std::enable_if<std::is_integral<IndexType>::value>::type * = nullptr>
    auto operator[](IndexType index) const -> const_reference;
    template<typename IndexType, typename std::enable_if<std::is_integral<IndexType>::value>::type * = nullptr>
    auto at(IndexType index) -> reference;
    template<typename IndexType, typename std::enable_if<std::is_integral<IndexType>::value>::type * = nullptr>
    auto at(IndexType index) const -> const_reference;

    auto front() -> reference;
    auto front() const -> const_reference;
    auto back() -> reference;
    auto back() const -> const_reference;

    T * data();
    const T * data() const;

    std::vector<T> & vector();
    const std::vector<T> & vector() const;
    void setVector(const std::vector<T> & vector);
    void setVector(std::vector<T> && vector);

protected:
    std::vector<T> m_data;
};


#include "Buffer.inl"
