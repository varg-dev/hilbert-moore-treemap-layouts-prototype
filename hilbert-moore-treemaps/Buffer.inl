
#pragma once


#include "Buffer.h"


template <typename T>
Buffer<T>::Buffer()
{
}

template <typename T>
Buffer<T>::Buffer(const std::vector<T>& data)
: m_data(data)
{
}

template <typename T>
Buffer<T>::Buffer(std::vector<T>&& data) 
: m_data(std::move(data))
{
}

template <typename T>
auto Buffer<T>::begin() -> iterator
{
    return m_data.begin();
}

template <typename T>
auto Buffer<T>::begin() const -> const_iterator
{
    return m_data.begin();
}

template <typename T>
auto Buffer<T>::end() -> iterator
{
    return m_data.end();
}

template <typename T>
auto Buffer<T>::end() const -> const_iterator
{
    return m_data.end();
}

template <typename T>
auto Buffer<T>::size() const -> size_type
{
    return static_cast<size_type>(m_data.size());
}

template <typename T>
void Buffer<T>::resize(const size_type size)
{
    m_data.resize(static_cast<size_type>(size));
}

template <typename T>
bool Buffer<T>::empty() const
{
    return m_data.empty();
}

template <typename T>
void Buffer<T>::clear()
{
    m_data.clear();
}

template <typename T>
auto Buffer<T>::operator[](const TreeNode * node) -> reference
{
    return m_data[indexOf(node)];
}

template <typename T>
auto Buffer<T>::operator[](const TreeNode * node) const -> const_reference
{
    return m_data[indexOf(node)];
}

template <typename T>
auto Buffer<T>::at(const TreeNode * node) -> reference
{
    return m_data[indexOf(node)];
}

template <typename T>
auto Buffer<T>::at(const TreeNode * node) const -> const_reference
{
    return m_data[indexOf(node)];
}

template <typename T>
template <typename IndexType, typename std::enable_if<std::is_integral<IndexType>::value>::type *>
auto Buffer<T>::operator[](IndexType index) -> reference
{
    return m_data[index];
}

template <typename T>
template <typename IndexType, typename std::enable_if<std::is_integral<IndexType>::value>::type *>
auto Buffer<T>::operator[](IndexType index) const -> const_reference
{
    return m_data[index];
}

template <typename T>
template <typename IndexType, typename std::enable_if<std::is_integral<IndexType>::value>::type *>
auto Buffer<T>::at(IndexType index) -> reference
{
    return m_data[index];
}

template <typename T>
template <typename IndexType, typename std::enable_if<std::is_integral<IndexType>::value>::type *>
auto Buffer<T>::at(IndexType index) const -> const_reference
{
    return m_data[index];
}

template <typename T>
auto Buffer<T>::front() -> reference
{
    return m_data.front();
}

template <typename T>
auto Buffer<T>::front() const -> const_reference
{
    return m_data.front();
}

template <typename T>
auto Buffer<T>::back() -> reference
{
    return m_data.back();
}

template <typename T>
auto Buffer<T>::back() const -> const_reference
{
    return m_data.back();
}

template <typename T>
T *  Buffer<T>::data()
{
    return m_data.data();
}

template <typename T>
const T * Buffer<T>::data() const
{
    return m_data.data();
}

template <typename T>
std::vector<T> & Buffer<T>::vector()
{
    return m_data;
}

template <typename T>
const std::vector<T> & Buffer<T>::vector() const
{
    return m_data;
}

template <typename T>
void Buffer<T>::setVector(const std::vector<T> & vector)
{
    m_data = vector;
}

template <typename T>
void Buffer<T>::setVector(std::vector<T> && vector)
{
    m_data = std::move(vector);
}
