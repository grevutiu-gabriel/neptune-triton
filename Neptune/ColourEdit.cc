// 
// File:   ColourEdit.h
// Author: M.R.Eggleston
//
// Created on 14 April 2013, 14:30
//

#include "ColourEdit.h"

ColourEdit::ColourEdit(QWidget* parent)
:
QWidget(parent),
m_red_label(new QLabel()),
m_green_label(new QLabel()),
m_blue_label(new QLabel()),
m_colour_label(new QLabel()),
m_drawing_width(512),
m_drawing_height(35)
{
    m_red_drawing = QImage(m_drawing_width, m_drawing_height, QImage::Format_RGB32);
    m_red_drawing.fill(0);
    m_red_label->setPixmap(QPixmap::fromImage(m_red_drawing));
    m_green_drawing = QImage(m_drawing_width, m_drawing_height, QImage::Format_RGB32);
    m_green_drawing.fill(0);
    m_green_label->setPixmap(QPixmap::fromImage(m_green_drawing));
    m_blue_drawing = QImage(m_drawing_width, m_drawing_height, QImage::Format_RGB32);
    m_blue_drawing.fill(0);
    m_blue_label->setPixmap(QPixmap::fromImage(m_blue_drawing));
    m_colour_drawing = QImage(m_drawing_width, m_drawing_height, QImage::Format_RGB32);
    m_colour_drawing.fill(0);
    m_colour_label->setPixmap(QPixmap::fromImage(m_colour_drawing));
}

ColourEdit::~ColourEdit()
{
}

void ColourEdit::draw_maps()
{
    VectorInt reds;
    VectorInt greens;
    VectorInt blues;
    m_map.getColours(CO_RGB, reds, greens, blues);
    int r_size = reds.size();
    int g_size = greens.size();
    int b_size = blues.size();
    int all_black = ((r_size + g_size + b_size) == 0);
    if (all_black)
    {
        m_colour_drawing.fill(0);
        m_red_drawing.fill(0);
        m_green_drawing.fill(0);
        m_blue_drawing.fill(0);
    }
    else
    {
        for (int row = 0; row < m_drawing_height; row++)
        {
            QRgb *rgb_pixel = reinterpret_cast<QRgb *>(m_colour_drawing.scanLine(row));
            QRgb *red_pixel = reinterpret_cast<QRgb *>(m_red_drawing.scanLine(row));
            QRgb *green_pixel = reinterpret_cast<QRgb *>(m_green_drawing.scanLine(row));
            QRgb *blue_pixel = reinterpret_cast<QRgb *>(m_blue_drawing.scanLine(row));
            for (int i = 0; i < m_drawing_width; i++)
            {
                int r = (r_size == 0 ? 0 : reds[i % r_size]);
                int g = (g_size == 0 ? 0 : greens[i % g_size]);
                int b = (b_size == 0 ? 0 : blues[i % b_size]);
                rgb_pixel[i] = qRgb(r, g, b);
                red_pixel[i] = qRgb(r, 0, 0);
                green_pixel[i] = qRgb(0, g, 0);
                blue_pixel[i] = qRgb(0, 0, b);
            }
        }
    }
    m_colour_label->setPixmap(QPixmap::fromImage(m_colour_drawing));
    m_red_label->setPixmap(QPixmap::fromImage(m_red_drawing));
    m_green_label->setPixmap(QPixmap::fromImage(m_green_drawing));
    m_blue_label->setPixmap(QPixmap::fromImage(m_blue_drawing));
}

