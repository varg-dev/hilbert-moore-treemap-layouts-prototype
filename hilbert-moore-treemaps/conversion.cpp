
// Adapted from https://github.com/cginternals/cppassist/

#include "conversion.h"

#include <cassert>
#include <sstream>


template <>
char fromString<char>(const std::string & string)
{
    using T = int;

    std::stringstream stream(string);
    auto value = T();
    stream >> value;
    return static_cast<char>(value);
}

template <>
unsigned char fromString<unsigned char>(const std::string & string)
{
    using T = unsigned int;

    std::stringstream stream(string);
    auto value = T();
    stream >> value;
    return static_cast<unsigned char>(value);
}

template <>
bool fromString<bool>(const std::string & string)
{
    return !(string == "" || string == "0" || string == "false");
}

template <>
int fromString<int>(const std::string & string)
{
    return static_cast<int>(std::strtol(string.data(),nullptr,10));
}
