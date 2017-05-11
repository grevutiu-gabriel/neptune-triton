//
// File:   FractalsConfig.cc
// Author: M.R.Eggleston
//
// Created on 28 January 2014, 11:25
//

#include <iostream>
#include <limits>
#include "Config.h"
#include "Utilities.h"
#include "FractalsConfig.h"
#include "KeyString.h"
#include "Utilities.h"
#include "TypesAndConstants.h"

FractalsConfig::QStringMap FractalsConfig::m_formula;
FractalsConfig::QStringMap FractalsConfig::m_name;
FractalsConfig::StringComplexMap FractalsConfig::m_scentre;
FractalsConfig::StdStringMap FractalsConfig::m_swidth;
FractalsConfig::StringComplexMap FractalsConfig::m_ccentre;
FractalsConfig::StdStringMap FractalsConfig::m_cwidth;
FractalsConfig::StringComplexMap FractalsConfig::m_default_scentre;
FractalsConfig::StdStringMap FractalsConfig::m_default_swidth;
FractalsConfig::LongDoubleMap FractalsConfig::m_limit;
FractalsConfig::QStringMap FractalsConfig::m_colour_map[2];
FractalsConfig::UintMap FractalsConfig::m_red_offset[2];
FractalsConfig::UintMap FractalsConfig::m_green_offset[2];
FractalsConfig::UintMap FractalsConfig::m_blue_offset[2];
FractalsConfig::StdStringMap FractalsConfig::m_method[2];
FractalsConfig::LongDoubleMap FractalsConfig::m_colour_scale[2];
FractalsConfig::IntMap FractalsConfig::m_max_iterations;
FractalsConfig::IntMap FractalsConfig::m_default_max_iterations;
FractalsConfig::UintMap FractalsConfig::m_iteration_step_size;
FractalsConfig::BoolMap FractalsConfig::m_smooth[2];
FractalsConfig::BoolMap FractalsConfig::m_abs_log[2];
FractalsConfig::IntMap FractalsConfig::m_srotation;
FractalsConfig::IntMap FractalsConfig::m_crotation;
long double FractalsConfig::m_width = 750.0L;
long double FractalsConfig::m_height = 500.0L;
FractalsConfig::IntMap FractalsConfig::m_interval;
FractalsConfig::CharMap FractalsConfig::m_orbit_colour_type;
FractalsConfig::UintMap FractalsConfig::m_default_interval;
FractalsConfig::CharMap FractalsConfig::m_default_orbit_colour_type;
FractalsConfig::BailoutMap FractalsConfig::m_default_bailout;
FractalsConfig::BailoutMap FractalsConfig::m_bailout;
FractalsConfig::BoolMap FractalsConfig::m_orbit_plotting;
FractalsConfig::QStringMap FractalsConfig::m_orbit_colour;
FractalsConfig::UintMap FractalsConfig::m_red_orbit_offset;
FractalsConfig::UintMap FractalsConfig::m_green_orbit_offset;
FractalsConfig::UintMap FractalsConfig::m_blue_orbit_offset;
FractalsConfig::IntMap FractalsConfig::m_calculating_area;
FractalsConfig::LongDoubleMap FractalsConfig::m_orbit_scale;
FractalsConfig::OrbitPlotsMap FractalsConfig::m_orbit_plots;
FractalsConfig::IntMap FractalsConfig::m_start_orbit_iteration;
FractalsConfig::ComponentOrderMap FractalsConfig::m_order[2];
FractalsConfig::ComponentOrderMap FractalsConfig::m_orbit_order;
FractalsConfig::UintMap FractalsConfig::m_density;
FractalsConfig::BoolMap FractalsConfig::m_has_orbit_background;
FractalsConfig::QColorMap FractalsConfig::m_background_colour;
FractalsConfig::IntMap FractalsConfig::m_precision;
FractalsConfig::VectorUIntMap FractalsConfig::m_critical_point;
FractalsConfig::UintMap FractalsConfig::m_number_of_critical_points;
FractalsConfig::ParameterMapMap FractalsConfig::m_parameters;
FractalsConfig::ParameterMapMap FractalsConfig::m_default_parameters;
FractalsConfig::UintMap FractalsConfig::m_number_of_parameters;
FractalConfig FractalsConfig::m_fractal;
FractalsConfig::InnerTypeMap FractalsConfig::m_inner_type;
FractalsConfig::CompoundImageMap FractalsConfig::m_compound_image;

FractalsConfig::FractalsConfig()
:
m_type(F_MC_4_3_2_1),
m_index(0),
m_colour_maps(ColourMaps::instance())
{
    if (m_name.empty())
    {
        defineMc4_3_2_1();
        defineMc4_3();
        defineMc4_2();
        defineMc5_4_3_2_1();
        defineMcSin();
        defineMc3_2_1();
        defineMc4_1();
        defineMc3_1();
        defineMc3_2();
        defineMc4_3_2();
        defineMc5_1();
        defineMc5_2();
        defineMc5_3();
        defineMc5_4();
        defineMc5_4_3();
        defineMc6_1();
        defineMc6_2();
        defineMc6_3();
        defineMc6_4();
        defineMc6_5();
        defineMc6_5_4();
        defineMc6_5_4_3_2_1();
        defineMc7_1();
        defineMc7_2();
        defineMc7_3();
        defineMc7_4();
        defineMc7_5();
        defineMc7_6();
        defineMc7_6_5();
        defineMc7_6_5_4_3_2_1();
        defineMc2_m2();
        defineMc2_m3();
        defineMc2_m4();
        defineMc3_m2();
        defineMc3_m3();
        defineMc3_m4();
        defineMc4_m2();
        defineMc4_m3();
        defineMc4_m4();
        defineMcp2_m2();
        defineMcp2_m3();
        defineMcp2_m4();
        defineMcp3_m2();
        defineMcp3_m3();
        defineMcp3_m4();
        defineMcp4_m2();
        defineMcp4_m3();
        defineMcp4_m4();
        defineMcp3_m1();
        defineMcp4_m1();
        defineMc8_1();
        defineMc8_2();
        defineMc8_3();
        defineMc8_4();
        defineMc8_5();
        defineMc8_6();
        defineMc8_7();
        defineMc8_7_6();
        defineMc8_7_6_5_4_3_2_1();
    }
}

void FractalsConfig::SetFractalType(FRACTAL_TYPE value)
{
    m_type = value;
}

