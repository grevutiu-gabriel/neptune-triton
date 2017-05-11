// 
// File:   FractalConfig.cc
// Author: M.R.Eggleston
// 
// Created on 29 January 2014, 10:33
//

#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <QFile>
#include "FractalConfig.h"
#include "KeyString.h"
#include "Utilities.h"

namespace io = boost::iostreams;

FractalConfig::FractalConfig()
:
m_ft(F_MC_4_3_2_1),
m_limit(16.0L),
m_plot_type(OP_ALL),
m_orbit_plotting(false),
m_area(9),
m_omit(0),
m_width("0"),
m_rotation(0),
m_max_iterations(5000),
m_interval(10000),
m_inner_type(IT_OVERLAP),
m_compound_image(CI_AVERAGE),
m_orbit_colour_type('a'),
m_red_orbit_offset(0),
m_green_orbit_offset(0),
m_blue_orbit_offset(0),
m_orbit_scale(1.0L),
m_density(1),
m_orbit_background(),
m_has_orbit_background(false),
m_width_in_pixels(0),
m_height_in_pixels(0),
m_index(0),
m_level(-1),
m_ok(false),
m_precision(0)
{
    for (int i = 0; i < 2; i++)
    {
        m_smooth[i] = false;
        m_abs_log[i] = false;
        m_scale[i] = 1.0L;
        m_red_offset[i] = 0;
        m_blue_offset[i] = 0;
        m_green_offset[i] = 0;
        m_order[i] = CO_RGB;
    }
}

FractalConfig::FractalConfig(FRACTAL_TYPE ft)
{
    FractalConfig();
    m_ft = ft;
}

FractalConfig::~FractalConfig()
{
}

FractalConfig::FractalConfig(const FractalConfig &other)
{
    operator=(other);
}

const FractalConfig &FractalConfig::operator=(const FractalConfig &other)
{
    m_name = other.m_name;
    m_ft = other.m_ft;
    m_limit = other.m_limit;
    m_bailout = other.m_bailout;
    m_plot_type = other.m_plot_type;
    m_orbit_plotting = other.m_orbit_plotting;
    m_area = other.m_area;
    m_omit = other.m_omit;
    m_density = other.m_density;
    m_centre = other.m_centre;
    m_width = other.m_width;
    m_rotation = other.m_rotation;
    m_max_iterations = other.m_max_iterations;
    m_interval = other.m_interval;
    for (int i = 0; i < 2; i++)
    {
        m_method[i] = other.m_method[i];
        m_colour_map_name[i] = other.m_colour_map_name[i];
        m_red_offset[i] = other.m_red_offset[i];
        m_green_offset[i] = other.m_green_offset[i];
        m_blue_offset[i] = other.m_blue_offset[i];
        m_scale[i] = other.m_scale[i];
        m_smooth[i] = other.m_smooth[i];
        m_abs_log[i] = other.m_abs_log[i];
        m_colour_map[i] = other.m_colour_map[i];
        m_order[i] = other.m_order[i];
    }
    m_orbit_colour_map = other.m_orbit_colour_map;
    m_orbit_colour_type = other.m_orbit_colour_type;
    m_red_orbit_offset = other.m_red_orbit_offset;
    m_green_orbit_offset = other.m_green_orbit_offset;
    m_blue_orbit_offset = other.m_blue_orbit_offset;
    m_orbit_scale = other.m_orbit_scale;
    m_ok = other.m_ok;
    m_level = other.m_level;
    m_width_in_pixels = other.m_width_in_pixels;
    m_height_in_pixels = other.m_height_in_pixels;
    m_width_in_pixels = other.m_width_in_pixels;
    m_height_in_pixels = other.m_height_in_pixels;
    m_has_orbit_background = other.m_has_orbit_background;
    m_orbit_background = other.m_orbit_background;
    m_precision = other.m_precision;
    m_number_of_critical_points = other.m_number_of_critical_points;
    m_number_of_complex_parameters = other.m_number_of_complex_parameters;
    for (int i = 0; i < MAX_PARAMETERS; i++)
    {
        m_cp[i] = other.m_cp[i];
    }
    m_ignore_critical_point = other.m_ignore_critical_point;
    m_inner_type = other.m_inner_type;
    m_compound_image = other.m_compound_image;
    return *this;
}

