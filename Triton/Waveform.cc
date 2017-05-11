//
// File:   Waveform.cc
// Author: M.R.Eggleston
//
// Created on 22 August 2011, 11:54
//

#include <cmath>
#include "Waveform.h"

const unsigned int Waveform::NUMBER_OF_COLOURS = 512;
const long double Waveform::m_pi = 2.0L*acos(0.0L);

Waveform::Waveform()
:
m_values(NUMBER_OF_COLOURS)
{
}

Waveform::~Waveform()
{
}

void Waveform::setWaveform(const AutoColour& auto_colour)
{
    m_auto_colour = auto_colour;
}

void Waveform::calculateValues()
{
    switch (m_auto_colour.wave)
    {
    default:
    case WAVE_SINE:
        calculateSine();
        break;
    case WAVE_SAW:
        calculateSaw();
        break;
    case WAVE_PARABOLA:
        calculateParabolic();
        break;
    }
}

void Waveform::calculateSine()
{
    // This will eventually switch between wave types
    // initially a sine wave
    double amplitude = static_cast<double>(m_auto_colour.maximum)/2.0;
    double increment = (static_cast<double>(m_auto_colour.cycles)*2.0*m_pi)/512.0;
    for (unsigned int i=0; i < NUMBER_OF_COLOURS; i++)
    {
        double pos = static_cast<double>((i+m_auto_colour.hoffset) % NUMBER_OF_COLOURS);
        double dvalue = sin(increment*pos) + 1.0;
        m_values[i] = static_cast<unsigned int>(dvalue*amplitude);
        m_values[i] += m_auto_colour.voffset;
        if (m_values[i] > 255)
        {
            m_values[i] = 255 - (m_values[i] % 256);
        }
    }
}

void Waveform::calculateSaw()
{
    // The saw tooth waveform
    double amplitude = static_cast<double>(m_auto_colour.maximum);
    double increment =  (static_cast<double>(m_auto_colour.cycles)*amplitude)/256.0;
    int cycle_size = NUMBER_OF_COLOURS/m_auto_colour.cycles;
    for (unsigned int i=0; i < NUMBER_OF_COLOURS; i++)
    {
        double pos = static_cast<double>((i+m_auto_colour.hoffset) % cycle_size);
        double dvalue = pos*increment;
        if (dvalue > amplitude)
        {
            dvalue =  2.0*amplitude - dvalue;
        }
        m_values[i] = static_cast<unsigned int>(dvalue) + m_auto_colour.voffset;
        if (m_values[i] > 255)
        {
            m_values[i] = 255 - (m_values[i] % 256);
        }
    }
}

void Waveform::calculateParabolic()
{
    // One cycle is a parabola where the first and last values are 255
    // and the central value is 0.
    double amplitude = static_cast<double>(m_auto_colour.maximum);
    double central = 512.0/(2.0*static_cast<double>(m_auto_colour.cycles));
    double increment = sqrt(amplitude)/central;
    int cycle_size = NUMBER_OF_COLOURS/m_auto_colour.cycles;
    for (unsigned int i=0; i < NUMBER_OF_COLOURS; i++)
    {
        double pos = static_cast<double>((i+m_auto_colour.hoffset) % cycle_size);
        double dvalue = (pos - central)*increment;
        m_values[i] = static_cast<unsigned int>(dvalue*dvalue);
        if (m_values[i] > amplitude)
        {
            m_values[i] =  static_cast<unsigned int>(2.0*amplitude) - m_values[i];
        }
        m_values[i] += m_auto_colour.voffset;
        if (m_values[i] > 255)
        {
            m_values[i] = 255 - (m_values[i] % 256);
        }
    }
}

int  Waveform::getAmplitude(unsigned int index) const
{
    unsigned int value = 0;
    if (index < NUMBER_OF_COLOURS)
    {
        value = m_values[(index + m_auto_colour.hoffset) % NUMBER_OF_COLOURS];
    }
    return value;
}
