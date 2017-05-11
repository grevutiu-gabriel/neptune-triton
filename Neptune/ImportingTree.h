// 
// File:   ImportingTree.h
// Author: M.R.Eggleston
//
// Created on 23 February 2012, 15:00
//

#pragma once

#include <QTreeWidget>

struct map_identity
{
    QString collection;
    QString name;
};

struct map_full_identity
{
    QString collection;
    QString import_name;
    QString name;
};


class ImportingTree : public  QTreeWidget
{
public:
    ImportingTree(QWidget* parent = nullptr);
    virtual ~ImportingTree();

    void addMap(const QString& import_name, const QString& collection_name, const QString& name);
    std::vector<map_full_identity>& getMaps();
    map_identity colourMapAtCursor();
    bool mapExists(const QString& collection, const QString& name);
    bool importingMapExists(const QString& name);
    bool hasCursor();
    void setCursorAtMap(const QString& name);
    void setCursorAtMap(const QString& collection, const QString& name);
    void deleteCollection(const QString& collection);
    void deleteImportingMap(const QString& collection, const QString &name);
    
private:
    ImportingTree(const ImportingTree& orig);

    void on_remove_menu_item();
    void on_remove_all_menu_item();
    void on_edited(const QString &path, const QString &new_text);

    std::vector<map_full_identity> m_map_identities;
};