const QString& FractalConfig::getName()
{
    return m_name;
}

void FractalConfig::setName(const QString& name)
{
    m_name = name;
}

FRACTAL_TYPE FractalConfig::GetFractalType() const
{
    return m_ft;
}

void FractalConfig::SetFractalType(FRACTAL_TYPE ft)
{
    m_ft = ft;
}

unsigned int FractalConfig::getNumberOfComplexParameters() const
{
    return m_number_of_complex_parameters;
}

void FractalConfig::setNumberOfComplexParameters(unsigned int value)
{
    m_number_of_complex_parameters = value;
}

const StringComplex FractalConfig::getComplexParameter(unsigned int index) const
{
    return (index < MAX_PARAMETERS ? m_cp[index] : StringComplex());
}

void FractalConfig::setComplexParameter(unsigned int index, const StringComplex& value)
{
    if (index < MAX_PARAMETERS)
    {
        m_cp[index] = value;
    }
}

VectorStringComplex FractalConfig::getComplexParameters()
{
    VectorStringComplex v;
    for (unsigned int i = 0; i < m_number_of_complex_parameters; i++)
    {
        v.push_back(m_cp[i]);
    }
    return v;
}

long double FractalConfig::getLimit()
{
    return m_limit;
}

void FractalConfig::setLimit(long double value)
{
    m_limit = value;
}

BAILOUT FractalConfig::getBailout()
{
    return m_bailout;
}

void FractalConfig::setBailout(BAILOUT value)
{
    m_bailout = value;
}

ORBIT_PLOTS FractalConfig::getOrbitPlot()
{
    return m_plot_type;
}

void FractalConfig::setOrbitPlot(ORBIT_PLOTS plot)
{
    m_plot_type = plot;
}

bool FractalConfig::isOrbitPlotting()
{
    return m_orbit_plotting;
}

void FractalConfig::setOrbitPlotting(bool plotting)
{
    m_orbit_plotting = plotting;
}

unsigned int FractalConfig::getArea()
{
    return m_area;
}

void FractalConfig::setArea(unsigned int area)
{
    m_area = area;
}

unsigned int FractalConfig::getStartOrbitIteration()
{
    return m_omit;
}

void FractalConfig::setOmit(unsigned int omit)
{
    m_omit = omit;
}

void FractalConfig::setDensity(unsigned int value)
{
    m_density = value;
}

unsigned int FractalConfig::getDensity()
{
    return m_density;
}

const StringComplex &FractalConfig::getImageCentre()
{
    return m_centre;
}

void FractalConfig::setImageCentre(const StringComplex &centre)
{
    m_centre = centre;
}

const std::string& FractalConfig::getImageWidth()
{
    return m_width;
}

void FractalConfig::setImageWidth(const std::string &value)
{
    m_width = value;
}

int FractalConfig::getRotation()
{
    return m_rotation;
}

void FractalConfig::setRotation(int value)
{
    m_rotation = value;
}

unsigned int FractalConfig::getMaxIterations()
{
    return m_max_iterations;
}

void FractalConfig::setMaxIterations(unsigned int value)
{
    m_max_iterations = value;
}

unsigned long FractalConfig::getInterval()
{
    return m_interval;
}

void FractalConfig::setInterval(unsigned int value)
{
    m_interval = value;
}


bool FractalConfig::isSmooth(bool inner)
{
    return m_smooth[inner ? 1 : 0];
}

void FractalConfig::setSmooth(bool inner, bool value)
{
    m_smooth[inner ? 1 : 0] = value;
}

void FractalConfig::setSmooth(bool value)
{
    m_smooth[m_index] = value;
}

bool FractalConfig::isAbsLog(bool inner)
{
    return m_abs_log[inner ? 1 : 0];
}

void FractalConfig::setAbsLog(bool inner, bool value)
{
    m_abs_log[inner ? 1 : 0] = value;
}

