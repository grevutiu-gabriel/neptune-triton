//
// File:   AutoColourEdit.cc
// Author: M.R.Eggleston
//
// Created on 20 July 2010, 14:55
//

#include <iostream>
#include <QGridLayout>
#include <QPainter>
#include <QVBoxLayout>
#include "AutoColourEdit.h"

AutoColourEdit::AutoColourEdit(QWidget* parent)
:
ColourEdit(parent),
m_wave_drawing(new WaveDrawing()),
m_hred_spin(new QSpinBox()),
m_vred_spin(new QSpinBox()),
m_mred_spin(new QSpinBox()),
m_red_wave_combo(new WaveComboBox()),
m_cred_spin(new QSpinBox()),
m_red_reset_button(new QPushButton("Reset")),
m_hgreen_spin(new QSpinBox()),
m_vgreen_spin(new QSpinBox()),
m_mgreen_spin(new QSpinBox()),
m_green_wave_combo(new WaveComboBox()),
m_cgreen_spin(new QSpinBox()),
m_green_reset_button(new QPushButton("Reset")),
m_hblue_spin(new QSpinBox()),
m_vblue_spin(new QSpinBox()),
m_mblue_spin(new QSpinBox()),
m_blue_wave_combo(new WaveComboBox()),
m_cblue_spin(new QSpinBox()),
m_blue_reset_button(new QPushButton("Reset")),
m_valid_name(false),
m_colour_maps(ColourMaps::instance()),
m_ignore_signals(false)
{
    setup_ui_layout();
    
    m_hred_spin->setRange(0, 511);
    m_hred_spin->setWrapping(true);
    m_vred_spin->setRange(-255, 255);
    m_vred_spin->setWrapping(true);
    m_mred_spin->setRange(0, 255);
    m_mred_spin->setWrapping(true);
    m_red_wave_combo->setCurrentIndex(static_cast<int>(Waveform::WAVE_SINE));
    m_cred_spin->setRange(1, 8);
    
    m_hgreen_spin->setRange(0, 511);
    m_hgreen_spin->setWrapping(true);
    m_vgreen_spin->setRange(-255, 255);
    m_vgreen_spin->setWrapping(true);
    m_mgreen_spin->setRange(0, 255);
    m_mgreen_spin->setWrapping(true);
    m_green_wave_combo->setCurrentIndex(static_cast<int>(Waveform::WAVE_SINE));
    m_cgreen_spin->setRange(1, 8);
    
    m_hblue_spin->setRange(0, 511);
    m_hblue_spin->setWrapping(true);
    m_vblue_spin->setRange(-255, 255);
    m_vblue_spin->setWrapping(true);
    m_mblue_spin->setRange(0, 255);
    m_mblue_spin->setWrapping(true);
    m_blue_wave_combo->setCurrentIndex(static_cast<int>(Waveform::WAVE_SINE));
    m_cblue_spin->setRange(1, 8);
    
    connect_signals();
}

AutoColourEdit::~AutoColourEdit()
{
}

void AutoColourEdit::setup_ui_layout()
{
    auto main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(m_wave_drawing);
    auto grid = new QGridLayout();
    grid->setSpacing(0);
    grid->setContentsMargins(5, 0, 0, 0);
    grid->addWidget(new QLabel(" "), 0, 0);
    grid->addWidget(new QLabel("Horizontal "), 0, 1);
    grid->addWidget(new QLabel("Vertical "), 0, 2);
    grid->addWidget(new QLabel("Maximum "), 0, 3);
    grid->addWidget(new QLabel("Cycles "), 0, 5);
    grid->addWidget(new QLabel("Red"), 1, 0);
    grid->addWidget(m_hred_spin, 1, 1);
    grid->addWidget(m_vred_spin, 1, 2);
    grid->addWidget(m_mred_spin, 1, 3);
    grid->addWidget(m_red_wave_combo, 1, 4);
    grid->addWidget(m_cred_spin, 1, 5);
    grid->addWidget(m_red_reset_button, 1, 6);
    grid->addWidget(new QLabel("Green"), 2, 0);
    grid->addWidget(m_hgreen_spin, 2, 1);
    grid->addWidget(m_vgreen_spin, 2, 2);
    grid->addWidget(m_mgreen_spin, 2, 3);
    grid->addWidget(m_green_wave_combo, 2, 4);
    grid->addWidget(m_cgreen_spin, 2, 5);
    grid->addWidget(m_green_reset_button, 2, 6);
    grid->addWidget(new QLabel("Blue"), 3, 0);
    grid->addWidget(m_hblue_spin, 3, 1);
    grid->addWidget(m_vblue_spin, 3, 2);
    grid->addWidget(m_mblue_spin, 3, 3);
    grid->addWidget(m_blue_wave_combo, 3, 4);
    grid->addWidget(m_cblue_spin, 3, 5);
    grid->addWidget(m_blue_reset_button, 3, 6);
    main_layout->addLayout(grid);
    main_layout->addStretch();
    main_layout->addWidget(m_red_label);
    main_layout->addWidget(m_green_label);
    main_layout->addWidget(m_blue_label);
    main_layout->addWidget(m_colour_label);
    
    setLayout(main_layout);
}

