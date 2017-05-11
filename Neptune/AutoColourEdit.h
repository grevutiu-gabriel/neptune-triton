//
// File:   AutoColourContainer.h
// Author: M.R.Eggleston
// Created on 20 July 2010, 14:55
//

#pragma once

#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include "ColourEdit.h"
#include "ColourMaps.h"
#include "OptionComboboxes.h"
#include "WaveDrawing.h"

class AutoColourEdit : public ColourEdit
{
    Q_OBJECT
signals:
    void changed();

private slots:
    void on_red_hoffset(int value);
    void on_red_voffset(int value);
    void on_red_maximum(int value);
    void on_red_wave(int index);
    void on_red_cycles(int index);
    void on_red_reset(bool clicked);
    void on_green_hoffset(int value);
    void on_green_voffset(int value);
    void on_green_maximum(int value);
    void on_green_wave(int index);
    void on_green_cycles(int index);
    void on_green_reset(bool clicked);
    void on_blue_hoffset(int value);
    void on_blue_voffset(int value);
    void on_blue_maximum(int value);
    void on_blue_wave(int index);
    void on_blue_cycles(int index);
    void on_blue_reset(bool clicked);

public:
    AutoColourEdit(QWidget* parent = nullptr);
    virtual ~AutoColourEdit();

    void setColourMap(const ColourMap &map);
    const ColourMap &getColourMap();

private:
    AutoColourEdit(const AutoColourEdit& orig);
    
    void setup_ui_layout();
    void connect_signals();

    void display();
    void drawWave(QPainter& painter, RGB colour);
    void redraw();

    WaveDrawing* m_wave_drawing;
    QSpinBox* m_hred_spin;
    QSpinBox* m_vred_spin;
    QSpinBox* m_mred_spin;
    WaveComboBox* m_red_wave_combo;
    QSpinBox* m_cred_spin;
    QPushButton* m_red_reset_button;
    
    QSpinBox* m_hgreen_spin;
    QSpinBox* m_vgreen_spin;
    QSpinBox* m_mgreen_spin;
    WaveComboBox* m_green_wave_combo;
    QSpinBox* m_cgreen_spin;
    QPushButton* m_green_reset_button;
    
    QSpinBox* m_hblue_spin;
    QSpinBox* m_vblue_spin;
    QSpinBox* m_mblue_spin;
    WaveComboBox* m_blue_wave_combo;
    QSpinBox* m_cblue_spin;
    QPushButton* m_blue_reset_button;

    Waveform::AutoColour m_auto_colour[3];
    //
    bool m_valid_name;
    boost::shared_ptr<ColourMaps> m_colour_maps;
    bool m_ignore_signals;
};

