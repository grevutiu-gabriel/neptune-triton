//
// File:   ColourMap.cc
// Author: M.R.Eggleston
//
// Created on 18 August 2011, 14:09
//

#include <iostream>
#include "ColourMap.h"

ColourMap::ColourMap()
:
m_type(CM_MANUAL)
{
}

ColourMap::ColourMap(const VectorColour& v)
:
m_type(CM_MANUAL)
{
    int size = static_cast<int>(v.size());
    for (int i = 0; i < size; i++)
    {
        m_manual_map[i] = v[i];
    }
}

ColourMap::ColourMap(const ColourMap& orig)
{
    operator=(orig);
}

ColourMap& ColourMap::operator=(const ColourMap& other)
{
    m_type = other.m_type;
    m_manual_map = other.m_manual_map;
    for (int i = 0; i < 3; i++)
    {
        m_rgb[i] = other.m_rgb[i];
    }
    m_red_map = other.m_red_map;
    m_green_map = other.m_green_map;
    m_blue_map = other.m_blue_map;
    return *this;
}

ColourMap::~ColourMap()
{
}

void ColourMap::setType(MAP_TYPE type)
{
    m_type = type;
}

ColourMap::MAP_TYPE ColourMap::getType()
{
    return m_type;
}

void ColourMap::setColour(unsigned int index, const QColor& colour)
{
    m_manual_map[index] = colour;
}

void ColourMap::setRed(unsigned int index, unsigned int red)
{
    m_red_map[index] = red;
}

void ColourMap::setGreen(unsigned int index, unsigned int green)
{
    m_green_map[index] = green;
}

void ColourMap::setBlue(unsigned int index, unsigned int blue)
{
    m_blue_map[index] = blue;
}

void ColourMap::swapColour(unsigned int index1, unsigned int index2)
{
    auto colour = m_manual_map[index1];
    m_manual_map[index1] = m_manual_map[index2];
    m_manual_map[index2] = colour;
}

void ColourMap::swapRed(unsigned int index1, unsigned int index2)
{
    auto colour = m_red_map[index1];
    m_red_map[index1] = m_red_map[index2];
    m_red_map[index2] = colour;
}

void ColourMap::swapGreen(unsigned int index1, unsigned int index2)
{
    auto colour = m_green_map[index1];
    m_green_map[index1] = m_green_map[index2];
    m_green_map[index2] = colour;
}

void ColourMap::swapBlue(unsigned int index1, unsigned int index2)
{
    auto colour = m_blue_map[index1];
    m_blue_map[index1] = m_blue_map[index2];
    m_blue_map[index2] = colour;
}

void ColourMap::setAutoColour(RGB colour, const Waveform::AutoColour &definition)
{
    m_rgb[colour] = definition;
}

void ColourMap::getAutoColour(RGB colour, Waveform::AutoColour &definition)
{
    definition = m_rgb[colour];
}

const VectorColour ColourMap::getColours()
{
    VectorColour vcolours;
    // Create colour map from definition
    if (m_type == CM_AUTO)
    {
        QRgb c;
        for (int i = 0; i < 3 ; i++)
        {
            m_waveform[i].setWaveform(m_rgb[i]);
            m_waveform[i].calculateValues();
        }
        for (unsigned int i = 0; i < Waveform::NUMBER_OF_COLOURS; i++)
        {
            c = qRgb(m_waveform[RGB_RED].getAmplitude(i),
                     m_waveform[RGB_GREEN].getAmplitude(i),
                     m_waveform[RGB_BLUE].getAmplitude(i));
            vcolours.push_back(c);
        }
    }
    else if (m_type == CM_MANUAL)
    {
        int old_index = -1;
        QColor last_colour(0, 0, 0);
        int count;
        for (auto it = m_manual_map.begin(); it != m_manual_map.end(); it++)
        {
            count = it->first - old_index;
            old_index = it->first;
            QColor c = it->second;
            float red_gradient = static_cast<float>(c.red() - last_colour.red())/static_cast<float>(count);
            float green_gradient = static_cast<float>(c.green() - last_colour.green())/static_cast<float>(count);
            float blue_gradient = static_cast<float>(c.blue() - last_colour.blue())/static_cast<float>(count);
            // determine gradients
            for (int i=0; i < count-1; i++)
            {
                auto colour = QColor(last_colour.red() + static_cast<int>(red_gradient*static_cast<float>(i) + 0.5),
                                     last_colour.green() + static_cast<int>(green_gradient*static_cast<float>(i) + 0.5),
                                     last_colour.blue() + static_cast<int>(blue_gradient*static_cast<float>(i) + 0.5));
                vcolours.push_back(colour.rgb());
            }
            vcolours.push_back(c.rgb());
            last_colour = c;
        }
    }
    return vcolours;
}

