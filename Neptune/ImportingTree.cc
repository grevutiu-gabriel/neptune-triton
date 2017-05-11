// 
// File:   ImportingTree.cc
// Author: M.R.Eggleston
// 
// Created on 23 February 2012, 15:00
//

#include <iostream>
#include "ImportingTree.h"

ImportingTree::ImportingTree(QWidget* parent)
:
QTreeWidget(parent)
{
    setHeaderHidden(true);
    setColumnCount(3);
    setColumnHidden(0, true); // -> collection name
    // column 1 -> import name must be unique
    setColumnHidden(2, true); // -> name
}

ImportingTree::~ImportingTree()
{
}

void ImportingTree::addMap(const QString& import_name, const QString& collection_name, const QString& name)
{
    auto item = new QTreeWidgetItem();
    item->setText(0, collection_name);
    item->setText(1, import_name);
    item->setText(2, name);
    addTopLevelItem(item);
    sortItems(1, Qt::AscendingOrder);
}

std::vector<map_full_identity>& ImportingTree::getMaps()
{
    m_map_identities.clear();
    int size = topLevelItemCount();
    for (int i = 0; i < size; i++)
    {
        map_full_identity id;
        auto item = topLevelItem(i);
        id.collection = item->text(0);
        id.import_name = item->text(1);
        id.name = item->text(2);
        m_map_identities.push_back(id);
    }
    return m_map_identities;
}

map_identity ImportingTree::colourMapAtCursor()
{
    map_identity id;
    auto item = currentItem();
    if (item != nullptr)
    {
        id.collection = item->text(0);
        id.name = item->text(2);
    }
    return id;
}

bool ImportingTree::mapExists(const QString& collection, const QString& name)
{
    bool exists = false;
    int size = topLevelItemCount();
    for (int i = 0; i < size; i++)
    {
        auto item = topLevelItem(i);
        if (item != nullptr)
        {
            if ((item->text(0) == collection) && (item->text(2) == name))
            {
                exists = true;
                break;
            }
        }
    }
    return exists;
}

bool ImportingTree::importingMapExists(const QString& name)
{
    bool exists = false;
    int size = topLevelItemCount();
    for (int i = 0; i < size; i++)
    {
        auto item = topLevelItem(i);
        if (item != nullptr)
        {
            if (item->text(1) == name)
            {
                exists = true;
                break;
            }
        }
    }
    return exists;
}
    
bool ImportingTree::hasCursor()
{
    return (currentItem() != nullptr);
}

void ImportingTree::setCursorAtMap(const QString& name)
{
    QTreeWidgetItem* found_item = nullptr;
    int size = topLevelItemCount();
    for (int i = 0; i < size; i++)
    {
        auto item = topLevelItem(i);
        if (item != nullptr)
        {
            if (item->text(1) == name)
            {
                found_item = item;
                break;
            }
        }
    }
    if (found_item != nullptr)
    {
        setCurrentItem(found_item);
    }
}

void ImportingTree::setCursorAtMap(const QString& collection, const QString& name)
{
    map_identity id = colourMapAtCursor();
    if (name.isEmpty())
    {
        setCurrentItem(nullptr);
    }
    else if ((id.collection != collection) || (id.name != name))
    {
        QTreeWidgetItem* found_item = nullptr;
        int size = topLevelItemCount();
        for (int i = 0; i < size; i++)
        {
            auto item = topLevelItem(i);
            if ((item->text(0) == collection) && (item->text(2) == name))
            {
                found_item = item;
            }
        }
        if (found_item != nullptr)
        {
            setCurrentItem(found_item);
        }
    }
}

void ImportingTree::deleteCollection(const QString& collection)
{
    int size = topLevelItemCount();
    int index = 0;
    while (index < size)
    {
        bool item_deleted = false;
        auto item = topLevelItem(index);
        if (item != nullptr)
        {
            if (item->text(0) == collection)
            {
                takeTopLevelItem(index);
                size = topLevelItemCount();
                item_deleted = true;
            }
        }
        if (!item_deleted)
        {
            index++;
        }
    }
}

void ImportingTree::deleteImportingMap(const QString& collection, const QString& name)
{
    int size = topLevelItemCount();
    for (int i = 0; i < size; i++)
    {
        auto item = topLevelItem(i);
        if (item != nullptr)
        {
            if ((item->text(0) == collection) && (item->text(2) == name))
            {
                takeTopLevelItem(i);
                break;
            }
        }
    }
}
