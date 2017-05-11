//
// File:   Summary.cc
// Author: M.R.Eggleston
//
// Created on 21 September 2011, 13:36
//

#include <limits>

#include "CriticalPointFinder.h"
#include "Summary.h"
#include "KeyString.h"

boost::shared_ptr<Summary> Summary::s_instance;

Summary::Summary()
:
m_picture(nullptr)
{
    loadFormulae();
    loadColourMethods();
    loadBailoutMaps();
}

Summary::~Summary()
{
}

boost::shared_ptr<Summary> Summary::create()
{
    if (!s_instance)
    {
        s_instance = boost::shared_ptr<Summary>(new Summary());
    }
    return s_instance;
}

boost::shared_ptr<Summary> Summary::instance()
{
    return s_instance;
}

const QString &Summary::get(Picture* picture)
{
    m_picture = picture;
    m_config = picture->getFractalConfig();
    auto ft = m_config.GetFractalType();
    m_str = "Triton Version 1.1.0\n\n";
    m_str += "Formula: " + fractalTypeToString(ft) + "\n\n";
    m_str += getFormula(ft);
    parameterSummary();
    criticalPointSummary();
    return bailoutSummary();
}

const QString &Summary::bailoutSummary()
{
    m_str += "Image centre: " + complexNumberToString(m_config.getImageCentre()) + "\n";
    m_str += "Image width: " + QString::fromStdString(m_config.getImageWidth()) + "\n";
    m_str += "Rotation about image centre: " + QString::number(m_config.getRotation()) + " degrees\n";
    if (!m_config.isOrbitPlotting())
    {
        m_str += "Maximum iterations: " + QString::number(m_config.getMaxIterations()) + "\n";
    }
    BAILOUT type = m_config.getBailout();
    long double limit = m_config.getLimit();
    m_str += "Bailout: " + bailout(type, limit) + "\n";
    if (m_config.isOrbitPlotting())
    {
        char method = m_config.getOrbitColourType();
        m_str += "Orbit length: " + QString::number(m_config.getMaxIterations()) + "\n";
        int omit = m_config.getStartOrbitIteration();
        if (omit > 0)
        {
            m_str += "Omit plotting first " + QString::number(omit) + " iteration" + (omit > 1 ? "s\n" : "\n");
        }
        int density = m_config.getDensity();
        if (density > 1)
        {
            m_str += "Plot density: " + QString::number(density) + "\n";
        }
        orbitPlot();
        m_str += "Colour method: ";
        if (method == 'a')
        {
            m_str += "accumulation, (the number of times an orbit visits a location)\n";
        }
        else
        {
            if (method == 'b')
            {
                m_str += "sqrt, scale = " + longdoubleToQString(m_config.getOrbitScale()) + "\n";
            }
            else if (method == 'c')
            {
                m_str += "average, interval " + QString::number(m_config.getInterval()) + "\n";
            }
            else if (method == 'd')
            {
                m_str += "logarithm, scale = " + longdoubleToQString(m_config.getOrbitScale()) + "\n";
            }
        }
    }
    else
    {
        m_str += "Colour selection: " + colourSelection() + "\n";
        m_str += "Outer areas: " + outerAreas() + "\n";
        if (!m_config.isOrbitPlotting())
        {
            m_str += "Inner/outer overlap: " + overlap() + "\n";
        }
        m_str += "Inner areas: average colour data\n";
        m_str += "Outer colouring: " + colouringMethod(false) + "\n";
        m_str += "Inner colouring: " + colouringMethod(true) + "\n";
        m_str += orbitTrap(false);
        m_str += orbitTrap(true);
    }
    return m_str;
}

void Summary::orbitPlot()
{
    switch (m_config.getOrbitPlot())
    {
    case OP_ALL:
    default:
        m_str += "Plot all orbits";
        break;
    case OP_ESCAPED:
        m_str += "Plot escaped orbits only";
        break;
    case OP_CAPTIVE:
        m_str += "Plot captive orbits only";
        break;
    }
    m_str += "\n";
}

