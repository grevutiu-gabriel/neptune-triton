//
// File:   KeyString.h
// Author: M.R.Eggleston
//
// Created on 12 April 2011, 13:33
//

#pragma once

#include <map>
#include <QString>
#include "Utilities.h"

class KeyString
{
public:
    KeyString();
    KeyString(const KeyString& other);
    KeyString(const std::string& str);
    void SetKey(const std::string& key, const std::string& value);
    void SetKey(const std::string& key, int value);
    void SetKey(const std::string& key, long double value);
    std::string GetKey(const std::string& str, const std::string& default_value);
    int GetKeyAsInt(const std::string& str, int default_value);
    long double GetKeyAsLongDouble(const std::string& str, long double default_value);
    std::string String();

    virtual ~KeyString();
private:
    void getKeysFromString();


    std::map<std::string, std::string> m_map;
};

