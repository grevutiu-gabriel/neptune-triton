//
// File:   NumberEntry.cc
// Author: trevone
//
// Created on 15 October 2010, 12:52
//

#include <iomanip>
#include <limits>
#include <sstream>
#include "mpreal.h"
#include "Utilities.h"
#include "NumberEntry.h"



NumberEntry::NumberEntry(NumberEntry::TYPE type, QWidget* parent)
:
QLineEdit(parent),
m_value("0"),
m_last_value("0"),
m_type(type)
{
    connect(this, SIGNAL(editingFinished()),
            this, SLOT(on_editingFinished()));
}

NumberEntry::~NumberEntry()
{
}

void NumberEntry::disable()
{
    setText("");
    setEnabled(false);
}

void NumberEntry::Set(const std::string& value)
{
    bool ok = false;
    try
    {
        if (m_type == FLOAT)
        {
            boost::lexical_cast<long double>(value); // discard return value
            ok = true;
        }
        else
        {
            auto val = boost::lexical_cast<int>(value);
            ok = (m_type == INT) || ((m_type == UINT) && (val >= 0));
        }
    }
    catch(...)
    {
        // do nothing;   
    }
    if (ok)
    {
        m_value = value;
    }
    setText(QString::fromStdString(m_value));
    setEnabled(true);
}


const std::string& NumberEntry::Get()
{
    return m_value;
}

void NumberEntry::on_editingFinished()
{
    auto value = text().toStdString();
    if (m_value != value)
    {
        Set(value);
        emit changed();
    }
}
