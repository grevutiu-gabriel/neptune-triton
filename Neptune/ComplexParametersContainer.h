// 
// File:   ComplexParameterContainer.h
// Author: M.R.Eggleston
//
// Created on 24 April 2012, 16:16
//

#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QListWidget>
#include "FractalsConfig.h"
#include "NumberEntry.h"
#include "TypesAndConstants.h"

class ComplexParametersContainer : public QWidget
{
    Q_OBJECT

signals:
    void changed();
    
private slots:
    void on_row_changed(int row);
    void on_parameter_changed();
    void on_parameter_state_changed(int);

public:
    ComplexParametersContainer(QWidget* parent = nullptr);
    virtual ~ComplexParametersContainer();

    void load(FRACTAL_TYPE ft);

private:
    ComplexParametersContainer(const ComplexParametersContainer &other);

    void setup_ui_layout();
    void connect_signals();

    void displayParameterInList(int row);
    void displayValueAtCursor();
    bool updateParameter();

    FractalsConfig m_config;
    QListWidget* m_list;
    NumberEntry* m_real_entry;
    NumberEntry* m_imag_entry;
    QWidget* m_main_widget;
    QLabel* m_no_complex_parameters_label;
    bool m_emit;
    int m_row;
    bool m_load_in_progress;
};

