// 
// File:   ComponentColourEdit.h
// Author: M.R.Eggleston
//
// Created on 15 April 2013, 13:01
//

#pragma once

#include <QPushButton>
#include <QSpinBox>
#include <QTreeWidget>
#include "ColourList.h"
#include "ColourEdit.h"
#include "ColourMaps.h"

class ComponentColourEdit : public ColourEdit
{
    Q_OBJECT
signals:
    void signal_map_changed();

private slots:
    void on_reds_changed();
    void on_greens_changed();
    void on_blues_changed();

public:
    ComponentColourEdit(QWidget* parent = nullptr);
    virtual ~ComponentColourEdit();

    void setColourMap(const ColourMap &map);
    const ColourMap &getColourMap();

private:
    ComponentColourEdit(const ComponentColourEdit& orig);
    
    void setup_ui_layout();
    void connect_signals();

    void on_remove_button();
    void on_swap_above_button();
    void on_swap_below_button();
    void on_add_button();
    void on_replace_button();

    void on_edit_cursor_changed();
    void on_edit_map_changed();

    void enableWidgetsForMode();

    ColourList* m_red;
    ColourList* m_green;
    ColourList* m_blue;
};