void FractalConfig::setAbsLog(bool value)
{
    m_abs_log[m_index] = value;
}

const std::string &FractalConfig::getMethod(bool inner)
{
    return m_method[inner ? 1 : 0];
}

void FractalConfig::setMethod(bool inner, const std::string& value)
{
    m_method[inner ? 1 : 0] = value;
}

void FractalConfig::setMethod(const std::string& value)
{
    m_method[m_index] = value;
}

bool FractalConfig::isOrbitTrap(bool inner)
{
    return m_method[static_cast<bool>(inner)][0] == 't';
}

bool FractalConfig::hasMultipleResults(bool inner)
{
    char type = getMethod(inner)[0];
    return (type == 'g') || (type == 'M') ||
           (type == 'A') || (type == 't') ||
           (type == 'r');
}

bool FractalConfig::hasTwoResultsSets()
{
    bool two_sets = false;
    std::string &outer_method = m_method[0];
    std::string &inner_method = m_method[1];
    bool outer_multiple = hasMultipleResults(false);
    bool inner_multiple = hasMultipleResults(true);
    if (outer_multiple && inner_multiple)
    {
        // Two sets if both are Gaussia Integer of differing types 
        two_sets = (outer_method[0] == 'g') && (outer_method[0] == 'g') &&
                   ((outer_method[1] != inner_method[1]) ||
                   (outer_method[2] != inner_method[2]));
        // Different colour method types
        if (!two_sets)
        {
            two_sets = (outer_method[0] != inner_method[0]);
        }
        // Same type, check for only being change in value indicated by a
        // difference in case of the option letters
        if (!two_sets)
        {
            bool both_angle = (outer_method[0] == 'A') && (inner_method[0] == 'A');
            bool both_magnitude = (outer_method[0] == 'M') && (inner_method[0] == 'M');
            if (both_angle || both_magnitude)
            {
                two_sets = ((outer_method[1] != inner_method[1]));
            }
            if (!two_sets && both_magnitude)
            {
                // Two sets of data required if Magnitude is set for both inner 
                // and outer and only one of them is Fractal Dimension
                two_sets = ((outer_method == "Mj") && (inner_method != "Mj"));
                two_sets = two_sets || ((outer_method != "Mj") && (inner_method == "Mj"));
            }
        }
        if (!two_sets)
        {
            // Two sets of data if both inner and outer are differing orbit traps
            two_sets = ((outer_method[0]== 't') &&
                        (inner_method[0] == 't') &&
                        (outer_method != inner_method));
        }
    }
    return two_sets;
}

//==============================================================================
// Code for setting fractal configuration from seed file metadata
//==============================================================================

int FractalConfig::neptuneLevel()
{
    int level = -1;
    auto str = option("program");
    QString neptune("neptune");
    int neptune_length = neptune.length();
    int str_length = str.length();
    bool is_neptune = false;
    if (str_length >= neptune_length)
    {
        is_neptune = (neptune == str.left(neptune_length));
    }
    if (is_neptune)
    {
        if (str_length == neptune_length)
        {
            level = 0;
        }
        else
        {
            QString id = str.mid(neptune_length);
            int value = id.toInt();
            if (value > 0)
            {
                level = value;
            }
        }
    }
    m_ok = (((level >= 0) && (level <= LEVEL)));
    return level;
}

QString FractalConfig::option(const QString& name)
{
    QString value;
    if (m_map.count(name) == 1)
    {
        value = m_map[name];
    }
    return value;
}

std::string FractalConfig::std_option(const QString& name)
{
    return option(name).toStdString();
}