void AutoColourEdit::connect_signals()
{
    connect(m_hred_spin, SIGNAL(valueChanged(int)),
            this,        SLOT(on_red_hoffset(int)));
    connect(m_vred_spin, SIGNAL(valueChanged(int)),
            this,        SLOT(on_red_voffset(int)));
    connect(m_mred_spin, SIGNAL(valueChanged(int)),
            this,        SLOT(on_red_maximum(int)));
    connect(m_red_wave_combo, SIGNAL(currentIndexChanged(int)),
            this,        SLOT(on_red_wave(int)));
    connect(m_cred_spin, SIGNAL(valueChanged(int)),
            this,        SLOT(on_red_cycles(int)));
    connect(m_red_reset_button, SIGNAL(clicked(bool)),
            this,        SLOT(on_red_reset(bool)));
    connect(m_hgreen_spin, SIGNAL(valueChanged(int)),
            this,        SLOT(on_green_hoffset(int)));
    connect(m_vgreen_spin, SIGNAL(valueChanged(int)),
            this,        SLOT(on_green_voffset(int)));
    connect(m_mgreen_spin, SIGNAL(valueChanged(int)),
            this,        SLOT(on_green_maximum(int)));
    connect(m_green_wave_combo, SIGNAL(currentIndexChanged(int)),
            this,        SLOT(on_green_wave(int)));
    connect(m_cgreen_spin, SIGNAL(valueChanged(int)),
            this,        SLOT(on_green_cycles(int)));
    connect(m_green_reset_button, SIGNAL(clicked(bool)),
            this,        SLOT(on_green_reset(bool)));
    connect(m_hblue_spin, SIGNAL(valueChanged(int)),
            this,        SLOT(on_blue_hoffset(int)));
    connect(m_vblue_spin, SIGNAL(valueChanged(int)),
            this,        SLOT(on_blue_voffset(int)));
    connect(m_mblue_spin, SIGNAL(valueChanged(int)),
            this,        SLOT(on_blue_maximum(int)));
    connect(m_blue_wave_combo, SIGNAL(currentIndexChanged(int)),
            this,        SLOT(on_blue_wave(int)));
    connect(m_cblue_spin, SIGNAL(valueChanged(int)),
            this,        SLOT(on_blue_cycles(int)));
    connect(m_blue_reset_button, SIGNAL(clicked(bool)),
            this,        SLOT(on_blue_reset(bool)));
}

void AutoColourEdit::display()
{
    m_ignore_signals = true;
    m_hred_spin->setValue(m_auto_colour[0].hoffset);
    m_vred_spin->setValue(m_auto_colour[0].voffset);
    m_mred_spin->setValue(m_auto_colour[0].maximum);
    m_red_wave_combo->setCurrentIndex(static_cast<int>(m_auto_colour[0].wave));
    m_cred_spin->setValue(m_auto_colour[0].cycles);
    
    m_hgreen_spin->setValue(m_auto_colour[1].hoffset);
    m_vgreen_spin->setValue(m_auto_colour[1].voffset);
    m_mgreen_spin->setValue(m_auto_colour[1].maximum);
    m_green_wave_combo->setCurrentIndex(static_cast<int>(m_auto_colour[1].wave));
    m_cgreen_spin->setValue(m_auto_colour[1].cycles);
    
    m_hblue_spin->setValue(m_auto_colour[2].hoffset);
    m_vblue_spin->setValue(m_auto_colour[2].voffset);
    m_mblue_spin->setValue(m_auto_colour[2].maximum);
    m_blue_wave_combo->setCurrentIndex(static_cast<int>(m_auto_colour[2].wave));
    m_cblue_spin->setValue(m_auto_colour[2].cycles);
    
    m_map.setAutoColour(RGB_RED, m_auto_colour[0]);
    m_map.setAutoColour(RGB_GREEN, m_auto_colour[1]);
    m_map.setAutoColour(RGB_BLUE, m_auto_colour[2]);
    draw_maps();
    m_wave_drawing->draw();
    m_ignore_signals = false;
}

