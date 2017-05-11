//
// File:   Colours.h
// Author: M.R.Eggleston
//
// Created on 19 October 2010, 15:52
//

#pragma once

#include <iostream>
#include "ldcomplex.h"
#include "ColourMap.h"
#include "TypesAndConstants.h"
#include "Utilities.h"

class Colours
{
public:
    Colours();
    virtual ~Colours();

    void setColours(COMPONENT_ORDER order, ColourMap& map);
    QRgb getColour(ResultsSet *rs);

    void setFixed(const QRgb& c);
    void setOffsets(unsigned int r, unsigned int g, unsigned int b);
    void setIteration(long double limit);
    void setFinalAngle(long double seg_size, bool second);
    void setFinalMagnitude(char type, long double seg_size, bool second);
    void setStatistics(char method, long double seg_size, bool second);
    void setGaussian(char method, long double seg_size, long double ang_seg_size, bool second);
    void setSmooth(bool value);
    void setAbsLog(bool value);

private:
    Colours(const Colours& orig);

    void allocate(unsigned int r_size, unsigned int g_size, unsigned int b_size);
    QRgb iterationColour();
    QRgb fixedColour();
    QRgb finalAngleColour();
    QRgb finalMagnitudeColour();
    QRgb gaussianColour();
    QRgb statisticsColour();
    QRgb valueColour();
    
    enum COLOUR_FUNCTION
    {
        CF_FIXED,
        CF_ITERATION,
        CF_FINAL_ANGLE,
        CF_FINAL_MAGNITUDE,
        CF_GAUSSIAN,
        CF_STATISTICS,
        CF_VALUE
    };

    QRgb colourFromValue(long double value);

    bool m_smooth;
    QRgb m_fixed;
    VectorInt m_reds;
    VectorInt m_greens;
    VectorInt m_blues;
    unsigned int m_number_of_reds;
    unsigned int m_number_of_greens;
    unsigned int m_number_of_blues;
    char m_magnitude_type;
    unsigned int m_red_offset;
    unsigned int m_green_offset;
    unsigned int m_blue_offset;
    long double m_scale;
    long double m_angle_scale;
    char m_method;
    long double m_limit;
    ResultsSet *m_rs;
    bool m_second; // second set of statistics
    bool m_abs_log;

    COLOUR_FUNCTION m_function_type;
    // Declare a constant for pi as some systems do not define M_PIl
    static const long double m_pi;
};

inline QRgb Colours::colourFromValue(long double value)
{
    long double weight = std::abs(value - trunc(value));
    int base_index = static_cast<int>(std::abs(value));
    int red_index1;
    int red_index2;
    int green_index1;
    int green_index2;
    int blue_index1;
    int blue_index2;
    if (value < 0)
    {
        red_index1 = (m_number_of_reds - 1) - (base_index % m_number_of_reds);
        red_index2 = (m_number_of_reds - 1) - ((base_index + 1) % m_number_of_reds);
        green_index1 = (m_number_of_greens - 1) - (base_index % m_number_of_greens);
        green_index2 = (m_number_of_greens - 1) - ((base_index + 1) % m_number_of_greens);
        blue_index1 = (m_number_of_blues - 1) - (base_index % m_number_of_blues);
        blue_index2 = (m_number_of_blues - 1) - ((base_index + 1) % m_number_of_blues);
    }
    else
    {
        red_index1 = (base_index) % m_number_of_reds;
        red_index2 = (base_index + 1) % m_number_of_reds;
        green_index1 = (base_index) % m_number_of_greens;
        green_index2 = (base_index + 1) % m_number_of_greens;
        blue_index1 = (base_index) % m_number_of_blues;
        blue_index2 = (base_index + 1) % m_number_of_blues;
    }
    QRgb colour = qRgb(m_reds[red_index1], m_greens[green_index1], m_blues[blue_index1]);
    if (m_smooth)
    {
        QRgb colour2 = qRgb(m_reds[red_index2], m_greens[green_index2], m_blues[blue_index2]);
        long double r1 = static_cast<long double>(qRed(colour));
        long double g1 = static_cast<long double>(qGreen(colour));
        long double b1 = static_cast<long double>(qBlue(colour));
        long double r2 = static_cast<long double>(qRed(colour2));
        long double g2 = static_cast<long double>(qGreen(colour2));
        long double b2 = static_cast<long double>(qBlue(colour2));
        colour = qRgb(static_cast<int>((1.0L - weight)*r1 + weight*r2 + 0.5L),
                      static_cast<int>((1.0L - weight)*g1 + weight*g2 + 0.5L),
                      static_cast<int>((1.0L - weight)*b1 + weight*b2 + 0.5L));
    }
    return colour;
}