void FractalConfig::readKeys(const QString& name)
{
    // QImage on OS X and Windows fails to load all the seed file's metadata
    // reporting that there is no space in the chunk cache for tEXt.
    //
    // To get around this problem the file must be read and processed directly.
    //
    bool ok = false;
    unsigned char signature[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
    unsigned char header_id[4] = { 73, 72, 68, 82 };
    unsigned char data[8];
    qint64 number_of_bytes = 0;
    qint64 pos = 0;
    bool first_chunk = true;
    CHUNK_TYPE type = CT_IGNORE;
    QFile file(name);
    if (file.open(QIODevice::ReadOnly))
    {
        file.seek(0);
        number_of_bytes = file.read(reinterpret_cast<char*>(data), 8);
        if (number_of_bytes == 8)
        {
            for (int i = 0; i < 8 ; i++)
            {
                ok = (data[i] == signature[i]);
                if (!ok)
                {
                    break;
                }
            }
        }
        pos += number_of_bytes;
        while (ok && !file.atEnd() && (type != CT_IEND))
        {
            ok = file.seek(pos);
            if (ok)
            {
                // Read size of data
                number_of_bytes = file.read(reinterpret_cast<char*>(data), 4);
                ok = (number_of_bytes == 4);
                int size = convertFromNetworkFormat32(data);
                // update position for start of next chunk
                pos += (size + 12); // extra 12, length, type & for CRC
                // Read chunk id
                number_of_bytes = file.read(reinterpret_cast<char*>(data), 4);
                ok = (number_of_bytes == 4);
                if (first_chunk)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        ok = (data[i] == header_id[i]);
                        if (!ok)
                        {
                            break;
                        }
                    }
                    if (ok)
                    {
                        number_of_bytes = file.read(reinterpret_cast<char*>(data), 4);
                        ok = (number_of_bytes == 4);
                        m_width_in_pixels = convertFromNetworkFormat32(data);
                        number_of_bytes = file.read(reinterpret_cast<char*>(data), 4);
                        ok = (number_of_bytes == 4);
                        m_height_in_pixels = convertFromNetworkFormat32(data);
                        first_chunk = false;
                    }
                }
                else
                {
                    type = chunk_type(data);
                    if (type == CT_TEXT)
                    {
                        std::string key;
                        std::string value;
                        int i = 1;
                        ok = file.getChar(reinterpret_cast<char*>(data));
                        while ((i < size) && (*data != static_cast<unsigned char>(0)))
                        {
                            if (ok) key += *data;
                            ok = file.getChar(reinterpret_cast<char*>(data));
                            i++;
                        }
                        while (i < size)
                        {
                            ok = file.getChar(reinterpret_cast<char*>(data));
                            value += *data;
                            i++;
                        }
                        if (ok)
                        {
                            m_map[QString::fromStdString(key)] = QString::fromStdString(value);
                        }
                    }
                    else if (type == CT_ZTXT)
                    {
                        std::string key;
                        std::vector<char> compressed_value;
                        int i = 1;
                        ok = file.getChar(reinterpret_cast<char*>(data));
                        while ((i < size) && (*data != static_cast<unsigned char>(0)))
                        {
                            if (ok) key += *data;
                            ok = file.getChar(reinterpret_cast<char*>(data));
                            i++;
                        }
                        // skip compression method
                        ok = file.getChar(reinterpret_cast<char*>(data));
                        i++;
                        while (i < size)
                        {
                            ok = file.getChar(reinterpret_cast<char*>(data));
                            compressed_value.push_back(*data);
                            i++;
                        }
                        if (ok)
                        {
                            std::string value = uncompress(compressed_value);
                            m_map[QString::fromStdString(key)] = QString::fromStdString(value);
                        }
                        
                    }
                }
            }
        }
    }
    file.close();
    m_ok = ok;
}

void FractalConfig::setFromSeed(const QString& file_name)
{
    readKeys(file_name);
    if (m_ok)
    {
        int ft = option("fractal_type").toInt();
        m_level = neptuneLevel();
        if (m_ok)
        {
            m_centre = StringComplex(std_option("centre_r"), std_option("centre_i"));
            m_width = std_option("longside");
            m_rotation = option("rotation").toInt();
            if (m_rotation > 180) m_rotation = m_rotation - 360;
            getBailoutFractalMetadata(ft);
        }
    }
    m_map.clear(); // Data no longer required.
}

bool FractalConfig::isOk()
{
    return m_ok;
}    

