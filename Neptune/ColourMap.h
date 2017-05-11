//
// File:   ColourMap.h
// Author: M.R.Eggleston
//
// Created on 18 August 2011, 14:09
//

#pragma once

#include <map>
#include <vector>
#include <QImage>
#include "TypesAndConstants.h"
#include "Waveform.h"

typedef std::map<int, QColor> ColoursMap;

class ColourMap
{
public:
    enum MAP_TYPE
    {
        CM_AUTO,
        CM_MANUAL,
        CM_COMPONENT
    };

    static const int NUMBER_OF_AUTO_COLOURS;

    ColourMap();
    ColourMap(const VectorColour &v);
    ColourMap(const ColourMap& orig);
    ColourMap& operator=(const ColourMap& other);
    virtual ~ColourMap();

    void setType(MAP_TYPE);
    MAP_TYPE getType();
    void setColour(unsigned int index, const QColor& colour);
    void setRed(unsigned int index, unsigned int red);
    void setGreen(unsigned int index, unsigned int green);
    void setBlue(unsigned int index, unsigned int green);
    void swapColour(unsigned int index1, unsigned int index2);
    void swapRed(unsigned int index1, unsigned int index2);
    void swapGreen(unsigned int index1, unsigned int index2);
    void swapBlue(unsigned int index1, unsigned int index2);
    void setAutoColour(RGB colour, const Waveform::AutoColour &definition);
    void getAutoColour(RGB colour, Waveform::AutoColour &definition);
    void clear();
    void clearAuto();
    void clearManual();
    void clearReds();
    void clearGreens();
    void clearBlues();
    bool empty();
    bool equal(ColourMap &other);
    void deleteColour(int index);
    void deleteRed(int index);
    void deleteGreen(int index);
    void deleteBlue(int index);

    const VectorColour getColours();
    void getColours(COMPONENT_ORDER order, VectorInt& reds, VectorInt& greens, VectorInt& blue);
    const IntMap& getRedMap();
    const IntMap& getGreenMap();
    const IntMap& getBlueMap();
    const ColoursMap &getColoursMap();
    
    void draw(QImage& image, int r_offset = 0, int g_offset = 0, int b_offset = 0, COMPONENT_ORDER order = CO_RGB);

private:
    VectorColour calculateSine();
    VectorColour calculateSaw();
    VectorColour calculateParabolic();
    void getColours(RGB colour, VectorInt& values);
    void splitColour(QRgb colour, COMPONENT_ORDER order, int& r, int& g, int& b);

    ColoursMap m_manual_map;
    IntMap m_red_map;
    IntMap m_green_map;
    IntMap m_blue_map;
    Waveform::AutoColour m_rgb[3];
    Waveform m_waveform[3];

    MAP_TYPE m_type;
};

