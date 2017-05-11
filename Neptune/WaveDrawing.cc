// 
// File:   WaveDrawing.h
// Author: M.R.Eggleston
//
// Created on 19 April 2013, 14:43
//

#include <iostream>
#include <QPainter>
#include "WaveDrawing.h"

WaveDrawing::WaveDrawing(QWidget*parent)
:
QWidget(parent)
{
    setFixedWidth(512);
    setFixedHeight(256);
}

void WaveDrawing::setWaveform(RGB colour, const Waveform::AutoColour& waveform)
{
    int index = static_cast<int>(colour);
    m_waveform[index].setWaveform(waveform);
    m_waveform[index].calculateValues();
}

void WaveDrawing::drawWave(QPainter& painter, RGB colour)
{
    painter.setPen(Qt::SolidLine);
    QColor pen_colour;
    if (colour == RGB_RED)
    {
        pen_colour = Qt::red;
    }
    else if (colour == RGB_GREEN)
    {
        pen_colour = Qt::green;
    }
    else
    {
        pen_colour = Qt::blue;
    }
    painter.setPen(pen_colour);
    int index = static_cast<int>(colour);
    // The graph is calculated using bottom left as the origin, however
    // the "logical" co-ordinate system use for drawing has its origin at the
    // top left and increasing vertical values go down.
    for (int i = 1; i < 512; i++)
    {
        painter.drawLine(QLine(i-1, 255 - m_waveform[index].getAmplitude(i-1),
                               i,   255 - m_waveform[index].getAmplitude(i)));
    }
}

void WaveDrawing::draw()
{
    update(rect());
}

void WaveDrawing::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), Qt::white);
    painter.setRenderHints(QPainter::Antialiasing);
    drawWave(painter, RGB_RED);
    drawWave(painter, RGB_GREEN);
    drawWave(painter, RGB_BLUE);
}