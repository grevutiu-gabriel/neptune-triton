//
// File:   ImageTab.h
// Author: M.R.Eggleston
//
// Created on 05 October 2010, 17:23
// Renamed from PositionTab.h 20th February 2012
//

#pragma once

#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QTextEdit>
#include <QWidget>
#include "mpreal.h"
#include "Config.h"
#include "FractalsConfig.h"
#include "ImageSize.h"
#include "NumberEntry.h"

class ImageTab : public QWidget
{
    Q_OBJECT
signals:
    void complex_plane_changed();

private slots:
    void on_scp_reset_button(bool);
    void on_use_ccp_button(bool);
    void on_new_centre();
    void on_new_width();
    void on_new_rotation();
    void on_precision_changed(int value);
    void on_override_check(int state);
    void on_precision_set(bool clicked);

public:
    ImageTab(QWidget *parent = nullptr);
    virtual ~ImageTab();

    void setFractalType(FRACTAL_TYPE ft);
    void display();
    
    ImageSize* image_size();

private:
    ImageTab(const ImageTab& orig);
    
    void setup_ui_layout();
    void connect_signals();
    void display_current_location();
    void display_start_location();

    QPushButton* m_scp_reset_button;
    QPushButton* m_use_ccp_button;
    NumberEntry* m_ccentre_r_entry;
    NumberEntry* m_ccentre_i_entry;
    NumberEntry* m_width_entry;
    QTextEdit* m_start_location_text;
    QTextEdit* m_current_location_text;
    QSpinBox* m_crotation_spin;
    ImageSize* m_image_size;
    QLabel* m_chorizontal_label;
    QLabel* m_cvertical_label;
    QCheckBox* m_precision_override_check;
    QSpinBox* m_precision_spin;
    QWidget* m_override_widget;
    QPushButton* m_override_button;
    FractalsConfig m_config;
    bool m_ignore_signals;
};


