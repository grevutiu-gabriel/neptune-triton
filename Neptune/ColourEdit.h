// 
// File:   ColourEdit.h
// Author: M.R.Eggleston
//
// Created on 14 April 2013, 14:24
//

#pragma once

#include <QLabel>
#include "ColourMap.h"

class ColourEdit : public QWidget
{
    Q_OBJECT
public:
    ColourEdit(QWidget* parent = nullptr);
    ~ColourEdit();
        
protected:
    virtual void setup_ui_layout() = 0;

    void draw_maps();

    QImage m_red_drawing;
    QLabel* m_red_label;
    QImage m_green_drawing;
    QLabel* m_green_label;
    QImage m_blue_drawing;
    QLabel* m_blue_label;
    QImage m_colour_drawing;
    QLabel* m_colour_label;
    int m_drawing_width;
    int m_drawing_height;
    
    ColourMap m_map;
    
private:
    ColourEdit(const ColourEdit& other);
};
