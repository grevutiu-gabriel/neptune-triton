// 
// File:   ComplexParameterContainer.h
// Author: M.R.Eggleston
//
// Created on 18 March 2014, 08:33
//

#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QListWidget>
#include "CriticalPointFinder.h"
#include "FractalsConfig.h"
#include "NumberEntry.h"
#include "TypesAndConstants.h"

class CriticalPointsContainer : public QWidget
{
    Q_OBJECT

signals:
    void changed();
    
private slots:
    void on_row_changed(int row);
    void on_ignore_check_changed(bool);

public:
    CriticalPointsContainer(QWidget* parent = nullptr);
    virtual ~CriticalPointsContainer();

    void load(FRACTAL_TYPE ft);

private:
    CriticalPointsContainer(const CriticalPointsContainer &other);

    void setup_ui_layout();
    void connect_signals();

    void displayParameterInList(int row);
    void updateInputFields();
    void displayValueAtCursor();

    CriticalPointFinder<LongComplex> m_cp_finder;
    FractalsConfig m_config;
    QListWidget* m_list;
    QCheckBox* m_ignore_check;
    QWidget* m_main_widget;
    bool m_emit;
    int m_row;
    bool m_load_in_progress;
};

