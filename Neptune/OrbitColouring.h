// 
// File:   OrbitColouring.h
// Author: M.R.Eggleston
//
// Created on 03 April 2013, 13:29
//

#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include "FractalsConfig.h"
#include "NumberEntry.h"
#include "OptionComboboxes.h"
#include "SingleColour.h"

class OrbitColouring : public QWidget
{
    Q_OBJECT
signals:
    void orbit_colouring_changed();

private slots:
    void on_method_changed(int index);
    void on_interval_changed(int value);
    void on_red_offset_changed(int value);
    void on_green_offset_changed(int value);
    void on_blue_offset_changed(int value);
    void on_lockstep_changed(int state);
    void on_scale_changed();
    void on_component_colour_order_changed(int index);
    void on_background_enabled(int state);
    void on_background_colour_changed(const QColor& color);

public:
    OrbitColouring(QWidget* parent = nullptr);
    virtual ~OrbitColouring();
    
    void setFractalType(FRACTAL_TYPE ft);
    void display();
    
private:
    OrbitColouring(const OrbitColouring&);
    
    void setup_ui_layout();
    void connect_signals();
    
    void setOffset();
    
    FractalsConfig m_config;
    OrbitColourCombobox* m_method_combo;
    QSpinBox* m_red_offset_spin;
    QSpinBox* m_green_offset_spin;
    QSpinBox* m_blue_offset_spin;
    QCheckBox* m_lockstep_check;
    QCheckBox* m_smooth_check;
    QWidget* m_scale_widget;
    QWidget* m_interval_widget;
    NumberEntry* m_scale_entry;
    QSpinBox* m_interval_spin;
    QLabel* m_info_label;
    ComponentOrderComboBox* m_component_order_combo;
    QWidget* m_component_order_widget;
    SingleColour* m_background;
    QCheckBox* m_use_colour_map_check;
    boost::shared_ptr<ColourMaps> m_colour_maps;
    bool m_ignore_signals;
    bool m_lockstep;
};
