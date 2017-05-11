//
// File:   ColourTab.h
// Author: M.R.Eggleston
//
// Created on 05 July 2010, 17:28
//

#pragma once

#ifdef NOT_MOC
#include <boost/scoped_ptr.hpp>
#endif
#include <QLineEdit>
#include "AutoColourEdit.h"
#include "ColourMaps.h"
#include "ComponentColourEdit.h"
#include "OptionComboboxes.h"
#include "ManualColourEdit.h"

class ColourTab : public QWidget
{
    Q_OBJECT
signals:
    void new_map();
    void map(const QString& map_name);

private slots:
    void on_edit_type(int index);
    void on_new_button(bool clicked);
    void on_delete_button(bool clicked);
    void on_revert_button(bool clicked);
    void on_save_button(bool clicked);
    void on_entry_changed(const QString& text);
    void on_map_changed();

public:
    ColourTab(QWidget* parent = nullptr);
    virtual ~ColourTab();
    
    void display();
    void setColourMap(const QString &map_name);
    const QString& currentColourMap();

private:
    ColourTab(const ColourTab& orig);
    
    void setup_ui_layout();
    void connect_signals();

    void displayButtons();
    void setupColourMap();
    
    ColourMapEditComboBox* m_edit_combo;

    // "Action" buttons
    QPushButton* m_new_button;
    QPushButton* m_delete_button;
    QPushButton* m_revert_button;
    QPushButton* m_save_button;

    QLineEdit* m_name_line;
    AutoColourEdit* m_auto;
    ManualColourEdit* m_manual1;
    ComponentColourEdit* m_manual2;
    boost::shared_ptr<ColourMaps> m_colour_maps;
    QString m_map_name;
    QString m_old_name;
    ColourMap m_map;
    bool m_changed;
    bool m_ignore_signals;
};

