//
// File:   ManualColourContainer.cc
// Author: M.R.Eggleston
//
// Created on 20 July 2010, 14:26
//

#include <iostream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ManualColourEdit.h"

ManualColourEdit::ManualColourEdit(QWidget* parent)
:
ColourEdit(parent),
m_colour_list(new ColourList(CT_ALL)),
//m_map_name(),
m_colour_maps(ColourMaps::instance())
{
    setup_ui_layout();
    connect_signals();
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
}

ManualColourEdit::~ManualColourEdit()
{
}

void ManualColourEdit::setup_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(m_colour_list);
    main_layout->addWidget(m_red_label);
    main_layout->addWidget(m_green_label);
    main_layout->addWidget(m_blue_label);
    main_layout->addWidget(m_colour_label);
    
    setLayout(main_layout);
}

void ManualColourEdit::connect_signals()
{
    connect(m_colour_list, SIGNAL(changed()),
            this,          SLOT(on_map_changed()));
}

void ManualColourEdit::setColourMap(const ColourMap& map)
{
    m_map = map;
    m_colour_list->clear();
    const ColoursMap& colours = m_map.getColoursMap();
    for (auto it = colours.begin(); it != colours.end(); it++)
    {
        m_colour_list->addRow(it->first, it->second);
    }
    m_colour_list->setColumns();
    draw_maps();
}

const ColourMap& ManualColourEdit::getColourMap()
{
    return m_map;
}

void ManualColourEdit::on_map_changed()
{
    m_map.clearManual();
    ColourEntryVector colours = m_colour_list->getColours();
    int size = colours.size();
    for (int i = 0; i < size; i++)
    {
        m_map.setColour(colours[i].index, colours[i].colour);
    }
    draw_maps();
}