void FractalConfig::getBailoutFractalMetadata(unsigned int ft)
{
    m_ft = static_cast<FRACTAL_TYPE>(ft);
    getParameterMetadata();
    getCriticalPointsMetadata();
    m_orbit_plotting = (option("orbit").toUInt() == 1);
    if (isOrbitPlotting())
    {
        m_area = option("area").toUInt();
        QString str = option("plot");
        ORBIT_PLOTS plot = OP_ALL;
        if (!str.isEmpty())
        {
            plot = static_cast<ORBIT_PLOTS>(str.toUInt());
        }
        m_plot_type = plot;
        m_omit = option("omit").toUInt();
        str = option("density");
        setDensity(str.isEmpty() ? 1 : str.toUInt());
        getOrbitColourMethod();
    }
    else
    {
        auto str = option("precision");
        setPrecision(str.isEmpty() ? BASE_PRECISION : str.toInt());
        getColourMethodMetadata(false); // Outer colouring
        getColourMethodMetadata(true); // Inner colouring
        str = option("inner_type");
        setInnerType(str.isEmpty() ? IT_OVERLAP : static_cast<INNER_TYPE>(str.toInt()));
        str = option("compound_image");
        setCompoundImage(str.isEmpty() ? CI_AVERAGE : static_cast<COMPOUND_IMAGE>(str.toInt()));
    }
    m_limit = stringToLongDouble(option("limit").toStdString());
    m_bailout = static_cast<BAILOUT>(option("bailout").toUInt());
    m_max_iterations = option("max_iterations").toUInt();
}

void FractalConfig::getParameterMetadata()
{
    QString smodifiers = option("modifiers");
    m_number_of_complex_parameters = smodifiers.toUInt();
    for (unsigned int i = 0; i < m_number_of_complex_parameters; i++)
    {
        QString re = "p" + QString::number(i) + "r";
        QString im = "p" + QString::number(i) + "i";
        m_cp[i] = StringComplex(std_option(re), std_option(im));
    }
}

void FractalConfig::getCriticalPointsMetadata()
{
    // Critical points are now calculated prior to use and no longer stored
    // in this configuration object.
    // 
    // This critical point ignore/use list is still required.
    QString max_cps = option("max_cps");
    if (max_cps.length() != 0)
    {
        auto points = max_cps.toUInt();
        m_ignore_critical_point.resize(points);
        for (auto i = 0U; i < points; i++)
        {
            QString point = "ig" + QString::number(i);
            m_ignore_critical_point[i] = static_cast<unsigned int>(option(point) == "1");
        }
    }
    else
    {
        // Old seed file
        QString number_of_cp = option("number_of_cp");
        auto points = number_of_cp.toUInt();
        m_ignore_critical_point.resize(points);
    }
}

void FractalConfig::getOrbitColourMethod()
{
    m_orbit_colour_type = static_cast<char>(std_option("method")[0]);
    m_interval = option("interval").toUInt();

    m_red_orbit_offset = option("red_offset").toUInt();
    m_green_orbit_offset = option("green_offset").toUInt();
    m_blue_orbit_offset = option("blue_offset").toUInt();
    m_order[0] = static_cast<COMPONENT_ORDER>(option("order").toUInt());
    QString background = option("background");
    if (background.isEmpty())
    {
        m_has_orbit_background = false;
    }
    else
    {
        m_has_orbit_background = true;
        KeyString keys(background.toStdString());
        int r = keys.GetKeyAsInt("red", 0);
        int g = keys.GetKeyAsInt("green", 0);
        int b = keys.GetKeyAsInt("blue", 0);
        m_orbit_background = QColor(r, g, b);
    }

    m_orbit_scale = stringToLongDouble(std_option("scale"));
    getColourMapMetadata(false);
}

