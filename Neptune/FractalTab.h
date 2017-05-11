//
// File:   FractalTab.h
// Author: M.R.Eggleston
//
// Created on 29th January 2014, 14:06
//

#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include "BailoutCondition.h"
#include "ComplexParametersContainer.h"
#include "CriticalPointsContainer.h"
#include "FractalsConfig.h"
#include "FractalList.h"
#include "NumberEntry.h"
#include "OptionComboboxes.h"

class FractalTab : public QWidget
{
    Q_OBJECT
signals:
    void config_changed();
    void settling_cycles_changed(int cycles);
    
private slots:
    void on_changed();
    void on_orbit_check(int state);
    void on_orbit_plot_changed(int value);
    void on_area_changed(int value);
    void on_omit_changed(int value);
    void on_bailout_defaults();
    void on_density_changed(int value);

public:
    FractalTab(QWidget *parent = nullptr);
    virtual ~FractalTab();

    void setFractalType(FRACTAL_TYPE ft);
    void startOrbitIterationRangeChanged(int value);
    void display();

private:
    FractalTab(const FractalTab& orig);

    void setup_ui_layout();
    void setup_bailout_layout();
    void connect_signals();

    void display_bailout_fractal();

    QWidget* m_bailout_widget;
    QWidget* m_lyapunov_widget;
    QPushButton *m_parameter_defaults_button;
    QTextEdit* m_formula_text;
    ComplexParametersContainer *m_parameters_container;
    CriticalPointsContainer *m_critical_points_container;
    QCheckBox* m_orbit_check;
    QSpinBox* m_calculating_area_spin;
    OrbitPlotCombobox* m_orbit_plot_combo;
    QSpinBox* m_omit_spin;
    QSpinBox* m_density_spin;
    NumberEntry* m_settling_cycles_entry;
    QLineEdit* m_sequence_line;
    BailoutCondition *m_bailout_condition;
    FractalsConfig m_config;
    FRACTAL_TYPE m_fractal_type;
    bool m_emit_colour_change;
    bool m_enable_fractal_settings_changed_signal;
};