void ColourMap::getColours(COMPONENT_ORDER order, VectorInt& reds, VectorInt& greens, VectorInt& blues)
{
    reds.clear();
    greens.clear();
    blues.clear();
    if ((m_type == CM_AUTO) || (m_type == CM_MANUAL))
    {
        VectorColour colours = getColours();
        int size = colours.size();
        int red;
        int green;
        int blue;
        for (int i = 0; i < size; i++)
        {
            splitColour(colours[i], order, red, green, blue);
            reds.push_back(red);
            greens.push_back(green);
            blues.push_back(blue);
        }
    }
    else
    {
        switch(order)
        {
        case CO_RGB:
            getColours(RGB_RED, reds);
            getColours(RGB_GREEN, greens);
            getColours(RGB_BLUE, blues);
            break;
        case CO_RBG:
            getColours(RGB_RED, reds);
            getColours(RGB_GREEN, blues);
            getColours(RGB_BLUE, greens);
            break;
        case CO_GRB:
            getColours(RGB_RED, greens);
            getColours(RGB_GREEN, reds);
            getColours(RGB_BLUE, blues);
            break;
        case CO_GBR:
            getColours(RGB_RED, greens);
            getColours(RGB_GREEN, blues);
            getColours(RGB_BLUE, reds);
            break;
        case CO_BRG:
            getColours(RGB_RED, blues);
            getColours(RGB_GREEN, reds);
            getColours(RGB_BLUE, greens);
            break;
        case CO_BGR:
            getColours(RGB_RED, blues);
            getColours(RGB_GREEN, greens);
            getColours(RGB_BLUE, reds);
            break;
        }
    }
}

void ColourMap::getColours(RGB colour, VectorInt& colours)
{
    int old_index = -1;
    int last_colour = 0;
    IntMap& map = (colour == RGB_RED ? m_red_map : (colour == RGB_GREEN ? m_green_map : m_blue_map));
    int count;
    for (auto it = map.begin(); it != map.end(); it++)
    {
        count = it->first - old_index;
        old_index = it->first;
        int c = it->second;
        float gradient = static_cast<float>(c - last_colour)/static_cast<float>(count);
        // determine gradients
        for (int i=0; i < count-1; i++)
        {
            auto colour = last_colour + static_cast<int>(gradient*static_cast<float>(i) + 0.5);
            colours.push_back(colour);
        }
        colours.push_back(c);
        last_colour = c;
    }
    if (colours.size() == 0)
    {
        // If empty add a single black entry
        colours.push_back(0);
    }
}

void ColourMap::splitColour(QRgb colour, COMPONENT_ORDER order, int& r, int& g, int& b)
{
    switch(order)
    {
    case CO_RGB:
        r = qRed(colour);
        g = qGreen(colour);
        b = qBlue(colour);
        break;
    case CO_RBG:
        r = qRed(colour);
        g = qBlue(colour);
        b = qGreen(colour);
        break;
    case CO_GRB:
        r = qGreen(colour);
        g = qRed(colour);
        b = qBlue(colour);
        break;
    case CO_GBR:
        r = qGreen(colour);
        g = qBlue(colour);
        b = qRed(colour);
        break;
    case CO_BRG:
        r = qBlue(colour);
        g = qRed(colour);
        b = qGreen(colour);
        break;
    case CO_BGR:
        r = qBlue(colour);
        g = qGreen(colour);
        b = qRed(colour);
        break;
    }
}