void FractalConfig::getColourMethodMetadata(bool inner)
{
    int index = (inner ? 1 : 0);
    QString prefix = (inner ? "inner_" : "");
    m_method[index] = std_option(prefix + "method");
    if (m_method[index][0] != 'f')
    {
        if (m_method[index][0] != 'i')
        {
            std::string str = std_option(prefix + "divisions");
            if (str.empty()) str = std_option(prefix + "scale");
            m_scale[index] = stringToLongDouble(str);
        }
        m_smooth[index] = (option(prefix + "smooth").toUInt() == 1);
        getColourMapMetadata(inner);
        m_red_offset[index] = option(prefix + "red_offset").toUInt();
        m_green_offset[index] = option(prefix + "green_offset").toUInt();
        m_blue_offset[index] = option(prefix + "blue_offset").toUInt();
        m_order[index] = static_cast<COMPONENT_ORDER>(option(prefix + "order").toUInt());
        m_abs_log[index] = (option(prefix + "abslog").toUInt() == 1);
    }
}

void FractalConfig::getColourMapMetadata(bool inner)
{
    ColourMap &colours = m_colour_map[(inner ? 1 : 0)];
    colours.clear();
    QString prefix = (inner ? "inner_" : "");
    QString name = option(prefix + "colourmap");
    QRgb c;
    auto type = static_cast<ColourMap::MAP_TYPE>(option(prefix + "colour_map_type").toUInt());
    colours.setType(type);
    if (type == ColourMap::CM_MANUAL)
    {
        int number_of_colours = option(prefix + "number_of_colours").toInt();
        for (int i=0; i<number_of_colours; i++)
        {
            KeyString keys(std_option(prefix + "colour" + QString::number(i)));
            int index = keys.GetKeyAsInt("index", 0);
            c = qRgb(static_cast<int>(keys.GetKeyAsInt("red", 0)),
                     static_cast<int>(keys.GetKeyAsInt("green", 0)),
                     static_cast<int>(keys.GetKeyAsInt("blue", 0)));
            colours.setColour(index, c);
        }
    }
    else if (type == ColourMap::CM_COMPONENT)
    {
        getComponentColour(RGB_RED, prefix + "red", colours);
        getComponentColour(RGB_GREEN, prefix + "green", colours);
        getComponentColour(RGB_BLUE, prefix + "blue", colours);
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            KeyString keys(std_option(prefix + "colour" + QString::number(i)));
            Waveform::AutoColour auto_colour;
            auto_colour.hoffset = keys.GetKeyAsInt("hoffset", 0);
            auto_colour.voffset = keys.GetKeyAsInt("voffset", 0);
            auto_colour.maximum = keys.GetKeyAsInt("maximum", 255);
            auto_colour.wave = static_cast<Waveform::WAVE>(keys.GetKeyAsInt("wave", 0));
            auto_colour.cycles = keys.GetKeyAsInt("cycles", 1);
            colours.setAutoColour(static_cast<RGB>(i), auto_colour);
        }
    }
    if (m_orbit_plotting)
    {
        m_orbit_colour_map = name;
    }
    else
    {
        m_colour_map_name[inner ? 1 : 0] = name;
    }
}

void FractalConfig::getComponentColour(RGB colour, const QString& base_name, ColourMap& map)
{
    int number_of_colours = option(base_name + "_colours").toInt();
    for (int i = 0; i < number_of_colours; i++)
    {
        KeyString keys(std_option(base_name + QString::number(i)));
        unsigned int index = static_cast<unsigned int>(keys.GetKeyAsInt("index", 0));
        unsigned int value = static_cast<unsigned int>(keys.GetKeyAsInt("value", 0));
        if (colour == RGB_RED)
        {
            map.setRed(index, value);
        }
        else if (colour == RGB_GREEN)
        {
            map.setGreen(index, value);
        }
        else
        {
            map.setBlue(index, value);
        }
    }
}

QString FractalConfig::getColourMapName(bool inner)
{
    return m_colour_map_name[(inner ? 1 : 0)];
}

QString FractalConfig::getOrbitColourMap()
{
    return m_orbit_colour_map;
}

long double FractalConfig::getScale(bool inner)
{
    return m_scale[inner];
}

void FractalConfig::setScale(bool inner, long double value)
{
    m_scale[inner] = value;
}

void FractalConfig::setScale(long double value)
{
    m_scale[m_index] = value;
}

