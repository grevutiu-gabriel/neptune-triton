//
// File:   ManualColourContainer.h
// Author: M.R.Eggleston
//
// Created on 20 July 2010, 14:26
///

#pragma once

#include <QPushButton>
#include <QSpinBox>
#include <QTreeWidget>
#include "ColourEdit.h"
#include "ColourList.h"
#include "ColourMaps.h"
#include "SingleColour.h"

class ManualColourEdit : public ColourEdit
{
    Q_OBJECT
signals:
    void signal_map_changed();

private slots:
    void on_map_changed();

public:
    ManualColourEdit(QWidget* parent = nullptr);
    virtual ~ManualColourEdit();

    void setColourMap(const ColourMap &map);
    const ColourMap &getColourMap();

private:
    ManualColourEdit(const ManualColourEdit& orig);
    
    void setup_ui_layout();
    void connect_signals();

    ColourList* m_colour_list;

    boost::shared_ptr<ColourMaps> m_colour_maps;
};

