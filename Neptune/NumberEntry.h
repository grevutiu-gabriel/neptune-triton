//
// File:   NumberEntry.h
// Author: M.R.Eggleston
//
// Created on 15 October 2010, 11:07
//

#pragma once

#include <iostream>
#ifdef NOT_MOC
#include <boost/lexical_cast.hpp>
#endif
#include <QLineEdit>

class NumberEntry : public QLineEdit
{
    Q_OBJECT
public:
    enum TYPE
    {
        UINT,
        INT,
        FLOAT
    };
    
signals:
    void changed();
            
private slots:
    void on_editingFinished();
 
public:
    explicit NumberEntry(TYPE, QWidget* parent=nullptr);
    virtual ~NumberEntry();
    void disable();
    const std::string& Get();
    void Set(const std::string& value);

protected:
    std::string m_value;
    std::string m_last_value;
    TYPE m_type;
};
