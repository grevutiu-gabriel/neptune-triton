// 
// File:   ImportColourMap.h
// Author: M.R.Eggleston
//
// Created on 21 February 2012, 10:56
//
// Converted to Qt April 2013

#pragma once

#include <QDomElement>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QTreeWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QTextEdit>
#include <QToolButton>
#include "ColourMap.h"
#include "ColourMaps.h"
#include "ImportingTree.h"
#include "ImportSourceTree.h"

typedef std::map<QString, Map> MapMap;

class ImportColourMap : public QTabWidget
{
    Q_OBJECT
private slots:
    void on_browse_button(bool);
    void on_move_button(bool);
    void on_import_button(bool);
    void on_source_cursor(QTreeWidgetItem*, QTreeWidgetItem*);
    void on_import_cursor(QTreeWidgetItem*, QTreeWidgetItem*);
    void on_source_remove();
    void on_source_remove_all();
    void on_import_remove();
    void on_import_remove_all();
    void on_source_context_menu(const QPoint& pos);
    void on_import_context_menu(const QPoint& pos);
 
public:
    ImportColourMap(QWidget* parent = nullptr);
    virtual ~ImportColourMap();
    
private:
    ImportColourMap(const ImportColourMap& orig);
    
    void setup_ui_layout();
    void connect_signals();
    
    void load(QStringList& names);
    void loadSourceTree();
    void importMapFile(const QString& name);
    void importUgrFile(const QString& name);
    void importScmFile(const QString& name);
    void addImportMap(const QString& name, Map& maps, ColourMap& colour_map);
    void deleteFromImportMap(const QString& collection, const QString& name);

    void mapExistsMessage(const QString& name, const QString& found_name);
    void mapImportError(const QString& name);
    void mapFileMessage(const QString& name);
    void badScmFileMessage(const QString& name);
    void exceptionMessage(const QString& name, std::exception& ex);
    void mapAlreadyLoadedMessage(const QString& name);
    void startMessages();
    void writeMessages();
    
    void load_colour_map_child(const QDomElement& root, ColourMap &map, QString& name);
    void load_auto_colour(const QDomElement& child, RGB colour, ColourMap &map);
    
    void drawMap();
    QString assignTemporaryName(const QString &base);

    QPushButton* m_browse_button;
    QToolButton* m_move_button;
    QPushButton* m_import_button;
    ImportingTree* m_importing_tree;
    ImportSourceTree* m_source_tree;
    QTextEdit* m_messages_text;
    
    QImage m_colour_map_image;
    QLabel* m_colour_map_drawing;

    QMenu* m_source_context_menu;
    QMenu* m_import_context_menu;
    
    QString m_messages;
    
    MapMap m_import_maps;
    boost::shared_ptr<ColourMaps> m_colour_maps;
};