void FractalsConfig::SetFractal(FractalConfig &f)
{
    setWidth(f.getWidth());
    setHeight(f.getHeight());
    SetFractalType(f.GetFractalType());
    setCentre(f.getImageCentre());
    setCurrentWidth(f.getImageWidth());
    setRotation(f.getRotation());
    auto starting_width = f.getStartingWidth();
    if (!starting_width.empty())
    {
        // If FractalConfig object was loaded from a seed file it won't
        // have starting location data.
        setStartingCentre(f.getStartingCentre());
        setStartingWidth(f.getStartingWidth());
        setStartingRotation(f.getStartingRotation());
    }
    setCalculatingArea(f.getArea());
    setBailout(f.getBailout());
    setLimit(f.getLimit());

    int parameters = getNumberOfComplexParameters();
    for (int i = 0; i < parameters; i++)
    {
        setComplexParameter(i, f.getComplexParameter(i));
    }
    setOrbitPlotting(f.isOrbitPlotting());
    if (isOrbitPlotting())
    {
        setStartOrbitIteration(f.getStartOrbitIteration());
        setOrbitPlots(f.getOrbitPlot());
        setDensity(f.getDensity());
        setOrbitColourInterval(f.getOrbitColourInterval());
        setOrbitColourType(f.getOrbitColourType());
        setColourMap(f.getOrbitColourMap(), false);
        setOrbitScale(f.getOrbitScale());
        QString name = f.getOrbitColourMap();
        auto map = f.getColourMap(false);
        if (map.empty())
        {
            // We only have the map's name as the FractalConfig object
            // was created from a <fractal> child in the config file
            bool exists = m_colour_maps->nameExists(name);
            setColourMap(exists ? name : "Default");
        }
        else
        {
            // FractalConfig created from a seed file or an npf file
            setColourMap(m_colour_maps->setColours(name, map, true));
        }
        setRedOffset(f.getOrbitRedOffset());
        setGreenOffset(f.getOrbitGreenOffset());
        setBlueOffset(f.getOrbitBlueOffset());
        setOrbitBackground(f.getOrbitBackground());
        enableOrbitBackground(f.hasOrbitBackground());
        setComponentOrder(f.getComponentOrder(false));
    }
    else
    {
        for (int i = 0; i < 2; i++)
        {
            bool inner = (i == 1);
            setMethod(inner, f.getMethod(inner));
            QString name = f.getColourMapName(inner);
            auto map = f.getColourMap(inner);
            if (f.getMethod(inner)[0] != 'f')
            {
                if (map.empty())
                {
                    // We only have the map's name as the FractalConfig object
                    // was created from a <fractal> child in the config file
                    bool exists = m_colour_maps->nameExists(name);
                    setColourMap(exists ? name : "Default", inner);
                }
                else
                {
                    // FractalConfig created from a seed file or an npf file
                    setColourMap(m_colour_maps->setColours(name, map, true), inner);
                }
            }
            setColourScale(inner, f.getScale(inner));
            setSmooth(inner, f.getSmooth(inner));
            setAbsLog(inner, f.getAbsLog(inner));
            setRedOffset(f.getRedOffset(inner), inner);
            setGreenOffset(f.getGreenOffset(inner), inner);
            setBlueOffset(f.getBlueOffset(inner), inner);
            setComponentOrder(f.getComponentOrder(inner), inner);
        }
    }
    setMaxIterations(f.getMaxIterations());
    //
    setPrecision(f.getPrecision());
    int points = f.getNumberOfCriticalPoints();
    for (int i = 0; i < points; i++)
    {
        setIgnoreCriticalPoint(i, f.ignoreCriticalPoint(i));
    }
    setInnerType(f.innerType());
    setCompoundImage(f.compoundImage());
}

FRACTAL_TYPE FractalsConfig::GetFractalType()
{
    return m_type;
}

const QString &FractalsConfig::getFormula()
{
    return m_formula[m_type];
}

const QString &FractalsConfig::getName()
{
    return m_name[m_type];
}

void FractalsConfig::setStartingCentre(const StringComplex &value)
{
    m_scentre[m_type] = value;
}

void FractalsConfig::setStartingWidth(const std::string &value)
{
    m_swidth[m_type] = value;
}

void FractalsConfig::setStartingRotation(const unsigned int value)
{
    m_srotation[m_type] = value;
}

const StringComplex &FractalsConfig::getStartingCentre()
{
    return m_scentre[m_type];
}

const std::string &FractalsConfig::getStartingWidth()
{
    return m_swidth[m_type];
}

int FractalsConfig::getStartingRotation()
{
    return m_srotation[m_type];
}

void FractalsConfig::setCentre(const StringComplex &value)
{
    m_ccentre[m_type] = value;
}

void FractalsConfig::setCentre(const StringComplex &value, bool& changed)
{
    changed = (value.r != m_ccentre[m_type].r) ||
              (value.i != m_ccentre[m_type].i);
    if (changed)
    {
        m_ccentre[m_type] = value;
    }
}

void FractalsConfig::setCurrentWidth(const std::string &value)
{
    m_cwidth[m_type] = value;
}

void FractalsConfig::setRotation(const int value)
{
    m_crotation[m_type] = value;
}

const StringComplex &FractalsConfig::getCentre()
{
    return m_ccentre[m_type];
}

const std::string &FractalsConfig::getCurrentWidth()
{
    return m_cwidth[m_type];
}

int FractalsConfig::getRotation()
{
    return m_crotation[m_type];
}

void FractalsConfig::setColourMap(const QString& id)
{
    if (isOrbitPlotting())
    {
        m_orbit_colour[m_type] = id;
    }
    else
    {
        m_colour_map[m_index][m_type] = id;
    }
}

void FractalsConfig::setColourMap(const QString& id, bool inner)
{
    if (isOrbitPlotting())
    {
        m_orbit_colour[m_type] = id;
    }
    else
    {
        m_colour_map[(inner ? 1 : 0)][m_type] = id;
    }
}

QString &FractalsConfig::getColourMap()
{
    return (isOrbitPlotting() ? m_orbit_colour[m_type] : m_colour_map[m_index][m_type]);
}

QString &FractalsConfig::getColourMap(bool inner)
{
    return m_colour_map[(inner ? 1 : 0)][m_type];
}

void FractalsConfig::setMethod(const std::string& method)
{
    m_method[m_index][m_type] = method;
}

void FractalsConfig::setMethod(bool inner, const std::string& method)
{
    m_method[(inner ? 1 : 0)][m_type] = method;
}

const std::string &FractalsConfig::getMethod()
{
    return m_method[m_index][m_type];
}

const std::string &FractalsConfig::getMethod(bool inner)
{
    return m_method[(inner ? 1 : 0)][m_type];
}

void FractalsConfig::setColourScale(long double scale)
{
    m_colour_scale[m_index][m_type] = scale;
}

void FractalsConfig::setColourScale(bool inner, long double scale)
{
    m_colour_scale[(inner ? 1 : 0)][m_type] = scale;
}

