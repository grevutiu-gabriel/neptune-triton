// 
// File:   ExportColourMap.h
// Author: M.R.Eggleston
//
// Created on 21 February 2012, 11:03
//
// Converted to Qt April 2013
//

#pragma once

#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>
#include "ColourMapList.h"
#include "ColourMaps.h"
#include "OptionComboboxes.h"

class ExportColourMap : public QWidget
{
    Q_OBJECT
private slots:
    void on_export_as_button(bool);
    void on_move_button(bool);
    void on_colour_map_cursor_changed(int);
    void on_exporting_cursor_changed(int);
    void remove();
    void remove_all();
    void on_context_menu(const QPoint& pos);
    
public:
    ExportColourMap(QWidget* parent = nullptr);
    virtual ~ExportColourMap();
    
private:
    ExportColourMap(const ExportColourMap& orig);

    void setup_ui_layout();
    void connect_signals();

    void saveScm(const QString& name);
    void saveUgr(const QString& name);
    
    QImage m_colour_map_image;
    QLabel* m_colour_map_drawing;

    QListWidget* m_exporting_list;
    QPushButton* m_export_as_button;
    QToolButton* m_move_button;
    ExportCombobox* m_export_combo;
    ColourMapList* m_colour_map_list;
    
    QMenu* m_context_menu;

    boost::shared_ptr<ColourMaps> m_colour_maps;
};

