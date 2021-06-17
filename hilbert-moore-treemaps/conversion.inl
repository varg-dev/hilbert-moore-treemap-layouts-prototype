
// Adapted from https://github.com/cginternals/cppassist/ 

#pragma once


#include <sstream>


template <typename Type>
Type fromString(const std::string & string)
{
    std::stringstream stream(string);
    auto value = typename std::remove_const<Type>::type();
    stream >> value;
    return value;
}

template <typename Type>
std::string toString(const Type & value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}