bool FractalConfig::getSmooth(bool inner)
{
    return m_smooth[(inner ? 1 : 0)];
}

bool FractalConfig::getAbsLog(bool inner)
{
    return m_abs_log[(inner ? 1 : 0)];
}

void FractalConfig::setOrbitColourInterval(unsigned int interval)
{
    m_interval = interval;
}

unsigned int FractalConfig::getOrbitColourInterval()
{
    return m_interval;
}

void FractalConfig::setOrbitColourType(char type)
{
    m_orbit_colour_type = type;
}

char FractalConfig::getOrbitColourType()
{
    return m_orbit_colour_type;
}

void FractalConfig::setOrbitScale(long double scale)
{
    m_orbit_scale = scale;
}

long double FractalConfig::getOrbitScale()
{
    return m_orbit_scale;
}

void FractalConfig::setOrbitRedOffset(unsigned int offset)
{
    m_red_orbit_offset = offset;
}

unsigned int FractalConfig::getOrbitRedOffset()
{
    return m_red_orbit_offset;
}

void FractalConfig::setOrbitGreenOffset(unsigned int offset)
{
    m_green_orbit_offset = offset;
}

unsigned int FractalConfig::getOrbitGreenOffset()
{
    return m_green_orbit_offset;
}

void FractalConfig::setOrbitBlueOffset(unsigned int offset)
{
    m_blue_orbit_offset = offset;
}

unsigned int FractalConfig::getOrbitBlueOffset()
{
    return m_blue_orbit_offset;
}

bool FractalConfig::hasColourMaps()
{
    return (!m_colour_map[0].empty() || !m_colour_map[1].empty());
}

ColourMap FractalConfig::getColourMap(bool inner)
{
    return m_colour_map[(inner ? 1 : 0)];
}

void FractalConfig::setColourMap(const ColourMap& map, bool inner)
{
    m_colour_map[inner] = map;
}

unsigned int FractalConfig::widthInPixels()
{
    return m_width_in_pixels;
}

unsigned int FractalConfig::heightInPixels()
{
    return m_height_in_pixels;
}

int FractalConfig::getWidth()
{
    return m_width_in_pixels;
}

void FractalConfig::setWidth(int w)
{
    m_width_in_pixels = w;
}

int FractalConfig::getHeight()
{
    return m_height_in_pixels;
}

void FractalConfig::setHeight(int h)
{
    m_height_in_pixels = h;
}
    
const StringComplex &FractalConfig::getCentre()
{
    return m_centre;
}

void FractalConfig::setCentre(const StringComplex& value)
{
    m_centre = value;
}

void FractalConfig::setComponentOrder(COMPONENT_ORDER order, bool inner)
{
    m_order[inner ? 1 : 0] = order;
}

void FractalConfig::setComponentOrder(COMPONENT_ORDER order)
{
    m_order[m_index] = order;
}

COMPONENT_ORDER FractalConfig::getComponentOrder(bool inner)
{
    return m_order[inner ? 1 : 0];
}

void FractalConfig::setRedOffset(unsigned int red, bool inner)
{
    m_red_offset[(inner ? 1 : 0)] = red;
}

void FractalConfig::setGreenOffset(unsigned int green, bool inner)
{
    m_green_offset[(inner ? 1 : 0)] = green;
}

void FractalConfig::setBlueOffset(unsigned int blue, bool inner)
{
    m_blue_offset[(inner ? 1 : 0)] = blue;
}

void FractalConfig::setRedOffset(unsigned int red)
{
    m_red_offset[m_index] = red;
}

void FractalConfig::setGreenOffset(unsigned int green)
{
    m_green_offset[m_index] = green;
}

void FractalConfig::setBlueOffset(unsigned int blue)
{
    m_blue_offset[m_index] = blue;
}

unsigned int FractalConfig::getRedOffset(bool inner)
{
    return m_red_offset[(inner ? 1 : 0)];
}

unsigned int FractalConfig::getGreenOffset(bool inner)
{
    return m_green_offset[(inner ? 1 : 0)];
}

