// 
// File:   ImportSourceTree.cc
// Author: M.R.Eggleston
// 
// Created on 23 February 2012, 16:11
//

#include <iostream>
#include "ImportSourceTree.h"

ImportSourceTree::ImportSourceTree(QWidget* parent)
:
QTreeWidget(parent)
{
    setHeaderHidden(true);
    setColumnCount(1);
}

ImportSourceTree::~ImportSourceTree()
{
}

void ImportSourceTree::addMap(const QString& collection, const QString& name)
{
    bool found = false;
    int size = topLevelItemCount();
    for (int i = 0; i < size; i++)
    {
        auto item = topLevelItem(i);
        if (item->text(0) == collection)
        {
            auto child = new QTreeWidgetItem();
            child->setText(0, name);
            item->addChild(child);
            found = true;
            break;
        }
    }
    if (!found)
    {
        auto item = new QTreeWidgetItem();
        item->setText(0, collection);
        auto child = new QTreeWidgetItem();
        child->setText(0, name);
        item->addChild(child);
        addTopLevelItem(item);
    }
}

QStringList& ImportSourceTree::getMapsAtCursor(QString& collection)
{
    m_map_names.clear();

    auto item = currentItem();
    if (item != nullptr)
    {
        auto parent = item->parent();
        if (parent != nullptr)
        {
            // Has a parent only return single map
            collection = parent->text(0);
            m_map_names.append(item->text(0));
        }
        else
        {
            // Must be a parent, return all the children
            collection = item->text(0);
            int size = item->childCount();
            for (int i = 0; i < size; i++)
            {
                auto child = item->child(i);
                if (child != nullptr)
                {
                    m_map_names.append(child->text(0));
                }
            }
        }
    }
    return m_map_names;
}

void ImportSourceTree::removeMap(const QString& collection, const QString& name)
{
    int size = topLevelItemCount();
    for (int i = 0; i < size; i++)
    {
        auto item = topLevelItem(i);
        if (item != nullptr)
        {
            if (item->text(0) == collection)
            {
                int children = item->childCount();
                for (int j = 0; j < children; j++)
                {
                    auto child = item->child(j);
                    if (child != nullptr)
                    {
                        if (child->text(0) == name)
                        {
                            item->takeChild(j);
                            if (item->childCount() == 0)
                            {
                                int index = indexOfTopLevelItem(item);
                                if (index != -1)
                                {
                                    takeTopLevelItem(index);
                                }
                            }
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
}

void ImportSourceTree::removeCollection(const QString& collection)
{
    int size = topLevelItemCount();
    for (int i = 0; i < size; i++)
    {
        auto item = topLevelItem(i);
        if (item != nullptr)
        {
            if (item->text(0) == collection)
            {
                int index = indexOfTopLevelItem(item);
                if (index != -1)
                {
                    takeTopLevelItem(index);
                }
            }
        }
    }
}

map_identity ImportSourceTree::colourMapAtCursor()
{
    map_identity id;
    auto item = currentItem();
    if (item != nullptr)
    {
        auto parent = item->parent();
        if (parent != nullptr)
        {
            // Has a parent only return single map
            id.collection = parent->text(0);
            id.name = item->text(0);
        }
        else
        {
            // Must be a parent, return all the children
            id.collection = item->text(0);
        }
    }
    return id;
}

void ImportSourceTree::deleteBranchAtCursor()
{
    auto item = currentItem();
    if (item != nullptr)
    {
        int index = indexOfTopLevelItem(item);
        if (index != -1)
        {
            takeTopLevelItem(index);
        }
    }
}

void ImportSourceTree::deleteItemAtCursor()
{
    auto item = currentItem();
    if (item != nullptr)
    {
        auto parent = item->parent();
        if (parent != nullptr)
        {
            int index = parent->indexOfChild(item);
            parent->takeChild(index);
            if (parent->childCount() == 0)
            {
                index = indexOfTopLevelItem(parent);
                takeTopLevelItem(index);
            }
        }
    }
}

void ImportSourceTree::setCursorAtMap(const QString& collection, const QString& name)
{
    map_identity id = colourMapAtCursor();
    if ((id.collection != collection) || (id.name != name))
    {
        auto size = topLevelItemCount();
        for (int i = 0; i < size; i++)
        {
            auto item = topLevelItem(i);
            if (item != nullptr)
            {
                if (item->text(0) == collection)
                {
                    int children = item->childCount();
                    for (int j = 0; j < children; j++)
                    {
                        auto child = item->child(j);
                        if (child != nullptr)
                        {
                            if (child->text(0) == name)
                            {
                                setCurrentItem(child);
                            }
                        }
                    }
                }
            }
        }
    }
}

map_identity ImportSourceTree::getMapAtNext()
{
    map_identity id;
    auto item = currentItem();
    if (item != nullptr)
    {
        QTreeWidgetItem* parent = nullptr;
        auto next = itemBelow(item);
        if (next == nullptr)
        {
            // No item below get data from current item
            next = item;
        }
        if (next != nullptr)
        {
            parent = next->parent();
        }
        if (parent != nullptr)
        {
            id.collection = parent->text(0);
            id.name = next->text(0);
        }
        else if (next != nullptr)
        {
            id.collection = next->text(0);
        }
    }
    return id;
}
            
    