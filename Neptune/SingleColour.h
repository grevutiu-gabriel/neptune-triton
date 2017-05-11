// 
// File:   SingleColour.h
// Author: M.R.Eggleston
//
// Created on 12 April 2013, 10:35
//

#pragma once

#include <QImage>
#include <QLabel>
#include <QSpinBox>
#include "TypesAndConstants.h"

class SingleColour : public QWidget
{
    Q_OBJECT
signals:
    void changed(const QColor& colour);
private slots:
    void on_red_changed(int value);
    void on_green_changed(int value);
    void on_blue_changed(int value);
    
public:
    SingleColour(COLOUR_TYPE type, QWidget* parent = nullptr);
    ~SingleColour();
    
    void setEnabled(bool value);
    void setColour(const QColor& colour);
    QColor value();
    
private:
    void setup_ui_layout();
    void connect_signals();
    
    void drawColour();
    void setColour();
    
    QImage m_colour_image;
    QLabel* m_colour_image_label;
    QSpinBox* m_red_spin;
    QSpinBox* m_blue_spin;
    QSpinBox* m_green_spin;
    QLabel* m_colour_label;
    COLOUR_TYPE m_type;
    int m_red;
    int m_green;
    int m_blue;
    bool m_ignore_signals;
};



