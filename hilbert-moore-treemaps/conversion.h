
// Adapted from https://github.com/cginternals/cppassist/

#pragma once


#include <string>


template <typename Type>
Type fromString(const std::string & string);

template <>
char fromString<char>(const std::string & string);

template <>
unsigned char fromString<unsigned char>(const std::string & string);

template <>
bool fromString<bool>(const std::string & string);

template <>
int fromString<int>(const std::string & string);


#include "conversion.inl"
