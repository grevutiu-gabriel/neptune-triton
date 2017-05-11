// 
// File:   ColourComponent.h
// Author: M.R.Eggleston
//
// Created on 15 April 2013, 13:29
//

#pragma once

#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTreeWidget>
#include "SingleColour.h"
#include "TypesAndConstants.h"

struct colourEntry
{
    colourEntry(int i, const QColor& c) : index(i), colour(c) {}
    int    index;
    QColor colour;
};
typedef std::vector<colourEntry> ColourEntryVector;


class ColourList : public QWidget
{
    Q_OBJECT
signals:
    void changed();
    
private slots:
    void on_cursor_changed(QTreeWidgetItem*, QTreeWidgetItem*);
    void on_index_spin(int value);
    void on_set(bool clicked);
    void on_add(bool clicked);
    void on_delete(bool clicked);
    void on_cancel(bool clicked);
    void on_above(bool clicked);
    void on_below(bool clicked);

public:
    ColourList(COLOUR_TYPE colour, QWidget* parent = nullptr);
    ~ColourList();
    
    void clear();
    void addRow(int index, const QColor& colour);
    void setColumns();
    
    int size();
    int indexAtRow(int row);
    QColor colourAtRow(int row);
    ColourEntryVector getColours();
    
private:
    ColourList();
    ColourList(const ColourList& other);

    void setup_ui_layout();
    void connect_signals();
    void set_buttons(int row);
    int rowWithIndex(int index);
    void deleteFromList(int index);
    void addToList(int ind, const QColor& colour);
    void deleteFromDisplayList(int index);
    void addToDisplayList(int ind, const QColor& colour);
    
    COLOUR_TYPE m_list_type;
    SingleColour* m_colour;
    QSpinBox* m_index_spin;
    QPushButton* m_add_button;
    QPushButton* m_set_button;
    QPushButton* m_delete_button;
    QPushButton* m_cancel_button;
    QPushButton* m_swap_above_button;
    QPushButton* m_swap_below_button;
    QTreeWidget* m_display_list;

    ColourEntryVector m_list;
};