const ColoursMap &ColourMap::getColoursMap()
{
    return m_manual_map;
}

const IntMap &ColourMap::getRedMap()
{
    return m_red_map;
}

const IntMap &ColourMap::getGreenMap()
{
    return m_green_map;
}

const IntMap &ColourMap::getBlueMap()
{
    return m_blue_map;
}

void ColourMap::clear()
{
    clearAuto();
    clearManual();
    clearReds();
    clearGreens();
    clearBlues();
}

void ColourMap::clearAuto()
{
    Waveform::AutoColour auto_colour;
    for (int i=0; i < 3; i++)
    {
        m_rgb[i] = auto_colour;
    }
}

void ColourMap::clearManual()
{
    m_manual_map.clear();
}

void ColourMap::clearReds()
{
    m_red_map.clear();
}

void ColourMap::clearGreens()
{
    m_green_map.clear();
}

void ColourMap::clearBlues()
{
    m_blue_map.clear();
}

bool ColourMap::empty()
{
    if (m_type == CM_MANUAL)
    {
        return m_manual_map.empty();
    }
    else if (m_type == CM_COMPONENT)
    {
        return (m_red_map.empty() && m_green_map.empty() && m_blue_map.empty());
    }
    else
    {
        return  false;
    }
}

bool ColourMap::equal(ColourMap &other)
{
    VectorInt reds;
    VectorInt greens;
    VectorInt blues;
    VectorInt other_reds;
    VectorInt other_greens;
    VectorInt other_blues;
    getColours(CO_RGB, reds, greens, blues);
    other.getColours(CO_RGB, other_reds, other_greens, other_blues);
    unsigned int red_size = reds.size();
    unsigned int green_size = greens.size();
    unsigned int blue_size = blues.size();
    bool match = (other_reds.size() == red_size) &&
                 (other_greens.size() == green_size) &&
                 (other_blues.size() == blue_size);
    if (match)
    {
        for (unsigned int i = 0; i < red_size; i++)
        {
            match = (reds[i] == other_reds[i]);
            if (!match) break;
        }
    }
    if (match)
    {
        for (unsigned int i = 0; i < green_size; i++)
        {
            match = (greens[i] == other_greens[i]);
            if (!match) break;
        }
    }
    if (match)
    {
        for (unsigned int i = 0; i < blue_size; i++)
        {
            match = (blues[i] == other_blues[i]);
            if (!match) break;
        }
    }
    return match;
}

void ColourMap::deleteColour(int index)
{
    m_manual_map.erase(index); // ignore return value.
}

void ColourMap::deleteRed(int index)
{
    m_red_map.erase(index); // ignore return value.
}

void ColourMap::deleteGreen(int index)
{
    m_green_map.erase(index); // ignore return value.
}

void ColourMap::deleteBlue(int index)
{
    m_blue_map.erase(index); // ignore return value.
}

void ColourMap::draw(QImage& image, int r_offset, int g_offset, int b_offset, COMPONENT_ORDER order)
{
    VectorInt reds;
    VectorInt greens;
    VectorInt blues;
    getColours(order, reds, greens, blues);
    int r_size = reds.size();
    int g_size = greens.size();
    int b_size = blues.size();
    bool all_black = ((r_size + g_size + b_size) == 0);
    if (all_black)
    {
        image.fill(0);
    }
    else
    {
        int height = image.height();
        int width = image.width();
        for (int row = 0; row < height; row++)
        {
            QRgb *pixel = reinterpret_cast<QRgb *>(image.scanLine(row));
            for (int i = 0; i < width; i++)
            {
                int r = (r_size == 0 ? 0 : reds[(i + r_offset) % r_size]);
                int g = (g_size == 0 ? 0 : greens[(i + g_offset) % g_size]);
                int b = (b_size == 0 ? 0 : blues[(i + b_offset) % b_size]);
                pixel[i] = qRgb(r, g, b);
            }
        }
    }
}