QString Summary::getFormula(FRACTAL_TYPE ft)
{
    bool exists = (m_formulae.count(ft) != 0);
    QString formula = (exists ? m_formulae[ft] : "To be defined\n\n");
    return formula;
}

void Summary::loadFormulae()
{
    m_formulae[F_MC_4_3_2_1] = "z = alpha*z^4 + beta*z^3 + gamma*z^2 + delta*z + c\n\n";
    m_formulae[F_MC_4_3] = "z = alpha*z^4 + beta*z^3 + c\n\n";
    m_formulae[F_MC_4_2] = "z = alpha*z^4 + beta*z^2 + c\n\n";
    m_formulae[F_MC_5_4_3_2_1] = "z = alpha*z^5 + z^4 + beta*z^3 + delta*z^2 + epsilonz + c\n\n";
    m_formulae[F_MC_SIN] = "z = sin(z) + c\n\n";
    m_formulae[F_MC_3_2_1] = "z = alpha*z^3 + beta*z^2 + gamma*z + c\n\n";
    m_formulae[F_MC_4_1] = "z = alpha*z^4 + beta*z + c\n\n";
    m_formulae[F_MC_3_1] = "z = alpha*z^3 + beta*z + c\n\n";
    m_formulae[F_MC_3_2] = "z = alpha*z^3 + beta*z^2 + c\n\n";
    m_formulae[F_MC_4_3_2] = "z = alpha*z^4 + beta*z^3 + gamma*z^2 + delta*z + c\n\n";
    m_formulae[F_MC_5_1] = "z = alpha*z^5 + beta*z + c\n\n";
    m_formulae[F_MC_5_2] = "z = alpha*z^5 + beta*z^2 + c\n\n";
    m_formulae[F_MC_5_3] = "z = alpha*z^5 + beta*z^3 + c\n\n";
    m_formulae[F_MC_5_4] = "z = alpha*z^5 + beta*z^4 + c\n\n";
    m_formulae[F_MC_5_4_3] = "z = alpha*z^5 + beta*z^4 + gamma*z^3 + c\n\n";
    m_formulae[F_MC_6_1] = "z = alpha*z^6 + beta*z + c\n\n";
    m_formulae[F_MC_6_2] = "z = alpha*z^6 + beta*z^2 + c\n\n";
    m_formulae[F_MC_6_3] = "z = alpha*z^6 + beta*z^3 + c\n\n";
    m_formulae[F_MC_6_4] = "z = alpha*z^6 + beta*z^4 + c\n\n";
    m_formulae[F_MC_6_5] = "z = alpha*z^6 + beta*z^5 + c\n\n";
    m_formulae[F_MC_6_5_4] = "z = alpha*z^6 + beta*z^5 + gamma*z^4 + c\n\n";
    m_formulae[F_MC_6_5_4_3_2_1] = "z = alpha*z^6 + beta*z^5 + gamma*z^4 + delta*z^3 + epsilon*z^2 + zeta*z + c\n\n";
    m_formulae[F_MC_7_1] = "z = alpha*z^7 + beta*z + c\n\n";
    m_formulae[F_MC_7_2] = "z = alpha*z^7 + beta*z^2 + c\n\n";
    m_formulae[F_MC_7_3] = "z = alpha*z^7 + beta*z^3 + c\n\n";
    m_formulae[F_MC_7_4] = "z = alpha*z^7 + beta*z^4 + c\n\n";
    m_formulae[F_MC_7_5] = "z = alpha*z^7 + beta*z^5 + c\n\n";
    m_formulae[F_MC_7_6] = "z = alpha*z^7 + beta*z^6 + c\n\n";
    m_formulae[F_MC_7_6_5] = "z = alpha*z^7 + beta*z^6 + gamma*z^5 + c\n\n";
    m_formulae[F_MC_7_6_5_4_3_2_1] = "z = alpha*z^7 + beta*z^6 + gamma*z^5 + delta*z^4 + epsilon*z^3 + zeta*z^2 + eta*z + c\n\n";
    m_formulae[F_MC_2_M2] = "z = alpha*z^2 + beta*z^-2 + c\n\n";
    m_formulae[F_MC_2_M3] = "z = alpha*z^2 + beta*z^-3 + c\n\n";
    m_formulae[F_MC_2_M4] = "z = alpha*z^2 + beta*z^-4 + c\n\n";
    m_formulae[F_MC_3_M2] = "z = alpha*z^3 + beta*z^-2 + c\n\n";
    m_formulae[F_MC_3_M3] = "z = alpha*z^3 + beta*z^-3 + c\n\n";
    m_formulae[F_MC_3_M4] = "z = alpha*z^3 + beta*z^-4 + c\n\n";
    m_formulae[F_MC_4_M2] = "z = alpha*z^4 + beta*z^-2 + c\n\n";
    m_formulae[F_MC_4_M3] = "z = alpha*z^4 + beta*z^-3 + c\n\n";
    m_formulae[F_MC_4_M4] = "z = alpha*z^2 + beta*z^-2 + c\n\n";
    m_formulae[F_MC_P_C_2_M2] = "z = z + alpha\n"
                                "z = beta*z^2 + gamma*z^-2 + c\n\n";
    m_formulae[F_MC_P_C_2_M3] = "z = z + alpha\n"
                                "z = beta*z^2 + gamma*z^-3 + c\n\n";
    m_formulae[F_MC_P_C_2_M4] = "z = z + alpha\n"
                                "z = beta*z^2 + gamma*z^-4 + c\n\n";
    m_formulae[F_MC_P_C_3_M2] = "z = z + alpha\n"
                                "z = beta*z^3 + gamma*z^-2 + c\n\n";
    m_formulae[F_MC_P_C_3_M3] = "z = z + alpha\n"
                                "z = beta*z^3 + gamma*z^-3 + c\n\n";
    m_formulae[F_MC_P_C_3_M4] = "z = z + alpha\n"
                                "z = beta*z^3 + gamma*z^-4 + c\n\n";
    m_formulae[F_MC_P_C_4_M2] = "z = z + alpha\n"
                                "z = beta*z^4 + gamma*z^-2 + c\n\n";
    m_formulae[F_MC_P_C_4_M3] = "z = z + alpha\n"
                                "z = beta*z^4 + gamma*z^-3 + c\n\n";
    m_formulae[F_MC_P_C_4_M4] = "z = z + alpha\n"
                                "z = beta*z^4 + gamma*z^-4 + c\n\n";
    m_formulae[F_MC_8_1] = "z = alpha*z^8 + beta*z + c\n\n";
    m_formulae[F_MC_8_2] = "z = alpha*z^8 + beta*z^2 + c\n\n";
    m_formulae[F_MC_8_3] = "z = alpha*z^8 + beta*z^3 + c\n\n";
    m_formulae[F_MC_8_4] = "z = alpha*z^8 + beta*z^4 + c\n\n";
    m_formulae[F_MC_8_5] = "z = alpha*z^8 + beta*z^5 + c\n\n";
    m_formulae[F_MC_8_6] = "z = alpha*z^8 + beta*z^6 + c\n\n";
    m_formulae[F_MC_8_7] = "z = alpha*z^8 + beta*z^7 + c\n\n";
    m_formulae[F_MC_8_7_6] = "z = alpha*z^8 + beta*z^7 + gamma*z^6 + c\n\n";
    m_formulae[F_MC_8_7_6_5_4_3_2_1] =
                             "z = alpha*z^8 + beta*z^7 + gamma*z^6 + "
                                 "delta*z^5 + epsilon*z^4 + zeta*z^3 + "
                                 "eta*z^2 + theta*z + c\n\n";
}

