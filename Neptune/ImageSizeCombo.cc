// 
// File:   ImageSizeCombo.cc
// Author: M.R.Eggleston
// 
// Created on 20 February 2012, 11:29
//

#include "Utilities.h"
#include "ImageSizeCombo.h"

ImageSizeCombo::ImageSizeCombo(QWidget* parent)
:
QComboBox(parent)
{
    int widths[11] = { 640, 640, 640, 640, 750, 750, 750, 1280, 1280, 1280, 0 };
    int heights[11] = { 360, 400, 480, 640, 450, 500, 750, 800, 960, 1280, 0 };
    for (int i = 0; i < 11; i++)
    {
        int w = widths[i];
        int h = heights[i];
        QString str;
        m_width.push_back(w);
        m_height.push_back(h);
        if (w != 0)
        {
            str = QString::number(w) + "x" + QString::number(h);
        }
        else
        {
            str = "Custom";
        }
        addItem(str);
    }
    setCurrentIndex(5); // 750x500
    connect(this, SIGNAL(activated(int)), this, SLOT(on_activated(int)));
}

ImageSizeCombo::~ImageSizeCombo()
{
}

void ImageSizeCombo::Set(int w, int h)
{
    bool found = false;
    int index = 0;
    for (int i = 0; i < 10; i++) // ignore "Custom"
    {
        if ((m_width[i] == w) && (m_height[i] == h))
        {
            found = true;
            break;
        }
        index++;
    }
    setCurrentIndex(found ? index : 10);
}

void ImageSizeCombo::Get(int &w, int &h)
{
    try
    {
        int index = currentIndex();
        w = m_width[index];
        h = m_height[index];
    }
    catch (...)
    {
        w = 0;
        h = 0;
    }
}

bool ImageSizeCombo::isCustom()
{
    return (currentIndex() == 10);
}

void ImageSizeCombo::on_activated(int)
{
    int w;
    int h;
    Get(w, h);
    emit changed(w, h);
}