inline QRgb Colours::getColour(ResultsSet *rs)
{
    m_rs = rs;
    switch (m_function_type)
    {
    case CF_FIXED:
        return m_fixed;
    default:
    case CF_ITERATION:
        return iterationColour();
    case CF_FINAL_ANGLE:
        return finalAngleColour();
    case CF_FINAL_MAGNITUDE:
        return finalMagnitudeColour();
    case CF_GAUSSIAN:
        return gaussianColour();
    case CF_STATISTICS:
        return statisticsColour();
    }
}

inline QRgb Colours::iterationColour()
{
    using std::abs;
    QRgb colour;
    auto& set = m_rs->set[0];
    if (m_smooth)
    {
        long double value = static_cast<long double>(set.escape) + m_limit/(abs(LongComplex(set.z_r, set.z_i)) + 1.0e-9L);
        colour = colourFromValue(value);
    }
    else
    {
         int r_index = set.escape % m_number_of_reds;
         int g_index = set.escape % m_number_of_greens;
         int b_index = set.escape % m_number_of_blues;
         colour = qRgb(m_reds[r_index], m_greens[g_index], m_blues[b_index]);
    }
    return colour;
}

inline QRgb Colours::finalAngleColour()
{
    auto& s = m_rs->set[m_second];
    LongComplex z(s.z_r, s.z_i);
    long double value = arg(z);
    if (value < 0.0L) value += 2.0L*m_pi;
    if (m_abs_log) value = std::abs(std::log(value));
    return colourFromValue(value*m_scale);
}

inline QRgb Colours::finalMagnitudeColour()
{
    using std::abs;
    long double value;
    auto& s = m_rs->set[m_second];
    switch (m_magnitude_type)
    {
    default:
    case 'n':
        value = s.z_r*s.z_r + s.z_i*s.z_i;
        break;
    case 's':
        {
            long double z_r = abs(s.z_r);
            long double z_i = abs(s.z_i);
            value = (z_r < z_i) ? z_r : z_i;
        }
        break;
    case 'g':
        {
            long double z_r = abs(s.z_r);
            long double z_i = abs(s.z_i);
            value = (z_r > z_i) ? z_r : z_i;
        }
        break;
    case 'r':
        value = s.z_r;
        break;
    case 'R':
        value = abs(s.z_r);
        break;
    case 'i':
        value = s.z_i;
        break;
    case 'I':
        value = abs(s.z_i);
        break;
    case 'c':
        value = s.z_r + s.z_i;
        break;
    case 'C':
        value = abs(s.z_r + s.z_i);
        break;
    case 'p':
        value = s.z_r*s.z_i;
        break;
    case 'P':
        value = abs(s.z_r*s.z_i);
        break;
    }
    if (m_abs_log)
    {
        if (value < 0.0L)
        {
            // The log of a -ve number is complex
            LongComplex log_value = ld::log(LongComplex(value, 0.0L));
            value = ld::abs(log_value);
        }
        else
        {
            value = std::abs(std::log(value));
        }
    }
    return colourFromValue(value*m_scale);
}

