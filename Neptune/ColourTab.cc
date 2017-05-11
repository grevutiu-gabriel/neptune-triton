//
// File:   ColourTab.cc
// Author: M.R.Eggleston
//
// Created on 05 July 2010, 17:28
//

//
// This object should be instantiated after ColourMaps::Load has been called.
//

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <QHBoxLayout>
#include "ColourTab.h"
#include "ColourMaps.h"
//#include "FractalSettingsWindow.h"

ColourTab::ColourTab(QWidget* parent)
:
QWidget(parent),
m_edit_combo(new ColourMapEditComboBox()),
m_new_button(new QPushButton("New")),
m_delete_button(new QPushButton("Delete")),
m_revert_button(new QPushButton("Revert")),
m_save_button(new QPushButton("Save")),
m_name_line(new QLineEdit()),
m_auto(new AutoColourEdit()),
m_manual1(new ManualColourEdit()),
m_manual2(new ComponentColourEdit()),
m_colour_maps(ColourMaps::instance()),
m_map_name("Default"),
m_changed(false),
m_ignore_signals(false)
{
    m_auto->hide();
    m_manual2->hide();
    setup_ui_layout();
    connect_signals();
}

ColourTab::~ColourTab()
{
}

void ColourTab::setup_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    auto hlayout = new QHBoxLayout();
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(m_edit_combo);
    hlayout->addWidget(new QLabel("Map name"));
    hlayout->addWidget(m_name_line);
    main_layout->addLayout(hlayout);
    main_layout->addWidget(m_auto);
    main_layout->addWidget(m_manual1);
    main_layout->addWidget(m_manual2);
    hlayout = new QHBoxLayout();
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addStretch();
    hlayout->addWidget(m_new_button);
    hlayout->addWidget(m_delete_button);
    hlayout->addWidget(m_revert_button);
    hlayout->addWidget(m_save_button);
    main_layout->addLayout(hlayout);
    
    setLayout(main_layout);
}

void ColourTab::connect_signals()
{
    connect(m_edit_combo, SIGNAL(currentIndexChanged(int)),
            this,         SLOT(on_edit_type(int)));
    connect(m_new_button, SIGNAL(clicked(bool)),
            this,         SLOT(on_new_button(bool)));
    connect(m_delete_button, SIGNAL(clicked(bool)),
            this,         SLOT(on_delete_button(bool)));
    connect(m_revert_button, SIGNAL(clicked(bool)),
            this,         SLOT(on_revert_button(bool)));
    connect(m_save_button, SIGNAL(clicked(bool)),
            this,         SLOT(on_save_button(bool)));
    connect(m_name_line,  SIGNAL(textChanged(const QString&)),
            this,         SLOT(on_entry_changed(const QString&)));
    connect(m_auto,       SIGNAL(changed()),
            this,         SLOT(on_map_changed()));
}

void ColourTab::on_edit_type(int index)
{
    if (!m_ignore_signals)
    {
        m_map.setType(static_cast<ColourMap::MAP_TYPE>(index));
        display();
    }
}

void ColourTab::on_new_button(bool)
{
    m_map_name.clear();
    m_map.clear();
    display();
    emit new_map();
}

void ColourTab::on_delete_button(bool)
{
    m_colour_maps->RemoveMap(m_map_name);
}

void ColourTab::on_revert_button(bool)
{
    m_map_name = m_old_name;
    m_map = m_colour_maps->getColourMap(m_map_name);
    display();
    emit map(m_map_name);
}

void ColourTab::on_save_button(bool)
{
    bool rename = false;
    auto type = m_map.getType();
    if (type == ColourMap::CM_AUTO)
    {
        m_map = m_auto->getColourMap();
    }
    else if (type == ColourMap::CM_MANUAL)
    {
        m_map = m_manual1->getColourMap();
    }
    else
    {
        m_map = m_manual2->getColourMap();
    }
    if (m_old_name != m_map_name)
    {
        ColourMap map = m_colour_maps->getColourMap(m_old_name);
        rename = map.equal(m_map);
    }
    if (rename)
    {
        m_colour_maps->updateMaps(m_old_name, m_map_name, m_map, false);
    }
    else
    {
        m_colour_maps->updateMaps(m_map_name, m_map_name, m_map, false);
    }
    m_old_name = m_map_name;
//    display();
    emit map(m_map_name);
}

void ColourTab::on_entry_changed(const QString& str)
{
    m_map_name = str.trimmed();
    m_name_line->setText(m_map_name);
    displayButtons();
}

void ColourTab::on_map_changed()
{
    m_changed = true;
    displayButtons();
}

void ColourTab::display()
{
    m_ignore_signals = true;
    auto type = m_map.getType();
    if (type == ColourMap::CM_AUTO)
    {
        m_auto->setColourMap(m_map);
        m_auto->show();
        m_manual1->hide();
        m_manual2->hide();
        m_edit_combo->setValue(CM_AUTO);
    }
    else if (type == ColourMap::CM_MANUAL)
    {
        m_manual1->setColourMap(m_map);
        m_manual1->show();
        m_manual2->hide();
        m_auto->hide();
        m_edit_combo->setValue(CM_MANUAL_1);
    }
    else
    {
        m_manual1->hide();
        m_manual2->setColourMap(m_map);
        m_manual2->show();
        m_auto->hide();
        m_edit_combo->setValue(CM_MANUAL_2);
    }
    m_name_line->setText(m_map_name);
    displayButtons();
    m_ignore_signals = false;
}

void ColourTab::setColourMap(const QString &map_name)
{
    m_map_name = map_name;
    m_old_name = map_name;
    m_map = m_colour_maps->getColourMap(m_map_name);
    display();
}

void ColourTab::displayButtons()
{
    bool exists = m_colour_maps->nameExists(m_map_name);
    bool is_default = m_colour_maps->isDefault(m_map_name);
    bool temporary = (m_map_name.indexOf("~") != -1);
    bool valid_name = (!exists && !temporary && !m_map_name.isEmpty());
    bool can_save = (valid_name && !is_default);
    m_save_button->setEnabled(can_save);
    m_delete_button->setEnabled(exists && !is_default);
    m_new_button->setEnabled(exists);
}

const QString& ColourTab::currentColourMap()
{
    return m_map_name;
}
