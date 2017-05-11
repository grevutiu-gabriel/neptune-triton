//
// File:   KeyString.cc
// Author: M.R.Eggleston
//
// Created on 12 April 2011, 13:33
//

#include <iomanip>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include "KeyString.h"

KeyString::KeyString()
{
}

KeyString::KeyString(const KeyString& other)
{
    m_map = other.m_map;
}

KeyString::KeyString(const std::string& str)
{
    boost::char_separator<char> sep(",=");
    boost::tokenizer<boost::char_separator<char>> tokens(str, sep);

    auto it = tokens.begin();
    while (it != tokens.end())
    {
        std::string key = *it;
        it++;
        if (it != tokens.end())
        {
            std::string value = *it;
            m_map[key] = *it;
            it++;
        }
    }
}

KeyString::~KeyString()
{
}

void KeyString::SetKey(const std::string& key, int value)
{
    SetKey(key, intToString(value));
}

void KeyString::SetKey(const std::string& key, long double value)
{
    SetKey(key, longdoubleToString(value));
}

void KeyString::SetKey(const std::string& key, const std::string& value)
{
    m_map[key] = value;
}

std::string KeyString::GetKey(const std::string& key, const std::string& default_value)
{
    std::string value = m_map[key];
    if (value.empty())
    {
        value = default_value;
    }
    return value;
}

int KeyString::GetKeyAsInt(const std::string& key, int default_value)
{
    std::string value = m_map[key];
    int ivalue = 0;
    try
    {
        ivalue = boost::lexical_cast<int>(value);
    }
    catch (boost::bad_lexical_cast&)
    {
        ivalue = default_value;
    }
    return ivalue;
}

long double KeyString::GetKeyAsLongDouble(const std::string& key, long double default_value)
{
    std::string value = m_map[key];
    long double ldvalue = 0;
    try
    {
        ldvalue = boost::lexical_cast<long double >(value);
    }
    catch (boost::bad_lexical_cast&)
    {
        ldvalue = default_value;
    }
    return ldvalue;
}

std::string KeyString::String()
{
    std::string str;
    for (auto it = m_map.begin(); it != m_map.end(); it++)
    {
        str += it->first;
        str += "=";
        str += it->second;
        str += ",";
    }
    auto length = str.length();
    str.erase(length-1);
    return str;
}
