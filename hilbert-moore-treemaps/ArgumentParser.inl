
// Adapted from https://github.com/cginternals/cppassist/ 

#include "conversion.h"


template <typename T>
T ArgumentParser::value(const std::string & option, const T & defaultValue) const
{
    const auto it = m_options.find(option);

    if (it == m_options.end())
    {
        return defaultValue;
    }

    return fromString<T>(it->second);
}
