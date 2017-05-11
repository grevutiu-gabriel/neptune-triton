//
// File:   BailoutCondition.h
// Author: M.R.Eggleston
//
// Created on 27 June 2011, 12:51
//

#pragma once

#include <QLabel>
//#include "mpreal.h"
#include "FractalsConfig.h"
#include "NumberEntry.h"
#include "OptionComboboxes.h"

class BailoutCondition : public QWidget
{
    Q_OBJECT
signals:
    void changed();

private slots:
    void on_limit_changed();
    void on_condition_changed(int);
    
public:
    BailoutCondition(QWidget* parent = nullptr);
    ~BailoutCondition();

    void load(FRACTAL_TYPE ft);

private:
    BailoutCondition(const BailoutCondition &other);
    
    void setup_ui_layout();
    void connect_signals();
    
    FractalsConfig m_config;
    NumberEntry* m_limit_entry;
    DivergeBailoutConditionCombobox *m_diverge_combo;
    QLabel* m_gt_label;
    bool m_emit;
};

