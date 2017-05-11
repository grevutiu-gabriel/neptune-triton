//
// File:   Waveform.h
// Author: trevone
//
// Created on 22 August 2011, 11:54
//

#pragma once

#include "TypesAndConstants.h"

class Waveform
{
public:
    static const unsigned int NUMBER_OF_COLOURS;

    enum WAVE
    {
        WAVE_SINE,
        WAVE_SAW,
        WAVE_PARABOLA
    };

    struct AutoColour
    {
        AutoColour() : hoffset(0), voffset(0), maximum(255), wave(WAVE_SINE), cycles(1) {};
        int hoffset;
        int voffset;
        int maximum;
        WAVE wave;
        int cycles;
    };

    Waveform();
    virtual ~Waveform();

    void setWaveform(const AutoColour &auto_colour);
    void calculateValues();
    int getAmplitude(unsigned int index) const;

private:
    Waveform(const Waveform& orig);

    void calculateSine();
    void calculateSaw();
    void calculateParabolic();

    AutoColour m_auto_colour;
    VectorUInt m_values;
    
    // Declare a constant for pi as some systems do not define M_PIl
    static const long double m_pi;
};
