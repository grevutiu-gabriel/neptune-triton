//
// File:   ColourMapList.cc
// Author: M.R.Eggleston
//
// Created on 06 July 2010, 10:20
//
// Converted to Qt March 2013 renamed from ColourMapTree.cc
//

#include <iostream>
#include "ColourMapList.h"

ColourMapList::ColourMapList(QWidget* parent)
:
QListWidget(parent),
m_colour_maps(ColourMaps::instance())
{
    load();

    connect(m_colour_maps.get(), SIGNAL(changed(const QString)),
            this,                SLOT(on_colour_map_changed(const QString)));
}

ColourMapList::~ColourMapList()
{
}

QString& ColourMapList::colourMapAtCursor()
{
    m_map_name = "";
    auto item = currentItem();
    if (item != nullptr)
    {
        m_map_name = item->text();
    }
    return m_map_name;
}

QString& ColourMapList::colourMapAfterCursor()
{
    m_map_name = "";
    int row = currentRow();
    if (row >= 0)
    {
        auto it = item(row + 1);
        if (it != nullptr)
        {
            m_map_name = it->text();
        }
    }
    return m_map_name;
}

void ColourMapList::setCursorAtMap(const QString& name)
{
    bool found = false;
    int row = 0;
    if (!name.isEmpty())
    {
        int size = count();
        for (int i = 0; i < size; i++)
        {
            auto it = item(i);
            if (it != nullptr)
            {
                found = (it->text() == name);
                if (found)
                {
                    row = i;
                    break;
                }
            }
        }
    }
    setCurrentRow(found ? row : -1);
}

bool ColourMapList::hasCursor()
{
    return (!colourMapAtCursor().isEmpty());
}

bool ColourMapList::isDefault()
{
    return ((m_map_name == "Default") ||
            (m_map_name == "Default 1") ||
            (m_map_name == "Default 2") ||
            (m_map_name == "Default 3") ||
            (m_map_name == "Default 4"));
}

void ColourMapList::load()
{
    clear();
    QStrings &maps = m_colour_maps->getNames();
    if (!maps.empty())
    {
        for (auto it = maps.begin(); it != maps.end(); it++)
        {
            addItem(*it);
        }
    }
}

void ColourMapList::on_colour_map_changed(const QString name)
{
    QString current_map;
    int size = static_cast<int>(m_colour_maps->size());

    auto row = currentRow();
    if (row != -1)
    {
        if (!m_colour_maps->nameExists(name))
        {
            // Deleted
            load();
            if (row >= size)
            {
                row--;
            }
            setCurrentRow(row);
        }
        else
        {
            load();
            setCursorAtMap(name);
        }
    }
    else
    {
        // No cursor currently displayed
        load();
    }
}
