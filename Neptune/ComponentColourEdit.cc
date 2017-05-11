// 
// File:   ComponentColourEdit.cc
// Author: M.R.Eggleston
//
// Created on 15 April 2013, 13:16
//

#include <QVBoxLayout>
#include <QTabWidget>
#include "ComponentColourEdit.h"

ComponentColourEdit::ComponentColourEdit(QWidget* parent)
:
ColourEdit(parent),
m_red(new ColourList(CT_RED)),
m_green(new ColourList(CT_GREEN)),
m_blue(new ColourList(CT_BLUE))
{
    setup_ui_layout();
    connect_signals();
}

ComponentColourEdit::~ComponentColourEdit()
{
}

void ComponentColourEdit::setup_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    auto tabs = new QTabWidget;
    tabs->addTab(m_red, "Red");
    tabs->addTab(m_green, "Green");
    tabs->addTab(m_blue, "Blue");
    main_layout->addWidget(tabs);
    main_layout->addWidget(m_red_label);
    main_layout->addWidget(m_green_label);
    main_layout->addWidget(m_blue_label);
    main_layout->addWidget(m_colour_label);
    
    setLayout(main_layout);
}

void ComponentColourEdit::connect_signals()
{
    connect(m_red,   SIGNAL(changed()),
            this,    SLOT(on_reds_changed()));
    connect(m_green, SIGNAL(changed()),
            this,    SLOT(on_greens_changed()));
    connect(m_blue,  SIGNAL(changed()),
            this,    SLOT(on_blues_changed()));
}

void ComponentColourEdit::setColourMap(const ColourMap& map)
{
    m_map = map;
    m_map.setType(ColourMap::CM_COMPONENT);
    m_red->clear();
    m_green->clear();
    m_blue->clear();
    const IntMap& reds = m_map.getRedMap();
    for (auto it = reds.begin(); it != reds.end(); it++)
    {
        m_red->addRow(it->first, QColor(it->second, 0, 0));
    }
    m_red->setColumns();
    const IntMap& greens = m_map.getGreenMap();
    for (auto it = greens.begin(); it != greens.end(); it++)
    {
        m_green->addRow(it->first, QColor(0, it->second, 0));
    }
    m_green->setColumns();
    const IntMap& blues = m_map.getBlueMap();
    for (auto it = blues.begin(); it != blues.end(); it++)
    {
        m_blue->addRow(it->first, QColor(0, 0, it->second));
    }
    m_blue->setColumns();
    draw_maps();
}

const ColourMap &ComponentColourEdit::getColourMap()
{
    return m_map;
}

void ComponentColourEdit::on_reds_changed()
{
    m_map.clearReds();
    ColourEntryVector colours = m_red->getColours();
    int size = colours.size();
    for (int i = 0; i < size; i++)
    {
        m_map.setRed(colours[i].index, colours[i].colour.red());
    }
    draw_maps();
}

void ComponentColourEdit::on_greens_changed()
{
    m_map.clearGreens();
    ColourEntryVector colours = m_green->getColours();
    int size = colours.size();
    for (int i = 0; i < size; i++)
    {
        m_map.setGreen(colours[i].index, colours[i].colour.green());
    }
    draw_maps();
}

void ComponentColourEdit::on_blues_changed()
{
    m_map.clearBlues();
    ColourEntryVector colours = m_blue->getColours();
    int size = colours.size();
    for (int i = 0; i < size; i++)
    {
        m_map.setBlue(colours[i].index, colours[i].colour.blue());
    }
    draw_maps();
}
