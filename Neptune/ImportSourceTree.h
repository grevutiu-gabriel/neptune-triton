// 
// File:   ImportSourceTree.h
// Author: M.R.Eggleston
//
// Created on 23 February 2012, 16:11
//

#pragma once

#include <QTreeWidget>
#include "ImportingTree.h"


class ImportSourceTree : public QTreeWidget
{
    Q_OBJECT
public:
    ImportSourceTree(QWidget* parent = nullptr);
    virtual ~ImportSourceTree();
    
    void addMap(const QString& collection, const QString& name);
    QStringList& getMapsAtCursor(QString& collection);
    void removeMap(const QString& collection, const QString& name);
    void removeCollection(const QString& collection);
    void setCursorAtMap(const QString& collection, const QString& name);
    map_identity colourMapAtCursor();
    map_identity getMapAtNext();
    void deleteBranchAtCursor();
    void deleteItemAtCursor();
    
private:
    ImportSourceTree(const ImportSourceTree& orig);
    
    QStringList m_map_names;
};