long double FractalsConfig::getColourScale()
{
    return m_colour_scale[m_index][m_type];
}

void FractalsConfig::setMaxIterations(int value)
{
    m_max_iterations[m_type] = value;
}

int FractalsConfig::getMaxIterations()
{
    return m_max_iterations[m_type];
}

void FractalsConfig::setDefaultMaxIterations()
{
    m_max_iterations[m_type] = m_default_max_iterations[m_type];
}

unsigned int FractalsConfig::getIterationStepSize()
{
    return m_iteration_step_size[m_type];
}

void FractalsConfig::defaultStartingComplexPlane()
{
    m_scentre[m_type] = m_default_scentre[m_type];
    m_swidth[m_type] = m_default_swidth[m_type];
    m_srotation[m_type] = 0;
}

void FractalsConfig::setWidth(unsigned int value)
{
    if (value != 0)
    {
        m_width = static_cast<long double>(value);
    }
}

long double FractalsConfig::width()
{
    return m_width;
}

void FractalsConfig::setHeight(unsigned int value)
{
    if (value != 0)
    {
        m_height = static_cast<long double>(value);
    }
}

long double FractalsConfig::height()
{
    return m_height;
}

FractalsConfig::VectorFractalId &FractalsConfig::getAllFractals()
{
    FractalId id;
    m_fractal_ids.clear();
    for (auto it=m_name.begin(); it != m_name.end(); it++)
    {
        id.type = it->first;
        id.name = it->second;
        m_fractal_ids.push_back(id);
    }
    return m_fractal_ids;
}

void FractalsConfig::setLimit(long double value)
{
    m_limit[m_type] = value;
}

long double FractalsConfig::getLimit()
{
    return m_limit[m_type];
}

void FractalsConfig::setSmooth(bool value)
{
    m_smooth[m_index][m_type] = value;
}

void FractalsConfig::setSmooth(bool inner, bool value)
{
    m_smooth[(inner ? 1 : 0)][m_type] = value;
}

bool FractalsConfig::getSmooth()
{
    return m_smooth[m_index][m_type];
}

void FractalsConfig::setAbsLog(bool value)
{
    m_abs_log[m_index][m_type] = value;
}

void FractalsConfig::setAbsLog(bool inner, bool value)
{
    m_abs_log[(inner ? 1 : 0)][m_type] = value;
}

bool FractalsConfig::getAbsLog()
{
    return m_abs_log[m_index][m_type];
}

int FractalsConfig::getOrbitColourInterval()
{
    return m_interval[m_type];
}

char FractalsConfig::getOrbitColourType()
{
    return m_orbit_colour_type[m_type];
}

void FractalsConfig::setOrbitColourInterval(unsigned int value)
{
    m_interval[m_type] = value;
}

void FractalsConfig::setOrbitColourType(char value)
{
    m_orbit_colour_type[m_type] = value;
}

void FractalsConfig::setInner(bool inner)
{
    m_index = (inner ? 1 : 0);
}

bool FractalsConfig::isInner()
{
    return (m_index == 1);
}

void FractalsConfig::setBailout(BAILOUT value)
{
    m_bailout[m_type] = value;
}

BAILOUT FractalsConfig::getBailout()
{
    return m_bailout[m_type];
}

void FractalsConfig::setCommon(const StringComplex& centre,
                               const std::string& side,
                               int iterations,
                               unsigned int cp)
{
    m_name[m_type] = fractalTypeToString(m_type);
    m_default_scentre[m_type] = centre;
    m_scentre[m_type] = centre;
    m_ccentre[m_type] = centre;
    m_ccentre[m_type] = centre;
    m_default_swidth[m_type] = side;
    m_swidth[m_type] = side;
    m_cwidth[m_type] = side;
    defineBailout(B_NORM);
    m_bailout[m_type] = m_default_bailout[m_type];
    m_colour_map[0][m_type] = "Default";
    m_order[0][m_type] = CO_RGB;
    m_red_offset[0][m_type] = 0;
    m_green_offset[0][m_type] = 0;
    m_blue_offset[0][m_type] = 0;
    m_method[0][m_type] = "i";
    m_colour_map[1][m_type] = "Default";
    m_order[1][m_type] = CO_RGB;
    m_red_offset[1][m_type] = 0;
    m_green_offset[1][m_type] = 0;
    m_blue_offset[1][m_type] = 0;
    m_method[1][m_type] = "f,0,0,0";
    m_limit[m_type] = 16.0L;
    m_max_iterations[m_type] = iterations;
    m_default_max_iterations[m_type] = iterations;
    m_default_interval[m_type] = 32000;
    m_default_orbit_colour_type[m_type] = 'a';
    m_interval[m_type] = m_default_interval[m_type];
    m_orbit_colour_type[m_type] = m_default_orbit_colour_type[m_type];
    m_calculating_area[m_type] = 9;
    m_density[m_type] = 1;
    m_number_of_parameters[m_type] = 0;
    m_inner_type[m_type] = IT_OVERLAP;
    m_compound_image[m_type] = CI_AVERAGE;
    m_number_of_critical_points[m_type] = cp;
    m_critical_point[m_type].resize(cp);
}

void FractalsConfig::defineBailout(BAILOUT type)
{
    m_default_bailout[m_type] = type;
    m_bailout[m_type] = type;
}
void FractalsConfig::defineParameter(int index, const StringComplex& value)
{
    m_parameters[m_type][index] = value;
    m_default_parameters[m_type][index] = value;
}

void FractalsConfig::defineMc4_3_2_1()
{
    m_type = F_MC_4_3_2_1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>4</sup> + βz<sup>3</sup> + γz<sup>2</sup> + δz + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 3);
    m_number_of_parameters[m_type] = 4;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
    defineParameter(3, StringComplex("1", "0"));
}

void FractalsConfig::defineMc4_3()
{
    m_type = F_MC_4_3;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>4</sup> + βz<sup>3</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 2);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc4_2()
{
    m_type = F_MC_4_2;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>4</sup> + βz<sup>2</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 3);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc5_4_3_2_1()
{
    m_type = F_MC_5_4_3_2_1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>5</sup> + βz<sup>4</sup> + γz<sup>3</sup> + δz<sup>2</sup> + εz + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 4);
    m_number_of_parameters[m_type] = 5;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
    defineParameter(3, StringComplex("1", "0"));
    defineParameter(4, StringComplex("1", "0"));
}

void FractalsConfig::defineMcSin()
{
    m_type = F_MC_SIN;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = sin(z) + c</big></b>");
    setCommon(StringComplex(), "16", 2000, 2);
    m_limit[m_type] = 160.0L;
}