inline QRgb Colours::gaussianColour()
{
    using std::sqrt;
    QRgb colour;
    Statistics& s = m_rs->set[m_second];

    if (m_method == 'b')
    {
        int r_index = s.iteration_at_min % m_number_of_reds;
        int g_index = s.iteration_at_min % m_number_of_greens;
        int b_index = s.iteration_at_min % m_number_of_blues;
        colour = qRgb(m_reds[r_index], m_greens[g_index], m_blues[b_index]);
    }
    else if (m_method == 'e')
    {
        int r_index = s.iteration_at_max % m_number_of_reds;
        int g_index = s.iteration_at_max % m_number_of_greens;
        int b_index = s.iteration_at_max % m_number_of_blues;
        colour = qRgb(m_reds[r_index], m_greens[g_index], m_blues[b_index]);
    }
    else
    {
        long double value;
        long double scale = m_scale;
        switch (m_method)
        {
        default:
        case 'a':
            value = norm(s.min_dist);
            break;
        case 'c':
            value = arg(s.min_dist) + m_pi;
            scale = m_angle_scale;
            break;
        case 'd':
            value = norm(s.max_dist);
            break;
        case 'f':
            value = arg(s.max_dist) + m_pi;
            scale = m_angle_scale;
            break;
        case 'g':
            value = s.square_total/static_cast<long double>(s.escape);
            break;
        case 'h':
            value = s.minimum;
            scale = m_angle_scale;
            break;
        case 'i':
            value = s.angle_total/static_cast<long double>(s.escape);
            scale = m_angle_scale;
            break;
        case 'j':
            value = s.maximum;
            scale = m_angle_scale;
            break;
        case 'k':
            value = norm(s.max_dist)/norm(s.min_dist);
            break;
        case 'l':
            value = norm(s.max_dist) - norm(s.min_dist);
            break;
        case 'm':
            // Variance
            value = (s.square_total - (s.total*s.total)/s.escape)/s.escape;
            break;
        case 'n':
            // Standard deviation
            value = sqrt((s.square_total - (s.total*s.total)/s.escape)/s.escape);
            break;
        case 'o':
            // Coefficient of variation
            value = sqrt((s.square_total - (s.total*s.total)/s.escape)/s.escape);
            value /= (s.total/s.escape);
            break;
        }
        if (m_abs_log) value = std::abs(std::log(value));
        colour = colourFromValue(value*scale);
    }
    return colour;
}

inline QRgb Colours::statisticsColour()
{
    using std::sqrt;
    Statistics& s = m_rs->set[m_second];
    QRgb colour;
    if (m_method == 'b')
    {
        int r_index = s.iteration_at_min % m_number_of_reds;
        int g_index = s.iteration_at_min % m_number_of_greens;
        int b_index = s.iteration_at_min % m_number_of_blues;
        colour = qRgb(m_reds[r_index], m_greens[g_index], m_blues[b_index]);
    }
    else if (m_method == 'd')
    {
        int r_index = s.iteration_at_max % m_number_of_reds;
        int g_index = s.iteration_at_max % m_number_of_greens;
        int b_index = s.iteration_at_max % m_number_of_blues;
        colour = qRgb(m_reds[r_index], m_greens[g_index], m_blues[b_index]);
    }
    else
    {
        long double value;
        switch (m_method)
        {
        default:
        case 'a':
            value = s.minimum;
            break;
        case 'c':
            value = s.maximum;
            break;
        case 'e':
            value = s.maximum - s.minimum;
            break;
        case 'f':
            value = s.total/s.escape;
            break;
        case 'g':
            // Variance
            value = (s.square_total - (s.total*s.total)/s.escape)/s.escape;
            break;
        case 'h':
            // Standard deviation
            value = sqrt((s.square_total - (s.total*s.total)/s.escape)/s.escape);
            break;
        case 'i':
            // Coefficient of variation
            value = sqrt((s.square_total - (s.total*s.total)/s.escape)/s.escape);
            value /= (s.total/s.escape);
            break;
        case 'j':
            // Fractal Dimension
            value = sqrt(s.square_total - (s.total*s.total)/s.escape)/(s.maximum - s.minimum);
            break;
        case 'k':
            value = s.es_sum1;
            break;
        case 'l':
            value = s.es_sum2;
            break;
        }
        if (m_abs_log) value = std::abs(std::log(value));
        long double scaled_value = value*m_scale;
        colour = colourFromValue(scaled_value);
    }
    return colour;
}