void Summary::loadColourMethods()
{
    m_colour_method['i'] = "iteration";
    m_colour_method['f'] = "fixed colour";
    m_colour_method['a'] = "final angle";
    m_colour_method['m'] = "final magnitude";
    m_colour_method['A'] = "angle";
    m_colour_method['M'] = "magnitude";
    m_colour_method['g'] = "gaussian integer";
    m_colour_method['r'] = "ratio";
    m_colour_method['t'] = "distance to orbit trap";
    //
    m_gaussian_type['r'] = "(round)";
    m_gaussian_type['c'] = "(ceiling)";
    m_gaussian_type['f'] = "(floor)";
    m_gaussian_type['t'] = "(truncate)";
    //
    m_gaussian_statistic['a'] = "minimum distance to";
    m_gaussian_statistic['b'] = "iteration at minimum distance to";
    m_gaussian_statistic['c'] = "angle at minimum distance to";
    m_gaussian_statistic['d'] = "maximum distance to";
    m_gaussian_statistic['e'] = "iteration at maximum distance to";
    m_gaussian_statistic['f'] = "angle at maximum distance to";
    m_gaussian_statistic['g'] = "average distance to";
    m_gaussian_statistic['h'] = "minimum angle to";
    m_gaussian_statistic['i'] = "average angle to";
    m_gaussian_statistic['j'] = "maximum angle to";
    m_gaussian_statistic['k'] = "maximum/minimum ratio of distance to";
    m_gaussian_statistic['l'] = "range of distance to ";
    m_gaussian_statistic['m'] = "variance of distance to";
    m_gaussian_statistic['n'] = "standard deviation of distance to";
    m_gaussian_statistic['o'] = "coefficient of variation of distance to";
    //
    m_statistics_type['a'] = "minimum";
    m_statistics_type['b'] = "iteration at minimum";
    m_statistics_type['c'] = "maximum";
    m_statistics_type['d'] = "iteration at maximum";
    m_statistics_type['e'] = "range of";
    m_statistics_type['f'] = "average";
    m_statistics_type['g'] = "variance of";
    m_statistics_type['h'] = "standard deviation of";
    m_statistics_type['i'] = "coefficient of variation of";
    m_statistics_type['j'] = "fractal dimension of";
    m_statistics_type['k'] = "exponential sum of";
    m_statistics_type['l'] = "exponential inverse change sum of";
    //
    m_final_magnitude_option['n'] = "norm(z)";
    m_final_magnitude_option['s'] = "smaller of real(z) & imag(z)";
    m_final_magnitude_option['g'] = "greater of real(z) & imag(z)";
    m_final_magnitude_option['r'] = "real(z)";
    m_final_magnitude_option['R'] = "abs(real(z))";
    m_final_magnitude_option['i'] = "imag(z)";
    m_final_magnitude_option['I'] = "abs(imag(z))";
    m_final_magnitude_option['c'] = "real(z) + imag(z)";
    m_final_magnitude_option['C'] = "abs(real(z) + imag(z))";
    m_final_magnitude_option['p'] = "real(z)*imag(z)";
    m_final_magnitude_option['P'] = "abs(real(z)*imag(z))";
    //
    m_ratio_part['a'] = "smaller(real(z), imag(z))";
    m_ratio_part['b'] = "larger(real(z), imag(z))";
    m_ratio_part['c'] = "abs(real(z))";
    m_ratio_part['d'] = "abs(imag(z))";
    m_ratio_part['e'] = "abs(real(z) + imag(z))";
    m_ratio_part['f'] = "abs(real(z) - imag(z))";
    m_ratio_part['g'] = "abs(real(z)) + abs(imag(z))";
    m_ratio_part['h'] = "abs(abs(real(z)) - abs(imag)))";
    m_ratio_part['i'] = "abs(real(z)*imag(z))";
    m_ratio_part['j'] = "abs(z)";
    m_ratio_part['k'] = "norm(z)";
    //
    m_trap_name['a'] = "Point";
    m_trap_name['b'] = "4 Points";
    m_trap_name['c'] = "Line";
    m_trap_name['d'] = "Cross";
    m_trap_name['e'] = "Square";
    //m_trap_name['f'] = "Grid";
    //m_trap_name['g'] = "Mesh";
    m_trap_name['h'] = "Circle";
    m_trap_name['i'] = "Triangle";
    m_trap_name['j'] = "Triform";
    m_trap_name['k'] = "Asterisk";
    m_trap_name['l'] = "Circle Line";
    m_trap_name['m'] = "Circle Cross";
    m_trap_name['n'] = "Circle Triform";
    m_trap_name['o'] = "Two Quarter Circles";
    m_trap_name['p'] = "Circle Triangle";
    m_trap_name['q'] = "Triangle Circle";
    m_trap_name['r'] = "Circle Square";
    m_trap_name['s'] = "Square Circle";
    m_trap_name['t'] = "Octothorpe";
    m_trap_name['u'] = "Running Track";
    m_trap_name['v'] = "Pinch";
    m_trap_name['w'] = "Steiner Chain";
}