void FractalsConfig::defineMc3_2_1()
{
    m_type = F_MC_3_2_1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>3</sup> + βz<sup>2</sup> + γz + c");
    setCommon(StringComplex(), "4", 2000, 2);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMc4_1()
{
    m_type = F_MC_4_1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>4</sup> + βz + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 3);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc3_1()
{
    m_type = F_MC_3_1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>3</sup> + βz + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 2);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc3_2()
{
    m_type = F_MC_3_2;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>3</sup> + βz<sup>2</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 2);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc4_3_2()
{
    m_type = F_MC_4_3_2;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>4</sup> + βz<sup>3</sup> + γz<sup>2</sup> + c");
    setCommon(StringComplex(), "4", 2000, 3);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMc5_1()
{
    m_type = F_MC_5_1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>5</sup> + βz + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 4);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc5_2()
{
    m_type = F_MC_5_2;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>5</sup> + βz<sup>2</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 4);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc5_3()
{
    m_type = F_MC_5_3;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>5</sup> + βz<sup>3</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 3);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc5_4()
{
    m_type = F_MC_5_4;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>5</sup> + βz<sup>3</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 2);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc5_4_3()
{
    m_type = F_MC_5_4_3;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>5</sup> + βz<sup>4</sup> + γz<sup>3</sup> + c");
    setCommon(StringComplex(), "4", 2000, 3);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMc6_1()
{
    m_type = F_MC_6_1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>6</sup> + βz + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 5);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc6_2()
{
    m_type = F_MC_6_2;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>6</sup> + βz<sup>2</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 5);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc6_3()
{
    m_type = F_MC_6_3;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>6</sup> + βz<sup>3</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 4);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc6_4()
{
    m_type = F_MC_6_4;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>6</sup> + βz<sup>4</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 3);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc6_5()
{
    m_type = F_MC_6_5;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>6</sup> + βz<sup>5</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 2);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc6_5_4()
{
    m_type = F_MC_6_5_4;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>6</sup> + βz<sup>5</sup> + γz<sup>4</sup> + c");
    setCommon(StringComplex(), "4", 2000, 3);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMc6_5_4_3_2_1()
{
    m_type = F_MC_6_5_4_3_2_1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>6</sup> + βz<sup>5</sup> + γz<sup>4</sup> +"
                    "δz<sup>3</sup> + εz<sup>2</sup> + ζz + c");
    setCommon(StringComplex(), "4", 2000, 5);
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
    defineParameter(3, StringComplex("1", "0"));
    defineParameter(4, StringComplex("1", "0"));
    defineParameter(5, StringComplex("1", "0"));
    m_number_of_parameters[m_type] = 6;
}

void FractalsConfig::defineMc7_1()
{
    m_type = F_MC_7_1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>7</sup> + βz + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 6);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc7_2()
{
    m_type = F_MC_7_2;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>7</sup> + βz<sup>2</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 6);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc7_3()
{
    m_type = F_MC_7_3;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>7</sup> + βz<sup>3</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 5);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc7_4()
{
    m_type = F_MC_7_4;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>7</sup> + βz<sup>4</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 4);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc7_5()
{
    m_type = F_MC_7_5;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>7</sup> + βz<sup>5</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 3);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc7_6()
{
    m_type = F_MC_7_6;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>7</sup> + βz<sup>6</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 2);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc7_6_5()
{
    m_type = F_MC_7_6_5;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>7</sup> + βz<sup>6</sup> + γz<sup>5</sup> + c");
    setCommon(StringComplex(), "4", 2000, 3);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMc7_6_5_4_3_2_1()
{
    m_type = F_MC_7_6_5_4_3_2_1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>7</sup> + βz<sup>6</sup> + γz<sup>5</sup> +"
                    "δz<sup>4</sup> + εz<sup>3</sup> + ζz<sup>2</sup> + ηz + c");
    setCommon(StringComplex(), "4", 2000, 6);
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
    defineParameter(3, StringComplex("1", "0"));
    defineParameter(4, StringComplex("1", "0"));
    defineParameter(5, StringComplex("1", "0"));
    defineParameter(6, StringComplex("1", "0"));
    m_number_of_parameters[m_type] = 7;
}

void FractalsConfig::defineMc2_m2()
{
    m_type = F_MC_2_M2;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>2</sup> + βz<sup>-2</sup> + c</big></b>");
    setCommon(StringComplex("-1", "0"), "8", 2000, 4);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc2_m3()
{
    m_type = F_MC_2_M3;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>2</sup> + βz<sup>-3</sup> + c</big></b>");
    setCommon(StringComplex("-1", "0"), "8", 2000, 5);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc2_m4()
{
    m_type = F_MC_2_M4;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>2</sup> + βz<sup>-4</sup> + c</big></b>");
    setCommon(StringComplex("-1", "0"), "8", 2000, 6);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc3_m2()
{
    m_type = F_MC_3_M2;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>3</sup> + βz<sup>-2</sup> + c</big></b>");
    setCommon(StringComplex("-1", "0"), "8", 2000, 5);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc3_m3()
{
    m_type = F_MC_3_M3;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>3</sup> + βz<sup>-3</sup> + c</big></b>");
    setCommon(StringComplex("-1", "0"), "8", 2000, 6);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc3_m4()
{
    m_type = F_MC_3_M4;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>3</sup> + βz<sup>-4</sup> + c</big></b>");
    setCommon(StringComplex("-1", "0"), "8", 2000, 7);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc4_m2()
{
    m_type = F_MC_4_M2;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>4</sup> + βz<sup>-2</sup> + c</big></b>");
    setCommon(StringComplex("-1", "0"), "8", 2000, 6);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc4_m3()
{
    m_type = F_MC_4_M3;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>4</sup> + βz<sup>-3</sup> + c</big></b>");
    setCommon(StringComplex("-1", "0"), "8", 2000, 7);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc4_m4()
{
    m_type = F_MC_4_M4;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>4</sup> + βz<sup>-4</sup> + c</big></b>");
    setCommon(StringComplex("-1", "0"), "8", 2000, 8);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMcp2_m2()
{
    m_type = F_MC_P_C_2_M2;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = z + α</big></b><br>"
        "<b><big>z = c(βz<sup>2</sup> + γz<sup>-2</sup>)</big></b>");
    setCommon(StringComplex("0", "0"), "4", 2000, 4);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMcp2_m3()
{
    m_type = F_MC_P_C_2_M3;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = z + α</big></b><br>"
        "<b><big>z = c(βz<sup>2</sup> + γz<sup>-3</sup>)</big></b>");
    setCommon(StringComplex("0", "0"), "4", 2000, 5);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMcp2_m4()
{
    m_type = F_MC_P_C_2_M4;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = z + α</big></b><br>"
        "<b><big>z = c(βz<sup>2</sup> + γz<sup>-4</sup>)</big></b>");
    setCommon(StringComplex("0", "0"), "4", 2000, 6);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMcp3_m2()
{
    m_type = F_MC_P_C_3_M2;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = z + α</big></b><br>"
        "<b><big>z = c(βz<sup>3</sup> + γz<sup>-2</sup>)</big></b>");
    setCommon(StringComplex("0", "0"), "4", 2000, 5);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMcp3_m3()
{
    m_type = F_MC_P_C_3_M3;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = z + α</big></b><br>"
        "<b><big>z = c(βz<sup>3</sup> + γz<sup>-3</sup>)</big></b>");
    setCommon(StringComplex("0", "0"), "4", 2000, 6);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMcp3_m4()
{
    m_type = F_MC_P_C_3_M4;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = z + α</big></b><br>"
        "<b><big>z = c(βz<sup>3</sup> + γz<sup>-4</sup>)</big></b>");
    setCommon(StringComplex("0", "0"), "4", 2000, 7);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMcp4_m2()
{
    m_type = F_MC_P_C_4_M2;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = z + α</big></b><br>"
        "<b><big>z = c(βz<sup>4</sup> + γz<sup>-2</sup>)</big></b>");
    setCommon(StringComplex("0", "0"), "4", 2000, 6);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMcp4_m3()
{
    m_type = F_MC_P_C_4_M3;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = z + α</big></b><br>"
        "<b><big>z = c(βz<sup>4</sup> + γz<sup>-3</sup>)</big></b>");
    setCommon(StringComplex("0", "0"), "4", 2000, 7);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMcp4_m4()
{
    m_type = F_MC_P_C_4_M4;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = z + α</big></b><br>"
        "<b><big>z = c(βz<sup>4</sup> + γz<sup>-4</sup>)</big></b>");
    setCommon(StringComplex("0", "0"), "4", 2000, 8);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMcp3_m1()
{
    m_type = F_MC_P_C_3_M1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = z + α</big></b><br>"
        "<b><big>z = c(βz<sup>3</sup> + γz<sup>-1</sup>)</big></b>");
    setCommon(StringComplex("0", "0"), "4", 2000, 4);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMcp4_m1()
{
    m_type = F_MC_P_C_4_M1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = z + α</big></b><br>"
        "<b><big>z = c(βz<sup>4</sup> + γz<sup>-1</sup>)</big></b>");
    setCommon(StringComplex("0", "0"), "4", 2000, 5);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMc8_1()
{
    m_type = F_MC_8_1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>8</sup> + βz + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 7);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc8_2()
{
    m_type = F_MC_8_2;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>8</sup> + βz<sup>2</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 7);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc8_3()
{
    m_type = F_MC_8_3;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>8</sup> + βz<sup>3</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 6);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc8_4()
{
    m_type = F_MC_8_4;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>8</sup> + βz<sup>4</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 5);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc8_5()
{
    m_type = F_MC_8_5;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>8</sup> + βz<sup>5</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 4);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc8_6()
{
    m_type = F_MC_8_6;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>8</sup> + βz<sup>6</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 3);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc8_7()
{
    m_type = F_MC_8_7;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>8</sup> + βz<sup>7</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 2);
    m_number_of_parameters[m_type] = 2;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
}

void FractalsConfig::defineMc8_7_6()
{
    m_type = F_MC_8_7_6;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>8</sup> + βz<sup>7</sup> + γz<sup>6</sup> + c</big></b>");
    setCommon(StringComplex(), "4", 2000, 3);
    m_number_of_parameters[m_type] = 3;
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
}

void FractalsConfig::defineMc8_7_6_5_4_3_2_1()
{
    m_type = F_MC_8_7_6_5_4_3_2_1;
    m_formula[m_type] = QString::fromUtf8(
        "<b><big>z = αz<sup>8</sup> + βz<sup>7</sup> + γz<sup>6</sup> + "
                    "δz<sup>5</sup> + εz<sup>4</sup> + ζz<sup>3</sup> + "
                    "ηz<sup>2</sup> + θz + c");
    setCommon(StringComplex(), "4", 2000, 7);
    defineParameter(0, StringComplex("1", "0"));
    defineParameter(1, StringComplex("1", "0"));
    defineParameter(2, StringComplex("1", "0"));
    defineParameter(3, StringComplex("1", "0"));
    defineParameter(4, StringComplex("1", "0"));
    defineParameter(5, StringComplex("1", "0"));
    defineParameter(6, StringComplex("1", "0"));
    defineParameter(7, StringComplex("1", "0"));
    m_number_of_parameters[m_type] = 8;
}


void FractalsConfig::setOrbitPlotting(bool plotting)
{
    m_orbit_plotting[m_type] = plotting;
}

bool FractalsConfig::isOrbitPlotting()
{
    return m_orbit_plotting[m_type];
}

void FractalsConfig::setCalculatingArea(unsigned int size)
{
    m_calculating_area[m_type] = size;
}

int FractalsConfig::getCalculatingArea()
{
    return m_calculating_area[m_type];
}

void FractalsConfig::setOrbitScale(long double scale)
{
    m_orbit_scale[m_type] = scale;
}

long double FractalsConfig::getOrbitScale()
{
    return (m_orbit_scale.count(m_type) == 0.0L ? 10.0L : m_orbit_scale[m_type]);
}

void FractalsConfig::setOrbitPlots(ORBIT_PLOTS value)
{
    m_orbit_plots[m_type] = value;
}

ORBIT_PLOTS FractalsConfig::getOrbitPlots()
{
    return ((m_orbit_plots.count(m_type) == 0) || !m_orbit_plotting[m_type] ? OP_ALL : m_orbit_plots[m_type]);
}

void FractalsConfig::setDensity(unsigned int value)
{
    m_density[m_type] = value;
}

unsigned int FractalsConfig::getDensity()
{
    return m_density[m_type];
}

int FractalsConfig::getWidth()
{
    return static_cast<int>(m_width);
}

int FractalsConfig::getHeight()
{
    return static_cast<int>(m_height);
}

void FractalsConfig::setStartOrbitIteration(int value)
{
    m_start_orbit_iteration[m_type] = value;
}

int FractalsConfig::getStartOrbitIteration()
{
    return m_start_orbit_iteration[m_type];
}

const FractalConfig &FractalsConfig::getFractalConfig(bool set_colour_maps)
{
    FractalConfig fractal(m_type);
    m_fractal = fractal;
    m_fractal.setName(getName());
    m_fractal.setImageCentre(getCentre());
    m_fractal.setImageWidth(getCurrentWidth());
    m_fractal.setRotation(getRotation());
    m_fractal.setLimit(getLimit());
    m_fractal.setBailout(getBailout());
    m_fractal.setNumberOfComplexParameters(getNumberOfComplexParameters());
    for (int i = 0; i < MAX_PARAMETERS; i++)
    {
        m_fractal.setComplexParameter(i, getComplexParameter(i));
    }
    m_fractal.setWidth(getWidth());
    m_fractal.setHeight(getHeight());
    m_fractal.setCentre(getCentre());
    setInner(false);
    m_fractal.setAbsLog(false, getAbsLog());
    m_fractal.setSmooth(false, getSmooth());
    m_fractal.setScale(false, this->getColourScale());
    m_fractal.setMethod(false, getMethod());
    if (set_colour_maps)
    {
        m_fractal.setColourMap(m_colour_maps->getColourMap(getColourMap()), false);
    }
    m_fractal.setRedOffset(getRedOffset(), false);
    m_fractal.setGreenOffset(getGreenOffset(), false);
    m_fractal.setBlueOffset(getBlueOffset(), false);
    m_fractal.setComponentOrder(getComponentOrder(false), false);
    setInner(true);
    m_fractal.setAbsLog(true, getAbsLog());
    m_fractal.setSmooth(true, getSmooth());
    m_fractal.setScale(true, getColourScale());
    m_fractal.setMethod(true, getMethod());
    if (set_colour_maps)
    {
        m_fractal.setColourMap(m_colour_maps->getColourMap(getColourMap()), true);
    }
    m_fractal.setRedOffset(getRedOffset(), true);
    m_fractal.setGreenOffset(getGreenOffset(), true);
    m_fractal.setBlueOffset(getBlueOffset(), true);
    m_fractal.setComponentOrder(getComponentOrder(true), true);
    // Orbit plotting
    m_fractal.setOrbitPlotting(isOrbitPlotting());
    if (isOrbitPlotting())
    {
        m_fractal.setOrbitPlot(getOrbitPlots());
        m_fractal.setOmit(getStartOrbitIteration());
        m_fractal.setArea(getCalculatingArea());
        m_fractal.setDensity(getDensity());
        m_fractal.setOrbitScale(getOrbitScale());
        m_fractal.setOrbitColourType(getOrbitColourType());
        m_fractal.setOrbitColourInterval(getOrbitColourInterval());
        m_fractal.setOrbitRedOffset(getRedOffset());
        m_fractal.setOrbitGreenOffset(getGreenOffset());
        m_fractal.setOrbitBlueOffset(getBlueOffset());
        m_fractal.setOrbitBackground(getOrbitBackground());
        m_fractal.enableOrbitBackground(hasOrbitBackground());
        m_fractal.setComponentOrder(getComponentOrder(), false);
    }
    m_fractal.setPrecision(getPrecision());
    int number_of_critical_points = m_number_of_critical_points[m_type];
    VectorUInt ignore_cp(number_of_critical_points);
    for (int i = 0; i < number_of_critical_points; i++)
    {
        ignore_cp[i] = (m_critical_point[m_type][i] ? 1U : 0U);
    }
    m_fractal.setCriticalPoints(ignore_cp);
    m_fractal.setInnerType(innerType());
    m_fractal.setCompoundImage(compoundImage());
    return m_fractal;
}

void FractalsConfig::MetaData(QImage& image)
{
    QString program = "neptune" + QString::number(LEVEL);
    image.setText("program", program);
    addBailoutFractalMetadata(image);
    addCriticalPointsMetadata(image);
}

void FractalsConfig::addBailoutFractalMetadata(QImage& image)
{
    image.setText("fractal_type", QString::number(static_cast<int>(GetFractalType())));
    image.setText("fractal_name", getName());
    image.setText("centre_r", QString::fromStdString(getCentre().r));
    image.setText("centre_i", QString::fromStdString(getCentre().i));
    image.setText("longside", QString::fromStdString(getCurrentWidth()));
    image.setText("rotation", QString::number(getRotation()));
    image.setText("bailout", QString::number(static_cast<unsigned int>(getBailout())));
    image.setText("limit", longdoubleToQString(getLimit()));
    image.setText("max_iterations", QString::number(getMaxIterations()));
    image.setText("orbit", QString::number(static_cast<unsigned int>(isOrbitPlotting())));
    image.setText("precision", QString::number(static_cast<int>(getPrecision())));
    if (isOrbitPlotting())
    {
        image.setText("omit", QString::number(getStartOrbitIteration()));
        image.setText("area", QString::number(getCalculatingArea()));
        image.setText("plot", QString::number(static_cast<int>(getOrbitPlots())));
        image.setText("density", QString::number(getDensity()));
    }
    else
    {
        image.setText("inner_type", QString::number(static_cast<int>(innerType())));
        image.setText("compound_image", QString::number(static_cast<int>(compoundImage())));
    }
    addParameterMetadata(image);
    (isOrbitPlotting() ? addOrbitColoursMetadata(image) : addColoursMetadata(image));
}

void FractalsConfig::addCriticalPointsMetadata(QImage& image)
{
    // Triton requires the actual critical points in use
    // Neptune requires ignore/use list
    unsigned int number_of_critical_points = getNumberOfCriticalPoints();
    image.setText("max_cps", QString::number(number_of_critical_points));
    for (unsigned int i = 0; i < number_of_critical_points; i++)
    {
        QString point = "ig" + QString::number(i);
        bool ignore = ignoreCriticalPoint(i);
        image.setText(point, ignore ? "1" : "0");
    }
}

void FractalsConfig::addParameterMetadata(QImage& image)
{
    image.setText("modifiers", QString::number(getNumberOfComplexParameters()));
    for (unsigned int i = 0; i < getNumberOfComplexParameters(); i++)
    {
        QString real = "p" + QString::number(i) + "r";
        QString imag = "p" + QString::number(i) + "i";
        image.setText(real, QString::fromStdString(getComplexParameter(i).r));
        image.setText(imag, QString::fromStdString(getComplexParameter(i).i));
    }
}

void FractalsConfig::addColoursMetadata(QImage& image)
{
    setInner(false);
    std::string method = getMethod();
    image.setText("method", QString::fromStdString(method));
    if (method[0] != 'f')
    {
        image.setText("smooth", QString::number(static_cast<int>(getSmooth())));
        if (method[0] != 'i')
        {
            image.setText("scale", longdoubleToQString(getColourScale()));
        }
        // due to change
        image.setText("red_offset", QString::number(getRedOffset()));
        image.setText("green_offset", QString::number(getGreenOffset()));
        image.setText("blue_offset", QString::number(getBlueOffset()));
        image.setText("order", QString::number(static_cast<int>(getComponentOrder())));
        image.setText("abslog", QString::number(static_cast<int>(getAbsLog())));
        addColourMapMetadata(image, "", getColourMap());
    }
    setInner(true);
    method = getMethod();
    image.setText("inner_method", QString::fromStdString(method));
    if (method[0] != 'f')
    {
        image.setText("inner_smooth", QString::number(static_cast<int>(getSmooth())));
        if (method[0] != 'i')
        {
            image.setText("inner_scale", longdoubleToQString(getColourScale()));
        }
        image.setText("inner_red_offset", QString::number(getRedOffset()));
        image.setText("inner_green_offset", QString::number(getGreenOffset()));
        image.setText("inner_blue_offset", QString::number(getBlueOffset()));
        image.setText("inner_order", QString::number(static_cast<int>(getComponentOrder())));
        image.setText("inner_abslog", QString::number(static_cast<int>(getAbsLog())));
        addColourMapMetadata(image, "inner_", getColourMap());
    }
}

void FractalsConfig::addOrbitColoursMetadata(QImage& image)
{
    QChar ch(getOrbitColourType());
    QString method(ch);
    image.setText("method", method);
    image.setText("interval", QString::number(getOrbitColourInterval()));
    image.setText("scale", longdoubleToQString(getOrbitScale()));
    image.setText("order", QString::number(static_cast<int>(getComponentOrder())));
    image.setText("red_offset", QString::number(getRedOffset()));
    image.setText("green_offset", QString::number(getGreenOffset()));
    image.setText("blue_offset", QString::number(getBlueOffset()));
    if (hasOrbitBackground())
    {
        QColor colour = getOrbitBackground();
        KeyString keys;
        keys.SetKey("red", colour.red());
        keys.SetKey("green", colour.green());
        keys.SetKey("blue", colour.blue());
        image.setText("background", QString::fromStdString(keys.String()));
    }
    addColourMapMetadata(image, "", getColourMap());
}

void FractalsConfig::addColourMapMetadata(QImage& image, QString prefix, QString map_name)
{
    ColourMap &colours = m_colour_maps->getColourMap(map_name);
    ColourMap::MAP_TYPE type = colours.getType();
    image.setText(prefix + "colourmap", map_name);
    image.setText(prefix + "colour_map_type", QString::number(static_cast<int>(type)));
    if (type == ColourMap::CM_MANUAL)
    {
        int count = 0;
        const ColoursMap &all_colours = colours.getColoursMap();
        int size = all_colours.size();
        image.setText(prefix + "number_of_colours", QString::number(size));
        for (auto it = all_colours.begin(); it != all_colours.end(); it++, count++)
        {
            int index = it->first;
            QColor c = it->second;
            KeyString metakeys;
            metakeys.SetKey("index", index);
            metakeys.SetKey("red", c.red());
            metakeys.SetKey("green", c.green());
            metakeys.SetKey("blue", c.blue());
            image.setText(prefix + "colour" + QString::number(count), QString::fromStdString(metakeys.String()));
        }
    }
    else if (type == ColourMap::CM_COMPONENT)
    {
        addComponentColours(image, RGB_RED, prefix, colours);
        addComponentColours(image, RGB_GREEN, prefix, colours);
        addComponentColours(image, RGB_BLUE, prefix, colours);
    }
    else
    {
        // auto colour meta data
        Waveform::AutoColour auto_colour;
        for (int i = 0; i < 3; i++)
        {
            colours.getAutoColour(static_cast<RGB>(i), auto_colour);
            KeyString metakeys;
            metakeys.SetKey("hoffset", auto_colour.hoffset);
            metakeys.SetKey("voffset", auto_colour.voffset);
            metakeys.SetKey("maximum", auto_colour.maximum);
            metakeys.SetKey("wave", static_cast<int>(auto_colour.wave));
            metakeys.SetKey("cycles", auto_colour.cycles);
            image.setText(prefix + "colour" + QString::number(i), QString::fromStdString(metakeys.String()));
        }
    }
}

void FractalsConfig::addComponentColours(QImage& image, RGB colour, const QString& prefix, ColourMap& colours)
{
    ::IntMap map;
    QString str(prefix);
    switch (colour)
    {
    case RGB_RED:
        map = colours.getRedMap();
        str += "red";
        break;
    case RGB_GREEN:
        map = colours.getGreenMap();
        str += "green";
        break;
    case RGB_BLUE:
        map = colours.getBlueMap();
        str += "blue";
        break;
    }
    image.setText(str + "_colours", QString::number(map.size()));
    int count = 0;
    for (auto it = map.begin(); it != map.end(); it++, count++)
    {
        KeyString metakeys;
        metakeys.SetKey("index", it->first);
        metakeys.SetKey("value", it->second);
        image.setText(str + QString::number(count), QString::fromStdString(metakeys.String()));
    }
}

void FractalsConfig::setComponentOrder(COMPONENT_ORDER order, bool inner)
{
    if (order > CO_BGR)
    {
        order = CO_RGB;
    }
    m_order[(inner ? 1 : 0)][m_type] = order;
}

COMPONENT_ORDER FractalsConfig::getComponentOrder(bool inner)
{
    if (isOrbitPlotting())
    {
        return m_orbit_order[m_type];
    }
    else
    {
        return m_order[(inner ? 1 : 0)][m_type];
    }
}

void FractalsConfig::setComponentOrder(COMPONENT_ORDER order)
{
    if (order > CO_BGR)
    {
        order = CO_RGB;
    }
    if (isOrbitPlotting())
    {
        m_orbit_order[m_type] = order;
    }
    else
    {
        m_order[m_index][m_type] = order;
    }
}

COMPONENT_ORDER FractalsConfig::getComponentOrder()
{
    if (isOrbitPlotting())
    {
        return m_orbit_order[m_type];
    }
    else
    {
        return m_order[m_index][m_type];
    }
}

void FractalsConfig::setRedOffset(unsigned int value, bool inner)
{
    m_red_offset[(inner ? 1 : 0)][m_type] = (value % 512);
}

void FractalsConfig::setGreenOffset(unsigned int value, bool inner)
{
    m_green_offset[(inner ? 1 : 0)][m_type] = (value % 512);
}

void FractalsConfig::setBlueOffset(unsigned int value, bool inner)
{
    m_blue_offset[(inner ? 1 : 0)][m_type] = (value % 512);
}

void FractalsConfig::setRedOffset(unsigned int value)
{
    if (isOrbitPlotting())
    {
        m_red_orbit_offset[m_type] = (value % 512);
    }
    else
    {
        m_red_offset[m_index][m_type] = (value % 512);
    }
}

void FractalsConfig::setGreenOffset(unsigned int value)
{
    if (isOrbitPlotting())
    {
        m_green_orbit_offset[m_type] = (value % 512);
    }
    else
    {
        m_green_offset[m_index][m_type] = (value % 512);
    }
}

void FractalsConfig::setBlueOffset(unsigned int offset)
{
    if (isOrbitPlotting())
    {
        m_blue_orbit_offset[m_type] = (offset % 512);
    }
    else
    {
        m_blue_offset[m_index][m_type] = (offset % 512);
    }
}

unsigned int FractalsConfig::getRedOffset(bool inner)
{
    if (isOrbitPlotting())
    {
        return m_red_orbit_offset[m_type];
    }
    else
    {
        return m_red_offset[(inner ? 1 : 0)][m_type];
    }
}

unsigned int FractalsConfig::getGreenOffset(bool inner)
{
    if (isOrbitPlotting())
    {
        return m_green_orbit_offset[m_type];
    }
    else
    {
        return m_green_offset[(inner ? 1 : 0)][m_type];
    }
}

unsigned int FractalsConfig::getBlueOffset(bool inner)
{
    if (isOrbitPlotting())
    {
        return m_blue_orbit_offset[m_type];
    }
    else
    {
        return m_blue_offset[(inner ? 1 : 0)][m_type];
    }
}

unsigned int FractalsConfig::getRedOffset()
{
    return (isOrbitPlotting() ? m_red_orbit_offset[m_type] : m_red_offset[m_index][m_type]);
}

unsigned int FractalsConfig::getGreenOffset()
{
    return (isOrbitPlotting() ? m_green_orbit_offset[m_type] : m_green_offset[m_index][m_type]);
}

unsigned int FractalsConfig::getBlueOffset()
{
    return (isOrbitPlotting() ? m_blue_orbit_offset[m_type] : m_blue_offset[m_index][m_type]);
}

void FractalsConfig::setOrbitBackground(const QColor& colour)
{
    m_background_colour[m_type] = colour;
}

void FractalsConfig::enableOrbitBackground(bool value)
{
    m_has_orbit_background[m_type] = value;
}

const QColor &FractalsConfig::getOrbitBackground()
{
    return m_background_colour[m_type];
}

bool FractalsConfig::hasOrbitBackground()
{
    return m_has_orbit_background[m_type];
}

int FractalsConfig::getPrecision()
{
    return m_precision[m_type];
}

void FractalsConfig::setPrecision(int value)
{
    m_precision[m_type] = value;
}

unsigned int FractalsConfig::getNumberOfCriticalPoints()
{
    return m_number_of_critical_points[m_type];
}

bool FractalsConfig::ignoreCriticalPoint(int index)
{
    return (m_critical_point[m_type][index] != 0U);
}

void FractalsConfig::setIgnoreCriticalPoint(int index, bool ignore)
{
    m_critical_point[m_type][index] = (ignore ? 1U : 0U);
}

unsigned int FractalsConfig::getNumberOfComplexParameters()
{
    return m_number_of_parameters[m_type];
}

const StringComplex &FractalsConfig::getComplexParameter(int i)
{
    return m_parameters[m_type][i];
}

void FractalsConfig::setComplexParameter(int i, const StringComplex &value)
{
    m_parameters[m_type][i] = value;
}

void FractalsConfig::resetFractalParameters()
{
    unsigned int& p = m_number_of_parameters[m_type];
    for (unsigned int i = 0; i < p; i++)
    {
        m_parameters[m_type][i] = m_default_parameters[m_type][i];
        m_critical_point[m_type][i] = 0U;
    }
}


//==============================================================================
// Utility methods
//==============================================================================

std::string FractalsConfig::colourSignature()
{
    std::string signature;
    if (isOrbitPlotting())
    {
        char ch = getOrbitColourType();
        auto type = static_cast<ORBIT_COLOUR>(ch - 'a');
        KeyString keys;
        keys.SetKey("method", std::string(1, getOrbitColourType()));
        keys.SetKey("map", getColourMap().toStdString());
        keys.SetKey("order", static_cast<int>(getComponentOrder()));
        switch (type)
        {
        case OC_SQRT:
        case OC_LOG:
            keys.SetKey("scale", getOrbitScale());
            break;
        case OC_AVERAGE:
            keys.SetKey("interval", getOrbitColourInterval());
            break;
        default:
            break;
        }
        keys.SetKey("red_offset", static_cast<int>(getRedOffset(false)));
        keys.SetKey("green_offset", static_cast<int>(getGreenOffset(false)));
        keys.SetKey("blue_offset", static_cast<int>(getBlueOffset(false)));
        if (hasOrbitBackground())
        {
            keys.SetKey("background", getOrbitBackground().name().toStdString());
        }
        // background colour ....
        signature = keys.String();
    }
    else
    {
        KeyString keys[2];
        for (int i = 0; i < 2; i++)
        {
            signature += m_method[i][m_type];
            if (m_method[i][m_type][0] != 'f')
            {
                signature += ":";
                keys[i].SetKey("smooth", static_cast<int>(m_smooth[i][m_type]));
                if (m_method[i][m_type][0] != 'i')
                {
                    keys[i].SetKey("abslog", static_cast<int>(m_abs_log[i][m_type]));
                }
                keys[i].SetKey("map", m_colour_map[i][m_type].toStdString());
                keys[i].SetKey("order", static_cast<int>(m_order[i][m_type]));
                keys[i].SetKey("scale", m_colour_scale[i][m_type]);
                keys[i].SetKey("red_offset", static_cast<int>(m_red_offset[i][m_type]));
                keys[i].SetKey("green_offset", static_cast<int>(m_green_offset[i][m_type]));
                keys[i].SetKey("blue_offset", static_cast<int>(m_blue_offset[i][m_type]));
                signature += keys[i].String();
            }
            signature += ":";
        }
        KeyString compound_keys;
        compound_keys.SetKey("inner_type", static_cast<int>(m_inner_type[m_type]));
        compound_keys.SetKey("compound_image", static_cast<int>(m_compound_image[m_type]));
        signature += compound_keys.String();
    }
    return signature;
}

void FractalsConfig::setInnerType(INNER_TYPE value)
{
    m_inner_type[m_type] = value;
}

INNER_TYPE FractalsConfig::innerType()
{
    return m_inner_type[m_type];
}

void FractalsConfig::setCompoundImage(COMPOUND_IMAGE value)
{
    m_compound_image[m_type] = value;
}

COMPOUND_IMAGE FractalsConfig::compoundImage()
{
    return m_compound_image[m_type];
}