unsigned int FractalConfig::getBlueOffset(bool inner)
{
    return m_blue_offset[(inner ? 1 : 0)];
}

unsigned int FractalConfig::getRedOffset()
{
    return m_red_offset[m_index];
}

unsigned int FractalConfig::getGreenOffset()
{
    return m_green_offset[m_index];
}

unsigned int FractalConfig::getBlueOffset()
{
    return m_blue_offset[m_index];
}

void FractalConfig::setOrbitBackground(const QColor& colour)
{
    m_orbit_background = colour;
}

void FractalConfig::enableOrbitBackground(bool value)
{
    m_has_orbit_background = value;
}

const QColor &FractalConfig::getOrbitBackground()
{
    return m_orbit_background;
}

bool FractalConfig::hasOrbitBackground()
{
    return m_has_orbit_background;
}

std::string FractalConfig::uncompress(std::vector<char>& buffer)
{
    std::string str;
    io::filtering_ostream os;
    os.push(io::zlib_decompressor());
    os.push(io::back_inserter(str));
    int size = buffer.size();
    for (int i = 0; i < size; i++)
    {
        os.put(buffer[i]);
    }
    return str;
}

FractalConfig::CHUNK_TYPE FractalConfig::chunk_type(unsigned char* data)
{
    static std::string text_id = "tEXt";
    static std::string ztxt_id = "zTXt";
    static std::string end_id = "IEND";    // IEND
    CHUNK_TYPE type = CT_IGNORE;
    std::string id;
    id += data[0];
    id += data[1];
    id += data[2];
    id += data[3];
    if (id == text_id)
    {
        type = CT_TEXT;
    }
    else if (id == ztxt_id)
    {
        type = CT_ZTXT;
    }
    else if (id == end_id)
    {
        type = CT_IEND;
    }
    return type;
}

void FractalConfig::setInner(bool inner)
{
    m_index = (inner ? 1 : 0);
}

bool FractalConfig::isInner()
{
    return (m_index == 1);
}


void FractalConfig::setStartingCentre(const StringComplex& value)
{
    m_scentre = value;
}

void FractalConfig::setStartingWidth(const std::string& value)
{
    m_swidth = value;
}

void FractalConfig::setStartingRotation(const unsigned int value)
{
    m_srotation = value;
}

const StringComplex& FractalConfig::getStartingCentre()
{
    return m_scentre;
}

const std::string& FractalConfig::getStartingWidth()
{
    return m_swidth;
}

int FractalConfig::getStartingRotation()
{
    return m_srotation;
}

void FractalConfig::setColourMapName(const QString& name, bool inner)
{
    m_colour_map_name[(inner ? 1 : 0)] = name;    
}

void FractalConfig::setColourMapName(const QString& name)
{
    m_colour_map_name[m_index] = name;    
}

void FractalConfig::setOrbitColourMapName(const QString& name)
{
    m_orbit_colour_map = name;
}

int FractalConfig::getPrecision() const
{
    return m_precision;
}

void FractalConfig::setPrecision(int value)
{
    m_precision = value;
}

void FractalConfig::setCriticalPoints(const VectorUInt& cp)
{
    m_number_of_critical_points = static_cast<unsigned int>(cp.size());
    m_ignore_critical_point = cp;
}


unsigned int FractalConfig::getNumberOfCriticalPoints() const
{
    return m_ignore_critical_point.size();
}

void FractalConfig::setIgnoreCriticalPoint(unsigned int index, bool ignore)
{
    m_ignore_critical_point[index] = (ignore ? 1U : 0U);
}

bool FractalConfig::ignoreCriticalPoint(unsigned int index) const
{
    return (m_ignore_critical_point[index] != 0U);
}

void FractalConfig::setInnerType(INNER_TYPE value)
{
    m_inner_type = value;
}

INNER_TYPE FractalConfig::innerType()
{
    return m_inner_type;
}

void FractalConfig::setCompoundImage(COMPOUND_IMAGE value)
{
    m_compound_image = value;
}

COMPOUND_IMAGE FractalConfig::compoundImage()
{
    return m_compound_image;
}
