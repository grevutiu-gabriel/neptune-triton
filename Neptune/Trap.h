// 
// File:   Trap.h
// Author: M.R.Eggleston
//
// Created on 08 April 2013, 14:26
//
// Taken from ColouringTabContainers .h and converted to Qt

#pragma once

#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include "Config.h"
#include "FractalsConfig.h"
#include "NumberEntry.h"
#include "OptionComboboxes.h"
#include "TrapConfig.h"

class Trap : public QWidget
{
    Q_OBJECT
signals:
    void trap_changed();

private slots:
    void on_trap_changed(int index);
    void on_trap_size_changed();
    void on_trap_position_changed();
    void on_trap_rotation_changed(int value);
    void on_centre_point_changed();
    void on_image_centre_button(bool);
    void on_trap_scale_button(bool);
    void on_inversion_distance_changed();
    void on_chain_length_changed(int value);
    void on_outer_check_changed(int state);
    void on_inner_check_changed(int state);
    void on_centre_points_check_changed(int state);
    void on_triangles_check_changed(int state);
    void on_trap_centre_point_changed(int state);

public:
    Trap(bool inner, QWidget* parent = nullptr);
    virtual ~Trap();

    void SetFractalType(FRACTAL_TYPE ft);
    void display();

    std::string asString();

private:
    Trap(Trap& other);
    
    void setup_ui_layout();
    void connect_signals();

    std::string getTrapMethod(char type);
    void updateTrapMethod();

    bool m_inner;
    FractalsConfig m_config;
    OrbitTrapTypeCombobox *m_trap_combo;
    NumberEntry* m_trap_size_entry;
    NumberEntry* m_trap_position_r_entry;
    NumberEntry* m_trap_position_i_entry;
    QCheckBox* m_trap_change_in_distance_check; // => remove add new type to colour method
    QSpinBox* m_trap_rotation_spin;
    QCheckBox* m_trap_centre_point_check;
    QPushButton* m_trap_image_centre_button;
    QPushButton* m_trap_scale_button;
    QWidget *m_steiner_chain_widget;
    NumberEntry* m_invd_entry;
    QSpinBox* m_chain_length_spin;
    QCheckBox* m_inner_circle_check;
    QCheckBox* m_outer_circle_check;
    QCheckBox* m_centres_check;
    QCheckBox* m_triangles_check;

    std::string m_method;
    TrapConfig m_trap_config;
    bool m_ignore_signals;
};