void Summary::loadBailoutMaps()
{
    m_gt_bailout[B_NONE] = "No Bailout";
    m_gt_bailout[B_ABS] = "abs(z)";
    m_gt_bailout[B_NORM] = "norm(z)";
    m_gt_bailout[B_IMAG] = "imag(z)";
    m_gt_bailout[B_IMAG_SQUARED] = "(imag(z))^2";
    m_gt_bailout[B_REAL] = "real(z)";
    m_gt_bailout[B_REAL_SQUARED] = "(real(z))^2";
    m_gt_bailout[B_ABS_PRODUCT] = "abs(real(z)*imag(z))";
    m_gt_bailout[B_MANHATTAN] = "abs(real(z)) + abs(imag(z))";
    m_gt_bailout[B_MANHATTANISH] = "real(z) + imag(z)";
    m_gt_bailout[B_ABS_MANHATTANISH] = "abs(real(z) + image(z))";
    m_gt_bailout[B_MAX_SQUARED] = "(max(real(z), imag(z)))^2";
    m_gt_bailout[B_MIN_SQUARED] = "(min(real(z), imag(z)))^2";
    m_gt_bailout[B_ABS_DIFF] = "abs(real(z) - imag(z))";
}

QString Summary::bailout(BAILOUT type, long double limit)
{
    BailoutMap &map = m_gt_bailout;
    bool exists = (map.count(type) != 0);
    QString str("unknown");
    if (exists)
    {
        str = map[type];
        if (type != B_NONE)
        {
            str += " > ";
            str += longdoubleToQString(limit);
        }
    }
    return str;
}

