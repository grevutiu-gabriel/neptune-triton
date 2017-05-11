// 
// File:   BailoutColouring.h
// Author: trevone
//
// Created on 04 April 2013, 11:10
//

#pragma once

#include <QCheckBox>
#include <QSpinBox>
#include "NumberEntry.h"
#include "OptionComboboxes.h"
#include "SingleColour.h"
#include "Trap.h"

class BailoutColouring : public QWidget
{
    Q_OBJECT
signals:
    void colouring_changed(const QString map_name);

private slots:
    void on_method_changed(int);
    void on_smooth_colouring_changed(int state);
    void on_abs_log_changed(int state);
    void on_red_offset_changed(int value);
    void on_green_offset_changed(int value);
    void on_blue_offset_changed(int value);
    void on_lockstep_changed(int state);
    void on_scale_changed();
    void on_statistics_option_changed(int);
    void on_final_magnitude_changed(int);
    void on_quotient_changed(int);
    void on_divisor_changed(int);
    void on_gaussian_type_changed(int);
    void on_gaussian_method_changed(int);
    void on_trap_changed();
    void on_component_colour_order_changed(int index);
    
public:
    BailoutColouring(bool inner, QWidget* parent = nullptr);
    virtual ~BailoutColouring();

    void setFractalType(FRACTAL_TYPE ft);
    void display();
    const SingleColour* single_colour();
    
private:
    BailoutColouring();
    BailoutColouring(const BailoutColouring&);
    
    void setup_ui_layout();
    void connect_signals();

    METHOD colouringMethod();
    void setScale();
    void setGaussian();
    void setMagnitudeType();
    void setOffset();
    void setRatio();
    void setStatisticsOption(int pos);
    void setTrap();
    void setColour(const std::string& value);

    FractalsConfig m_config;
    MethodCombobox* m_method_combo;
    StatisticsOptionTypeCombobox* m_statistics_options_combo;
    QWidget* m_statistics_widget;
    FinalMagnitudeTypeCombobox* m_final_magnitude_combo;
    QWidget* m_final_magnitude_widget;
    RatioPartTypeCombobox* m_quotient_combo;
    RatioPartTypeCombobox* m_divisor_combo;
    QWidget* m_ratio_widget;
    GaussianTypeCombobox *m_gaussian_type_combo;
    GaussianMethodCombobox *m_gaussian_method_combo;
    QWidget* m_gaussian_widget;
    SingleColour* m_single_colour;
    QWidget* m_multi_colour;
    QSpinBox* m_red_offset_spin;
    QSpinBox* m_green_offset_spin;
    QSpinBox* m_blue_offset_spin;
    QCheckBox* m_lockstep_check;
    QCheckBox* m_smooth_check;
    QCheckBox* m_abs_log_check;
    QWidget* m_scale_widget;
    NumberEntry* m_scale_entry;
    Trap *m_trap;
    ComponentOrderComboBox* m_component_order_combo;
    QWidget* m_component_order_widget;

    boost::shared_ptr<ColourMaps> m_colour_maps;
    bool m_ignore_signals;
    bool m_inner;
    bool m_lockstep;
    bool m_map_cursor;
};