//
// File:   Colours.cc
// Author: M.R.Eggleston
//
// Created on 19 November 2010, 19:21
//

#include "Colours.h"
#include "Utilities.h"

const long double Colours::m_pi = 2.0L*acos(0.0L);

Colours::Colours()
:
m_smooth(false),
m_magnitude_type('n'),
m_red_offset(0),
m_green_offset(0),
m_blue_offset(0),
m_scale(0.0L),
m_limit(0.0L),
m_abs_log(false)
{
}

Colours::~Colours()
{
    allocate(0, 0, 0);
}

void Colours::setColours(COMPONENT_ORDER order, ColourMap& map)
{
    VectorInt reds;
    VectorInt greens;
    VectorInt blues;
    map.getColours(order, reds, greens, blues);
    unsigned int number_of_reds = reds.size();
    unsigned int number_of_greens = greens.size();
    unsigned int number_of_blues = blues.size();
    allocate(number_of_reds, number_of_greens, number_of_blues);
    for (unsigned int i=0; i < number_of_reds; i++)
    {
        m_reds[i] = reds[(i + m_red_offset) % number_of_reds];
    }
    for (unsigned int i=0; i < number_of_greens; i++)
    {
        m_greens[i] = greens[(i + m_green_offset) % number_of_greens];
    }
    for (unsigned int i=0; i < number_of_blues; i++)
    {
        m_blues[i] = blues[(i + m_blue_offset) % number_of_blues];
    }
}

void Colours::setFixed(const QRgb &c)
{
    allocate(0, 0, 0);
    m_fixed = c;
    m_function_type = CF_FIXED;
}

void Colours::setOffsets(unsigned int r, unsigned int g, unsigned int b)
{
    m_red_offset = r;
    m_green_offset = g;
    m_blue_offset = b;
}

void Colours::setIteration(long double limit)
{
    m_limit = limit;
    m_function_type = CF_ITERATION;
}

void Colours::setFinalAngle(long double scale, bool second)
{
    m_scale = scale;
    m_second = second;
    m_function_type = CF_FINAL_ANGLE;
}

void Colours::setFinalMagnitude(char type, long double scale, bool second)
{
    m_magnitude_type = type;
    m_scale = scale;
    m_second = second;
    m_function_type = CF_FINAL_MAGNITUDE;
}

void Colours::allocate(unsigned int r_size, unsigned int g_size, unsigned int b_size)
{
    m_number_of_reds = (r_size == 0 ? 1 : r_size);
    m_reds.resize(m_number_of_reds);
    m_number_of_greens = (g_size == 0 ? 1 : g_size);
    m_greens.resize(m_number_of_greens);
    m_number_of_blues = (b_size == 0 ? 1 : b_size);
    m_blues.resize(m_number_of_blues);
}

void Colours::setGaussian(char gaussian_method,
                          long double scale,
                          long double ang_scale,
                          bool second)
{
    m_method = gaussian_method;
    m_scale = scale;
    m_angle_scale = ang_scale;
    m_second = second;
    m_function_type = CF_GAUSSIAN;
}


void Colours::setSmooth(bool value)
{
    m_smooth = value;
}


void Colours::setStatistics(char method,
                            long double scale,
                            bool second)
{
    m_method = method;
    if (m_method < 'a')
    {
        m_method = m_method - 'A' + 'a';
    }
    m_scale = scale;
    m_second = second;
    m_function_type = CF_STATISTICS;
}

void Colours::setAbsLog(bool value)
{
    m_abs_log = value;
}