QString Summary::colourSelection()
{
    QString str;
    BAILOUT bailout = m_config.getBailout();
    if (bailout == B_NONE)
    {
        str += "all points are inner";
    }
    else
    {
        str += "outer for bailout otherwise inner";
    }
    return str;
}

QString Summary::outerAreas()
{
    QString str;
    COMPOUND_IMAGE compound = m_config.compoundImage();
    if (compound == CI_AVERAGE)
    {
        str += "average colour data";
    }
    else
    {
        str += "use colour data for highest iteration";
    }
    return str;
}

QString Summary::overlap()
{
    QString str;
    INNER_TYPE type = m_config.innerType();
    if (type == IT_OVERLAP)
    {
        str += "average inner and outer colours";
    }
    else if (type == IT_ONE_OR_MORE)
    {
        str += "inner colours only";
    }
    else
    {
        str += "outer colours only";
    }
    return str;
}

QString Summary::colouringMethod(bool inner)
{
    QString str;
    std::string method_str = m_config.getMethod(inner);
    char method = method_str[0];
    QString method_name = methodName(method);
    if (method == 'f')
    {
        str += m_colour_method[method];
    }
    else if (method == 'i')
    {
        if (m_config.isSmooth(inner))
        {
            str += "continuous potential";
        }
        else
        {
            str += method_name;
            unsigned int red_offset = m_config.getRedOffset(inner);
            unsigned int green_offset = m_config.getGreenOffset(inner);
            unsigned int blue_offset = m_config.getBlueOffset(inner);
            if (red_offset > 0)
            {
                str += ", red offset = " + QString::number(red_offset);
            }
            if (green_offset > 0)
            {
                str += ", green offset = " + QString::number(green_offset);
            }
            if (blue_offset > 0)
            {
                str += ", blue offset = " + QString::number(blue_offset);
            }
        }
    }
    else
    {
        if (m_config.isAbsLog(inner))
        {
            str += "absolute log of ";
        }
        if (method == 'a')
        {
            str += method_name;
            str += ", scale = " + longdoubleToQString(m_config.getScale(inner));
        }
        else if (method == 'm')
        {
            char option = method_str[1];
            str += method_name + ", " + finalMagnitudeOptionName(option);
            str += ", scale = " + longdoubleToQString(m_config.getScale(inner));
        }
        else if (method == 'A')
        {
            str += statistics(method_str[1]) + " ";
            if (method_str[1] < 'a') str += "change in ";
            str += method_name;
            if ((method_str[1] != 'b') && (method_str[1] !='d') &&
                (method_str[1] != 'B') && (method_str[1] !='D'))
            {
                str += ", scale = " + longdoubleToQString(m_config.getScale(inner));
            }
        }
        else if (method == 'M')
        {
            str += statistics(method_str[1]) + " ";
            if (method_str[1] < 'a') str += "change in ";
            str += method_name;
            if ((method_str[1] != 'b') && (method_str[1] !='d') &&
                (method_str[1] != 'B') && (method_str[1] !='D'))
            {
                str += ", scale = " + longdoubleToQString(m_config.getScale(inner));
            }
        }
        else if (method == 'g')
        {
            str += gaussianStatistic(method_str[2]) + " ";
            str += method_name + " " + gaussianType(method_str[1]);
            if ((method_str[2] != 'b') && (method_str[2] !='e') &&
               (method_str[2] != 'B') && (method_str[2] !='E'))
            {
                str += ", scale = " + longdoubleToQString(m_config.getScale(inner));
            }
        }
        else if (method == 't')
        {
            str += statistics(method_str[2]) + " ";
            if (method_str[2] < 'a') str += "change in ";
            str += method_name;
            if ((method_str[2] != 'b') && (method_str[2] !='d') &&
                (method_str[2] != 'B') && (method_str[2] !='D'))
            {
                str += ", scale = " + longdoubleToQString(m_config.getScale(inner));
            }
        }
        else if (method == 'r')
        {
            str += statistics(method_str[3]) + " ";
            if (method_str[3] < 'a') str += "change in ";
            str += method_name + ", ";
            str += ratio(method_str[1], method_str[2]);
            if ((method_str[3] != 'b') && (method_str[3] !='d') &&
                (method_str[3] != 'B') && (method_str[3] !='D'))
            {
                str += ", scale = " + longdoubleToQString(m_config.getScale(inner));
            }
        }
        unsigned int red_offset = m_config.getRedOffset(inner);
        unsigned int green_offset = m_config.getGreenOffset(inner);
        unsigned int blue_offset = m_config.getBlueOffset(inner);
        if (red_offset > 0)
        {
            str += ", red offset = " + QString::number(red_offset);
        }
        if (green_offset > 0)
        {
            str += ", green offset = " + QString::number(green_offset);
        }
        if (blue_offset > 0)
        {
            str += ", blue offset = " + QString::number(blue_offset);
        }
    }
    return str;
}

