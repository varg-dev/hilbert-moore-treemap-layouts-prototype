
// Adapted from https://github.com/cginternals/cppassist/ 

#include "ArgumentParser.h"

#include <iostream>


ArgumentParser::ArgumentParser()
{
}

ArgumentParser::~ArgumentParser()
{
}

void ArgumentParser::parse(int argc, char * argv[])
{
    m_options.clear();
    m_params.clear();

    for (int i=1; i<argc; i++)
    {
        // Get current and next argument
        std::string arg  = argv[i];
        std::string next = (i+1 < argc ? argv[i+1] : "");

        // Options with value (--option-name <value>)
        if (arg.find("--") == 0)
        {
            // Save value
            m_options[std::move(arg)] = std::move(next);
            i++;
        }

        // Options without value (-option-name)
        else if (arg.find("-") == 0)
        {
            m_options[std::move(arg)] = "true";
        }

        // Additional parameters
        else
        {
            m_params.push_back(std::move(arg));
        }
    }
}

const std::map<std::string, std::string> & ArgumentParser::options() const
{
    return m_options;
}

bool ArgumentParser::isSet(const std::string & option) const
{
    return m_options.count(option) > 0;
}

std::string ArgumentParser::value(const std::string & option, const std::string & defaultValue) const
{
    const auto it = m_options.find(option);

    if (it == m_options.end())
    {
        return defaultValue;
    }

    return it->second;
}

const std::string & ArgumentParser::value(const std::string & option, std::string & defaultValue) const
{
    const auto it = m_options.find(option);

    if (it == m_options.end())
    {
        return defaultValue;
    }

    return it->second;
}

const std::vector<std::string> & ArgumentParser::params() const
{
    return m_params;
}

void ArgumentParser::print() const
{
    std::cout << "Options:" << std::endl;

    for (const auto & param : m_options)
    {
        const auto & option = param.first;
        const auto & value  = param.second;
        std::cout << "  " << option << " = " << value << std::endl;
    }

    std::cout << std::endl;

    std::cout << "Parameters:" << std::endl;

    for (const auto & param : m_params)
    {
        std::cout << "  " << param << std::endl;
    }

    std::cout << std::endl;
}