void AutoColourEdit::setColourMap(const ColourMap& map)
{
    m_map = map;
    m_map.setType(ColourMap::CM_AUTO);
    for (int i = 0; i < 3; i++)
    {
        m_map.getAutoColour(static_cast<RGB>(i), m_auto_colour[i]);
        m_wave_drawing->setWaveform(static_cast<RGB>(i), m_auto_colour[i]);
    }
    display();
}

const ColourMap &AutoColourEdit::getColourMap()
{
    for (int i = 0; i < 3; i++)
    {
        m_map.setAutoColour(static_cast<RGB>(i), m_auto_colour[i]);
    }
    return m_map;
}

void AutoColourEdit::on_red_hoffset(int value)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[0].hoffset = value;
        m_wave_drawing->setWaveform(RGB_RED, m_auto_colour[0]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_red_voffset(int value)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[0].voffset = value;
        m_wave_drawing->setWaveform(RGB_RED, m_auto_colour[0]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_red_maximum(int value)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[0].maximum = value;
        m_wave_drawing->setWaveform(RGB_RED, m_auto_colour[0]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_red_wave(int index)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[0].wave = static_cast<Waveform::WAVE>(index);
        m_wave_drawing->setWaveform(RGB_RED, m_auto_colour[0]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_red_cycles(int value)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[0].cycles = value;
        m_wave_drawing->setWaveform(RGB_RED, m_auto_colour[0]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_red_reset(bool)
{
    if (!m_ignore_signals)
    {
        Waveform::AutoColour default_colours;
        m_auto_colour[0] = default_colours;
        m_wave_drawing->setWaveform(RGB_RED, default_colours);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_green_hoffset(int value)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[1].hoffset = value;
        m_wave_drawing->setWaveform(RGB_GREEN, m_auto_colour[1]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_green_voffset(int value)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[1].voffset = value;
        m_wave_drawing->setWaveform(RGB_GREEN, m_auto_colour[1]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}
    
void AutoColourEdit::on_green_maximum(int value)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[1].maximum = value;
        m_wave_drawing->setWaveform(RGB_GREEN, m_auto_colour[1]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_green_wave(int index)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[1].wave = static_cast<Waveform::WAVE>(index);
        m_wave_drawing->setWaveform(RGB_GREEN, m_auto_colour[1]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_green_cycles(int value)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[1].cycles = value;
        m_wave_drawing->setWaveform(RGB_GREEN, m_auto_colour[1]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_green_reset(bool)
{
    if (!m_ignore_signals)
    {
        Waveform::AutoColour default_colours;
        m_auto_colour[1] = default_colours;
        m_wave_drawing->setWaveform(RGB_GREEN, default_colours);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_blue_hoffset(int value)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[2].hoffset = value;
        m_wave_drawing->setWaveform(RGB_BLUE, m_auto_colour[2]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_blue_voffset(int value)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[2].voffset = value;
        m_wave_drawing->setWaveform(RGB_BLUE, m_auto_colour[2]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_blue_maximum(int value)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[2].maximum = value;
        m_wave_drawing->setWaveform(RGB_BLUE, m_auto_colour[2]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_blue_wave(int index)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[2].wave = static_cast<Waveform::WAVE>(index);
        m_wave_drawing->setWaveform(RGB_BLUE, m_auto_colour[2]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_blue_cycles(int value)
{
    if (!m_ignore_signals)
    {
        m_auto_colour[2].cycles = value;
        m_wave_drawing->setWaveform(RGB_BLUE, m_auto_colour[2]);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}

void AutoColourEdit::on_blue_reset(bool)
{
    if (!m_ignore_signals)
    {
        Waveform::AutoColour default_colours;
        m_auto_colour[2] = default_colours;
        m_wave_drawing->setWaveform(RGB_BLUE, default_colours);
//        m_wave_drawing->draw();
        display();
        emit changed();
    }
}