QString Summary::methodName(char value)
{
    QString str;
    if (m_colour_method.count(value) != 0)
    {
        str = m_colour_method[value];
    }
    return str;
}

QString Summary::finalMagnitudeOptionName(char value)
{
    QString str;
    if (m_final_magnitude_option.count(value) != 0)
    {
        str = m_final_magnitude_option[value];
    }
    return str;
}

QString Summary::statistics(char value)
{
    if (value < 'a') value = value - 'A' + 'a';
    QString str;
    if (m_statistics_type.count(value) != 0)
    {
        str = m_statistics_type[value];
    }
    return str;
}

QString Summary::gaussianType(char value)
{
    QString str;
    if (m_gaussian_type.count(value) != 0)
    {
        str = m_gaussian_type[value];
    }
    return str;
}

QString Summary::ratio(char q, char d)
{
    QString str;
    if ((m_ratio_part.count(q) != 0) && (m_ratio_part.count(d) != 0))
    {
        str += m_ratio_part[q] + " divided by " + m_ratio_part[d];
    }
    return str;
}

QString Summary::gaussianStatistic(char value)
{
    QString str;
    if (m_gaussian_statistic.count(value) != 0)
    {
        str = m_gaussian_statistic[value];
    }
    return str;
}

QString Summary::orbitTrap(bool inner)
{
    QString str;
    if (m_config.isOrbitTrap(inner))
    {
        std::string method_str = m_config.getMethod(inner);
        KeyString keys(method_str.substr(4));
        str += "\n";
        str += QString(inner ? "Inner Orbit Trap" : "Outer Orbit Trap") + "\n\n";
        str += "Name: " + trapName(method_str[1]) + "\n";
        StringComplex centre(keys.GetKey("r", "0"), keys.GetKey("i", "0"));
        str += "Position: " + complexNumberToString(centre) + "\n";
        OrbitTrap::TRAP_TYPE type = static_cast<OrbitTrap::TRAP_TYPE>(method_str[1] - 'a');
        auto trap = OrbitTrap::getTrap(type, inner);
        if (trap->hasSize())
        {
            str += "Size: " + QString::fromStdString(keys.GetKey("width", "0")) + "\n";
        }
        if (trap->hasRotation())
        {
            str += "Rotation: " + QString::fromStdString(keys.GetKey("rot", "0")) + "\n";
        }
        if (trap->hasCentrePoint())
        {
            str += "Centre Point: " + QString::fromStdString(keys.GetKey("cp", "0") == "1" ? "yes" : "no") + "\n";
        }
        if (type == OrbitTrap::STEINER_CHAIN)
        {
            str += "\nChain Construction:\n\n";
            str += "Number of chain circles: " + QString::fromStdString(keys.GetKey("n", "0")) + "\n";
            str += "Inversion distance: " + QString::fromStdString(keys.GetKey("invd", "0")) + "\n";
            str += "Outer Circle: " + QString(keys.GetKey("outer", "0") == "1" ? "yes" : "no") + "\n";
            str += "Inner Circle: " + QString(keys.GetKey("inner", "0") == "1" ? "yes" : "no") + "\n";
            str += "Chain Circle Triangles: " + QString(keys.GetKey("triangles", "0") == "1" ? "yes" : "no") + "\n";
            str += "Chain Circle Centre Point: " + QString(keys.GetKey("cccp", "0") == "1" ? "yes" : "no") + "\n";
        }
    }
    return str;
}

