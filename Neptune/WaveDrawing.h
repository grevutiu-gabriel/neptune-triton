// 
// File:   WaveDrawing.h
// Author: trevone
//
// Created on 19 April 2013, 14:34
//

#pragma once

#include <QWidget>
#include "TypesAndConstants.h"
#include "Waveform.h"

class WaveDrawing : public QWidget
{
    Q_OBJECT
public:
    WaveDrawing(QWidget* parent = nullptr);
    
    void setWaveform(RGB colour, const Waveform::AutoColour& waveform);
    void draw();
    
private:
    void paintEvent(QPaintEvent*);
    void drawWave(QPainter& painter, RGB colour);
    
    Waveform m_waveform[3];
};