QString Summary::trapName(char value)
{
    QString str;
    if (m_trap_name.count(value) != 0)
    {
        str = m_trap_name[value];
    }
    return str;
}

void Summary::parameterSummary()
{
    int size = m_config.getNumberOfComplexParameters();
    bool blank_line = false;
    if (size > 0) parameter("alpha", 0);
    if (size > 1) parameter("beta", 1);
    if (size > 2) parameter("gamma", 2);
    if (size > 3) parameter("delta", 3);
    if (size > 4) parameter("epsilon", 4);
    if (size > 5) parameter("zeta", 5);
    if (size > 6) parameter("eta", 6);
    if (size > 7) parameter("iota", 7);
    blank_line = blank_line || (size > 0);
    if (blank_line) m_str += "\n";
}

void Summary::criticalPointSummary()
{
    m_str += "Critical points:\n";
    CriticalPointFinder<LongComplex> cp_finder(m_config);
    auto points = cp_finder.find();
    for (auto i = 0U; i < points; i++)
    {
        m_str += longComplexToString(cp_finder[i]) + "\n";
    }
    m_str += "\n";
}

void Summary::parameter(const QString& name, int index)
{
    m_str += name;
    m_str += " = ";
    m_str += complexNumberToString(m_config.getComplexParameter(index)) + "\n";
}
