//
// File:   Config.cc
// Author: M.R.Eggleston
//
// Created on 17 July 2010, 16:13
//

#include <boost/scoped_ptr.hpp>
#include <exception>
#include <iostream>
#include <limits>

#include <QByteArray>
#include <QDir>
#include <QString>

#include "Config.h"
#include "Utilities.h"
#include "FractalsConfig.h"
#include "KeyString.h"

boost::shared_ptr<Config> Config::s_instance;

boost::shared_ptr<Config> Config::create()
{
    if (!s_instance)
    {
        s_instance = boost::shared_ptr<Config>(new Config());
        s_instance->load();
    }
    return s_instance;
}

boost::shared_ptr<Config> Config::instance()
{
    return s_instance;
}

Config::Config()
:
m_ft(F_MC_4_3_2_1),
m_file_exists(false),
m_directory_path(QDir::homePath() + "/.neptune-1.1"),
m_config_file_path(m_directory_path + "/config.xml"),
m_colour_file_path(m_directory_path + "/colour.xml"),
m_colour_maps(ColourMaps::instance()),
m_level(LEVEL)
{
}

Config::~Config()
{
}

void Config::save_parameter_file(FRACTAL_TYPE ft, const QString& path)
{
    boost::scoped_ptr<QDomDocument> dom(new QDomDocument());
    auto root = dom->createElement("neptune_parameters");
    dom->appendChild(root);
    root.setAttribute("level", LEVEL);
    root.setAttribute("hpixels", m_fractals_config.getWidth());
    root.setAttribute("vpixels", m_fractals_config.getHeight());
    save_fractal_child(dom.get(), root, ft);
    if (m_fractals_config.isOrbitPlotting())
    {
        save_colour_map_child(dom.get(), root, m_fractals_config.getColourMap());
    }
    else
    {
        save_colour_map_child(dom.get(), root, m_fractals_config.getColourMap(false));
        save_colour_map_child(dom.get(), root, m_fractals_config.getColourMap(true));
    }
    QByteArray doc_bytes = dom->toByteArray();
    QFile file(path);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc_bytes, doc_bytes.size());
    }
    else
    {
        std::cout << "error " << file.error() << std::endl;
    }
    file.close();
}

void Config::save()
{
    save_config_file();
    save_colour_file();
}

void Config::save_config_file()
{
    boost::scoped_ptr<QDomDocument> dom(new QDomDocument());
    auto root = dom->createElement("saturn");
    dom->appendChild(root);
    save_current_child(dom.get(), root);
    save_fractal_children(dom.get(), root);
    QByteArray doc_bytes = dom->toByteArray();
    QFile backup(m_config_file_path + "~");
    backup.remove();
    QFile new_backup(m_config_file_path);
    new_backup.rename(m_config_file_path + "~");
    QFile file(m_config_file_path);
    file.open(QIODevice::WriteOnly);
    file.write(doc_bytes, doc_bytes.size());
    file.close();
}

void Config::save_colour_file()
{
    boost::scoped_ptr<QDomDocument> dom(new QDomDocument());
    auto root = dom->createElement("saturn");
    dom->appendChild(root);
    save_colour_map_children(dom.get(), root);
    QByteArray doc_bytes = dom->toByteArray();
    if (doc_bytes.size() > 0)
    {
        QFile backup(m_colour_file_path + "~");
        QFile new_backup(m_colour_file_path);
        new_backup.rename(m_colour_file_path + "~");
        QFile file(m_colour_file_path);
        file.open(QIODevice::WriteOnly);
        file.write(doc_bytes, doc_bytes.size());
        file.close();
    }
}

void Config::load()
{
    load_colour_file();
    load_config_file();
}

void Config::load_config_file()
{
    boost::scoped_ptr<QDomDocument> dom(new QDomDocument());
    // Does config directory exist.
    QDir directory(QDir::homePath());
    if (!directory.exists(m_directory_path))
    {
        if (!directory.mkpath(m_directory_path))
        {
            std::cout << "failed to create " << m_directory_path.toStdString() << std::endl;
        }
    }
    bool ok = false;
    // Does config.xml exist.
    if (directory.exists(m_config_file_path))
    {
        boost::shared_ptr<Config> cfg = Config::instance();
        QFile file(m_config_file_path);
        if (file.open(QIODevice::ReadOnly))
        {
            ok = dom->setContent(&file);
            file.close();
        }
    }
    if (ok)
    {
        // The document contains one node saturn
        QDomElement root = dom->documentElement();
        if (!root.isNull())
        {
            // Node should be saturn
            if (root.tagName() == "saturn")
            {
                // is Saturn's config file 
                QDomElement child = root.firstChildElement();
                while (!child.isNull())
                {
                    if (child.tagName() == "current")
                    {
                        load_current_child(child);
                    }
                    else if (child.tagName() == "fractal")
                    {
                        load_fractal_child(child, false);
                        m_fractals_config.SetFractal(m_fractal_config);
                    }
                    child = child.nextSiblingElement();
                }
            }
        }
    }
}

void Config::load_colour_file()
{
    boost::scoped_ptr<QDomDocument> dom(new QDomDocument());
    QDir directory(QDir::homePath());
    bool ok = false;
    // Does config.xml exist.
    if (directory.exists(m_colour_file_path))
    {
        boost::shared_ptr<Config> cfg = Config::instance();
        QFile file(m_colour_file_path);
        if (file.open(QIODevice::ReadOnly))
        {
            ok = dom->setContent(&file);
            file.close();
        }
    }
    if (ok)
    {
        // The document contains one node saturn
        QDomElement root = dom->documentElement();
        if (!root.isNull())
        {
            // Node should be saturn
            if (root.tagName() == "saturn")
            {
                // is Saturn's config file 
                QDomElement child = root.firstChildElement();
                while (!child.isNull())
                {
                    if (child.tagName() == "colourmap")
                    {
                        load_colour_map_child(child, false);
                    }
                    child = child.nextSiblingElement();
                }
            }
        }
    }
}

void Config::save_current_child(QDomDocument* dom, QDomElement& root)
{
    auto tag = dom->createElement("current");
    tag.setAttribute("fractaltype", static_cast<int>(m_ft));
    tag.setAttribute("hpixels", m_fractals_config.getWidth());
    tag.setAttribute("vpixels", m_fractals_config.getHeight());
    root.appendChild(tag);
}

void Config::save_fractal_children(QDomDocument* dom, QDomElement& root)
{
    FractalsConfig::VectorFractalId &fractals = m_fractals_config.getAllFractals();
    for (auto it = fractals.begin(); it != fractals.end(); it++)
    {
        save_fractal_child(dom, root, it->type);
    }
}

void Config::save_fractal_child(QDomDocument* dom, QDomElement& root, FRACTAL_TYPE ft)
{
    FractalsConfig &f = m_fractals_config;
    f.SetFractalType(ft);
    auto tag = dom->createElement("fractal");
    tag.setAttribute("name", f.getName());
    tag.setAttribute("id", static_cast<int>(ft));
    tag.setAttribute("orbit", static_cast<int>(f.isOrbitPlotting()));
    int precision = f.getPrecision();
    if ((!f.isOrbitPlotting()) && (precision != 0))
    {
        tag.setAttribute("precision", precision);
    }
    root.appendChild(tag);
    save_fractal_parameters(dom, tag);
    save_fractal_colour(dom, tag);
    save_fractal_starting_position(dom, tag);
    save_fractal_current_position(dom, tag);
}

void Config::save_fractal_colour(QDomDocument* dom, QDomElement& root)
{
    // Add colour node
    auto colour_tag = dom->createElement("colour");
    root.appendChild(colour_tag);
    if (m_fractals_config.isOrbitPlotting())
    {
        save_orbit_colour_child(dom, colour_tag);
    }
    else
    {
        save_fractal_colour_child(dom, colour_tag, "outer", false);
        save_fractal_colour_child(dom, colour_tag, "inner", true);
    }
}

void Config::save_fractal_colour_child(QDomDocument* dom, QDomElement& root, const QString& tagName, bool inner)
{
    auto tag = dom->createElement(tagName);
    root.appendChild(tag);
    FractalsConfig &f = m_fractals_config;
    if (!inner)
    {
        save_colour_selection(dom, root);
    }
    f.setInner(inner);
    std::string method = f.getMethod(inner);
    if (method.empty())
    {
        method = "i";
    }
    if (method[0] == 'f')
    {
        save_fixed_colour(dom, tag, method);
    }
    else
    {
        save_colour_map(dom, tag);
        if (method == "a")
        {
            save_final_angle(dom, tag);
        }
        else if (method[0] == 'm')
        {
            save_final_magnitude(dom, tag, method);
        }
        else if (method[0] == 'g')
        {
            save_gaussian(dom, tag, method);
        }
        else if (method[0] == 'M')
        {
            save_magnitude(dom, tag, method);
        }
        else if (method[0] == 'A')
        {
            save_angle(dom, tag, method);
        }
        else if (method[0] == 'r')
        {
            save_ratio(dom, tag, method);
        }
        else if (method[0] == 't')
        {
            save_trap(dom, tag, method, false);
        }
        else if (method[0] == 'i')
        {
            auto child_tag = dom->createElement("iteration");
            child_tag.setAttribute("smooth", static_cast<int>(f.getSmooth()));
            tag.appendChild(child_tag);
        }
        if (f.getAbsLog())
        {
            auto child_tag = dom->createElement("abslog");
            tag.appendChild(child_tag);
        }
    }
}

void Config::save_fractal_parameters(QDomDocument* dom, QDomElement& root)
{
    auto parameters_tag = dom->createElement("parameters");
    root.appendChild(parameters_tag);
    FractalsConfig &f = m_fractals_config;
    QDomElement tag = dom->createElement("bailout");
    tag.setAttribute("type", longdoubleToQString(f.getBailout()));
    tag.setAttribute("limit", longdoubleToQString(f.getLimit()));
    parameters_tag.appendChild(tag);
    tag = dom->createElement("max_iterations");
    tag.setAttribute("value", f.getMaxIterations());
    parameters_tag.appendChild(tag);
    if (f.isOrbitPlotting())
    {
        tag = dom->createElement("start_orbit_iteration");
        tag.setAttribute("value", f.getStartOrbitIteration());
        parameters_tag.appendChild(tag);
        tag = dom->createElement("plot");
        tag.setAttribute("area", f.getCalculatingArea());
        tag.setAttribute("type", static_cast<int>(f.getOrbitPlots()));
        parameters_tag.appendChild(tag);
        tag = dom->createElement("plot_density");
        tag.setAttribute("value", f.getDensity());
        parameters_tag.appendChild(tag);
    }
    save_complex_parameters(dom, parameters_tag);
    save_critical_points(dom, parameters_tag);
}

void Config::save_complex_parameters(QDomDocument* dom, QDomElement& root)
{
    FractalsConfig &f = m_fractals_config;
    int modifiers = f.getNumberOfComplexParameters();
    for (int i = 0; i < modifiers; i++)
    {
        auto tag = dom->createElement("complex_param");
        tag.setAttribute("r", QString::fromStdString(f.getComplexParameter(i).r));
        tag.setAttribute("i", QString::fromStdString(f.getComplexParameter(i).i));
        root.appendChild(tag);
    }
}

void Config::save_critical_points(QDomDocument* dom, QDomElement& root)
{
    FractalsConfig &f = m_fractals_config;
    int points = f.getNumberOfCriticalPoints();
    for (int i = 0; i < points; i++)
    {
        auto tag = dom->createElement("critical_point");
        tag.setAttribute("ignore", f.ignoreCriticalPoint(i) ? "1" : "0");
        root.appendChild(tag);
    }
}

void Config::save_colour_map_children(QDomDocument* dom, QDomElement& root)
{
    unsigned int colour_map_number = 0;
    QStrings &names = m_colour_maps->getNames();
    if (names.begin() != names.end())
    {
        for (auto it = names.begin(); it != names.end(); it++)
        {
            save_colour_map_child(dom, root, *it);
            colour_map_number++;
        }
    }
}

void Config::save_colour_map_child(QDomDocument* dom, QDomElement& root, const QString& name)
{
    if (!m_colour_maps->isDefault(name))
    {
        ColourMap colours = m_colour_maps->getColourMap(name);
        auto type = colours.getType();
        auto colourmap_tag = dom->createElement("colourmap");
        colourmap_tag.setAttribute("name", name);
        colourmap_tag.setAttribute("type", static_cast<int>(type));
        root.appendChild(colourmap_tag);
        if (type == ColourMap::CM_MANUAL)
        {
            auto first = colours.getColoursMap().begin();
            auto last = colours.getColoursMap().end();
            for (auto it = first; it != last; it++)
            {
                int index = it->first;
                QColor c = it->second;
                auto rgb_tag = dom->createElement("rgb");
                rgb_tag.setAttribute("index", index);
                rgb_tag.setAttribute("red", c.red());
                rgb_tag.setAttribute("green", c.green());
                rgb_tag.setAttribute("blue", c.blue());
                colourmap_tag.appendChild(rgb_tag);
            }
        }
        else if (type == ColourMap::CM_COMPONENT)
        {
            add_component_colours(dom, colourmap_tag, RGB_RED, colours);
            add_component_colours(dom, colourmap_tag, RGB_GREEN, colours);
            add_component_colours(dom, colourmap_tag, RGB_BLUE, colours);
        }
        else
        {
            Waveform::AutoColour colour;
            QString rgb[3] = { "red", "green", "blue" };
            RGB colour_id[3] = { RGB_RED, RGB_GREEN, RGB_BLUE };
            for (int i = 0; i < 3; i++)
            {
                colours.getAutoColour(colour_id[i], colour);
                auto colour_tag = dom->createElement(rgb[i]);
                colour_tag.setAttribute("hoffset", colour.hoffset);
                colour_tag.setAttribute("voffset", colour.voffset);
                colour_tag.setAttribute("maximum", colour.maximum);
                colour_tag.setAttribute("wave", static_cast<int>(colour.wave));
                colour_tag.setAttribute("cycles", colour.cycles);
                colourmap_tag.appendChild(colour_tag);
            }
        }
    }
}

void Config::add_component_colours(QDomDocument* dom, QDomElement& root, RGB colour, ColourMap& colours)
{
    IntMap map;   
    QString tag_string;
    switch(colour)
    {
    default:
    case RGB_RED:
        tag_string = "red";
        map = colours.getRedMap();
        break;
    case RGB_GREEN:
        tag_string = "green";
        map = colours.getGreenMap();
        break;
    case RGB_BLUE:
        tag_string = "blue";
        map = colours.getBlueMap();
        break;
    }
    for (auto it = map.begin(); it != map.end(); it++)
    {
        int index = it->first;
        int c = it->second;
        auto colour_tag = dom->createElement(tag_string);
        colour_tag.setAttribute("index", index);
        colour_tag.setAttribute("value", c);
        root.appendChild(colour_tag);
    }
}

FRACTAL_TYPE Config::getFractalType()
{
    return m_ft;
}

void Config::setFractalType(FRACTAL_TYPE ft)
{
    m_ft = ft;
}

QRgb Config::getColour(const std::string &str)
{
    int red = 0;
    int green = 0;
    int blue = 0;
    try
    {
        std::istringstream input(str);
        char junk;
        input >> junk;  // 'f'
        input >> junk;  // ','
        input >> red;
        input >> junk;
        input >> green;
        input >> junk;
        input >> blue;
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    return qRgb(red, green, blue);
}

void Config::load_current_child(const QDomElement& child)
{
    m_ft = static_cast<FRACTAL_TYPE>(child.attribute("fractaltype").toUInt());
    m_fractals_config.setWidth(child.attribute("hpixels").toInt());
    m_fractals_config.setHeight(child.attribute("vpixels").toInt());
}

void Config::load_fractal_child(const QDomElement& root, bool parameters)
{
    auto ft = static_cast<FRACTAL_TYPE>(root.attribute("id").toInt());
    m_fractals_config.SetFractalType(ft);
    m_fractal_config = m_fractals_config.getFractalConfig(false);
    m_fractal_config.setOrbitPlotting(root.attribute("orbit").toInt() == 1);
    auto str = root.attribute("precision");
    if (!str.isEmpty())
    {
        int precision = str.toInt();
        if ((precision > BASE_PRECISION) && ((precision % 16) == 0))
        {
            m_fractal_config.setPrecision(precision);
        }
    }
    else
    {
        m_fractal_config.setPrecision(0);
    }
    QDomElement child = root.firstChildElement();
    while (!child.isNull())
    {
        QString name = child.tagName();
        if (name == "parameters")
        {
           load_fractal_parameters(child);
        }
        else if (name == "colour")
        {
            load_fractal_colour_parameters(child, parameters);
        }
        else if (name == "startposition")
        {
            unsigned int rotation = child.attribute("angle").toInt();
            m_fractal_config.setStartingCentre(StringComplex(child.attribute("r").toStdString(),
                                                             child.attribute("i").toStdString()));
            m_fractal_config.setStartingWidth(child.attribute("l").toStdString());
            m_fractal_config.setStartingRotation(rotation);
        }
        else if (name == "currentposition")
        {
            unsigned int rotation = child.attribute("angle").toInt();
            m_fractal_config.setImageCentre(StringComplex(child.attribute("r").toStdString(),
                                                          child.attribute("i").toStdString()));
            m_fractal_config.setImageWidth(child.attribute("l").toStdString());
            m_fractal_config.setRotation(rotation);
        }
        child = child.nextSiblingElement();
    }
}

void Config::load_fractal_parameters(const QDomElement& root)
{
    FractalConfig &f = m_fractal_config;
    int p_index = 0;
    int cp_index = 0;
    QString name;
    QDomElement child = root.firstChildElement();
    while (!child.isNull())
    {
        name = child.tagName();
        if (name == "bailout")
        {
            f.setBailout(static_cast<BAILOUT>(child.attribute("type").toUInt()));
            f.setLimit(stringToLongDouble(child.attribute("limit").toStdString()));
        }
        else if (name == "max_iterations")
        {
            f.setMaxIterations(child.attribute("value").toInt());
        }
        else if (name == "start_orbit_iteration")
        {
            f.setOmit(child.attribute("value").toUInt());
        }
        else if (name == "complex_param")
        {
            std::string r = child.attribute("r").toStdString();
            std::string i = child.attribute("i").toStdString();
            f.setComplexParameter(p_index, StringComplex(r, i));
            p_index++;
        }
        else if (name == "critical_point")
        {
            auto ignore = child.attribute("ignore");
            f.setIgnoreCriticalPoint(cp_index, (ignore == "1"));
            cp_index++;
        }
        else if (name == "plot")
        {
            auto type = child.attribute("type").toUInt();
            f.setArea(child.attribute("area").toUInt());
            f.setOrbitPlot(static_cast<ORBIT_PLOTS>(type));
        }
        else if (name == "plot_density")
        {
            f.setDensity(child.attribute("value").toInt()); 
        }
        child = child.nextSiblingElement();
    }
}

void Config::load_fractal_colour_parameters(const QDomElement& root, bool parameters_file)
{
    FractalConfig &f = m_fractal_config;
    QString name;
    QDomElement child = root.firstChildElement();
    while (!child.isNull())
    {
        name = child.tagName();
        if (name == "map")
        {
            f.setInner(false);
            load_fractal_colour_map(child, parameters_file);
        }
        else if (name == "orbit")
        {
            f.setInner(false);
            f.enableOrbitBackground(false);
            QDomElement orbit_child = child.firstChildElement();
            while (!orbit_child.isNull())
            {
                name = orbit_child.tagName();
                if (name == "map")
                {
                    if (parameters_file)
                    {
                        m_map1 = orbit_child.attribute("name");
                        f.setOrbitRedOffset(orbit_child.attribute("red_offset").toUInt());
                        f.setOrbitGreenOffset(orbit_child.attribute("green_offset").toUInt());
                        f.setOrbitBlueOffset(orbit_child.attribute("blue_offset").toUInt());
                        f.setComponentOrder(static_cast<COMPONENT_ORDER>(orbit_child.attribute("order").toUInt()));
                    }
                    else
                    {
                        f.setOrbitColourMapName(orbit_child.attribute("name"));
                        f.setOrbitRedOffset(orbit_child.attribute("red_offset").toUInt());
                        f.setOrbitGreenOffset(orbit_child.attribute("green_offset").toUInt());
                        f.setOrbitBlueOffset(orbit_child.attribute("blue_offset").toUInt());
                        f.setComponentOrder(static_cast<COMPONENT_ORDER>(orbit_child.attribute("order").toUInt()));
                    }
                }
                else if (name == "accumulation")
                {
                    f.setOrbitColourType('a');
                }
                else if (name == "sqrt")
                {
                    f.setOrbitScale(stringToLongDouble(orbit_child.attribute("scale").toStdString()));
                    f.setOrbitColourType('b');
                }
                else if (name == "average")
                {
                    f.setOrbitColourInterval(orbit_child.attribute("intervals").toUInt());
                    f.setOrbitColourType('c');
                }
                else if (name == "log")
                {
                    f.setOrbitScale(stringToLongDouble(orbit_child.attribute("scale").toStdString()));
                    f.setOrbitColourType('d');
                }
                else if (name == "background")
                {
                    int r = orbit_child.attribute("red").toInt();
                    int g = orbit_child.attribute("green").toInt();
                    int b = orbit_child.attribute("blue").toInt();
                    f.setOrbitBackground(QColor(r,g,b));
                    f.enableOrbitBackground(true);
                }
                orbit_child = orbit_child.nextSiblingElement();
            }
        }
        else if (name == "outer")
        {
            f.setInner(false);
            load_fractal_colour_parameters_child(child, parameters_file);
        }
        else if (name == "inner")
        {
            f.setInner(true);
            load_fractal_colour_parameters_child(child, parameters_file);
        }
        else if (name == "select")
        {
            auto inner_type = static_cast<INNER_TYPE>(child.attribute("inner_type").toInt());
            auto compound_image = static_cast<COMPOUND_IMAGE>(child.attribute("compound_image").toInt());
            m_fractal_config.setInnerType(inner_type);
            m_fractal_config.setCompoundImage(compound_image);
        }
        child = child.nextSiblingElement();
    }
}

void Config::load_fractal_colour_parameters_child(const QDomElement& root, bool parameters)
{
    bool abslog = false;
    QString name;
    QDomElement child = root.firstChildElement();
    while (!child.isNull())
    {
        name = child.tagName();
        if (name == "map")
        {
            load_fractal_colour_map(child, parameters);
        }
        else if (name == "abslog")
        {
            abslog = true;
        }
        else if (name == "iteration")
        {
            m_fractal_config.setMethod("i");
            m_fractal_config.setSmooth(child.attribute("smooth").toUInt() == 1);
        }
        else if (name == "fixed")
        {
            QString fixed("f,");
            fixed += child.attribute("r");
            fixed += ",";
            fixed += child.attribute("g");
            fixed += ",";
            fixed += child.attribute("b");
            m_fractal_config.setMethod(fixed.toStdString());
        }
        else if (name == "final_angle")
        {
            m_fractal_config.setMethod("a");
            m_fractal_config.setScale(stringToLongDouble(child.attribute("scale").toStdString()));
            m_fractal_config.setSmooth(child.attribute("smooth").toUInt() == 1);
        }
        else if (name == "final_magnitude")
        {
            std::string magnitude("m");
            magnitude += child.attribute("type").toStdString();
            m_fractal_config.setMethod(magnitude);
            m_fractal_config.setScale(stringToLongDouble(child.attribute("scale").toStdString()));
            m_fractal_config.setSmooth(child.attribute("smooth").toUInt() == 1);
        }
        else if (name == "gaussian")
        {
            std::string magnitude("g");
            magnitude += child.attribute("type").toStdString();
            m_fractal_config.setMethod(magnitude);
            m_fractal_config.setScale(stringToLongDouble(child.attribute("scale").toStdString()));
            m_fractal_config.setSmooth(child.attribute("smooth").toUInt() == 1);
        }
        else if (name == "magnitude")
        {
            std::string magnitude("M");
            magnitude += child.attribute("type").toStdString();
            m_fractal_config.setMethod(magnitude);
            m_fractal_config.setScale(stringToLongDouble(child.attribute("scale").toStdString()));
            m_fractal_config.setSmooth(child.attribute("smooth").toUInt() == 1);
        }
        else if (name == "angle")
        {
            std::string magnitude("A");
            magnitude += child.attribute("type").toStdString();
            m_fractal_config.setMethod(magnitude);
            m_fractal_config.setScale(stringToLongDouble(child.attribute("scale").toStdString()));
            m_fractal_config.setSmooth(child.attribute("smooth").toUInt() == 1);
        }
        else if (name == "ratio")
        {
            std::string magnitude("r");
            magnitude += child.attribute("type").toStdString();
            m_fractal_config.setMethod(magnitude);
            m_fractal_config.setScale(stringToLongDouble(child.attribute("scale").toStdString()));
            m_fractal_config.setSmooth(child.attribute("smooth").toUInt() == 1);
        }
        else if (name == "trap")
        {
            load_trap_parameters(child, true);
        }
        child = child.nextSiblingElement();
    }
    m_fractal_config.setAbsLog(abslog);
}


void Config::load_colour_map_child(const QDomElement& root, bool parameters)
{
    ColourMap map;
    QDomElement child = root.firstChildElement();
    QString map_name = root.attribute("name");
    ColourMap::MAP_TYPE type = ColourMap::CM_MANUAL;
    type = static_cast<ColourMap::MAP_TYPE>(root.attribute("type").toInt());
    map.setType(type);
    QString tag_name;
    while (!child.isNull())
    {
        tag_name = child.tagName();
        if (tag_name == "rgb")
        {
            unsigned int index = child.attribute("index").toUInt();
            QRgb colour = qRgb(child.attribute("red").toInt(),
                               child.attribute("green").toInt(),
                               child.attribute("blue").toInt());
            map.setColour(index, colour);
        }
        else if ((tag_name == "red") && (type == ColourMap::CM_AUTO)) 
        {
            load_auto_colour(child, RGB_RED, map);
        }
        else if ((tag_name == "green") && (type == ColourMap::CM_AUTO)) 
        {
            load_auto_colour(child, RGB_GREEN, map);
        }
        else if ((tag_name == "blue") && (type == ColourMap::CM_AUTO)) 
        {
            load_auto_colour(child, RGB_BLUE, map);
        }
        else if (tag_name == "red")
        {
            unsigned int index = child.attribute("index").toUInt();
            unsigned int value = child.attribute("value").toUInt();
            map.setRed(index, value);
        }
        else if (tag_name == "green")
        {
            unsigned int index = child.attribute("index").toUInt();
            unsigned int value = child.attribute("value").toUInt();
            map.setGreen(index, value);
        }
        else if (tag_name == "blue")
        {
            unsigned int index = child.attribute("index").toUInt();
            unsigned int value = child.attribute("value").toUInt();
            map.setBlue(index, value);
        }
        child = child.nextSiblingElement();
    }
    QString assigned_map_name = m_colour_maps->setColours(map_name, map, parameters);
    if (map_name == m_map1)
    {
        m_map1 = assigned_map_name;
    }
    if (map_name == m_map2)
    {
        m_map2 = assigned_map_name;
    }
}

void Config::load_auto_colour(const QDomElement& child, RGB colour, ColourMap &map)
{
    Waveform::AutoColour auto_colour;
    auto_colour.hoffset = child.attribute("hoffset").toInt();
    auto_colour.voffset = child.attribute("voffset").toInt();
    auto_colour.maximum = child.attribute("maximum").toInt();
    auto_colour.wave = static_cast<Waveform::WAVE>(child.attribute("wave").toUInt());
    auto_colour.cycles = child.attribute("cycles").toUInt();
    map.setAutoColour(colour, auto_colour);
}

void Config::save_fractal_starting_position(QDomDocument* dom, QDomElement& root)
{
    auto tag = dom->createElement("startposition");
    tag.setAttribute("r", QString::fromStdString(m_fractals_config.getStartingCentre().r));
    tag.setAttribute("i", QString::fromStdString(m_fractals_config.getStartingCentre().i));
    tag.setAttribute("l", QString::fromStdString(m_fractals_config.getStartingWidth()));
    tag.setAttribute("angle", m_fractals_config.getStartingRotation());
    root.appendChild(tag);
}

void Config::save_fractal_current_position(QDomDocument* dom, QDomElement& root)
{
    auto tag = dom->createElement("currentposition");
    tag.setAttribute("r", QString::fromStdString(m_fractals_config.getCentre().r));
    tag.setAttribute("i", QString::fromStdString(m_fractals_config.getCentre().i));
    tag.setAttribute("l", QString::fromStdString(m_fractals_config.getCurrentWidth()));
    tag.setAttribute("angle", m_fractals_config.getRotation());
    root.appendChild(tag);
}

void Config::save_colour_selection(QDomDocument* dom, QDomElement& root)
{
    auto& f = m_fractals_config;
    if (!f.isOrbitPlotting())
    {
        auto tag = dom->createElement("select");
        tag.setAttribute("inner_type", QString::number(static_cast<int>(f.innerType())));
        tag.setAttribute("compound_image", QString::number(static_cast<int>(f.compoundImage())));
        root.appendChild(tag);
    }
}

void Config::save_colour_map(QDomDocument* dom, QDomElement& root)
{
    FractalsConfig& f = m_fractals_config;
    auto tag = dom->createElement("map");
    tag.setAttribute("name", f.getColourMap());
    tag.setAttribute("order", f.getComponentOrder());
    tag.setAttribute("red_offset", f.getRedOffset());
    tag.setAttribute("green_offset", f.getGreenOffset());
    tag.setAttribute("blue_offset", f.getBlueOffset());
    root.appendChild(tag);
}

void Config::save_orbit_colour_child(QDomDocument* dom, QDomElement& root)
{
    auto tag = dom->createElement("orbit");
    root.appendChild(tag);
    FractalsConfig& f = m_fractals_config;
    f.setInner(false);
    auto map_tag = dom->createElement("map");
    map_tag.setAttribute("name", f.getColourMap());
    map_tag.setAttribute("order", f.getComponentOrder());
    map_tag.setAttribute("red_offset", f.getRedOffset());
    map_tag.setAttribute("green_offset", f.getGreenOffset());
    map_tag.setAttribute("blue_offset", f.getBlueOffset());
    tag.appendChild(map_tag);
    char type = f.getOrbitColourType();
    QDomElement child_tag;
    switch (type)
    {
    default:
    case 'a':
        child_tag = dom->createElement("accumulation");
        break;
    case 'b':
        child_tag = dom->createElement("sqrt");
        child_tag.setAttribute("scale", longdoubleToQString(f.getOrbitScale()));
        break;
    case 'c':
        child_tag = dom->createElement("average");
        child_tag.setAttribute("intervals", f.getOrbitColourInterval());
        break;
    case 'd':
        child_tag = dom->createElement("log");
        child_tag.setAttribute("scale", longdoubleToQString(f.getOrbitScale()));
        break;
    }
    tag.appendChild(child_tag);
    if (f.hasOrbitBackground())
    {
        QColor colour = f.getOrbitBackground();
        child_tag = dom->createElement("background");
        child_tag.setAttribute("red", colour.red());
        child_tag.setAttribute("green", colour.green());
        child_tag.setAttribute("blue", colour.blue());
        tag.appendChild(child_tag);
    }
}    

void Config::save_fixed_colour(QDomDocument* dom, QDomElement& root, const std::string &method)
{
    auto tag = dom->createElement("fixed");
    QRgb colour = getColour(method);
    tag.setAttribute("r", qRed(colour));
    tag.setAttribute("g", qGreen(colour));
    tag.setAttribute("b", qBlue(colour));
    root.appendChild(tag);
}

void Config::save_final_angle(QDomDocument* dom, QDomElement& root)
{
    FractalsConfig& f = m_fractals_config;
    auto tag = dom->createElement("final_angle");
    tag.setAttribute("scale", longdoubleToQString(f.getColourScale()));
    tag.setAttribute("smooth", f.getSmooth());
    root.appendChild(tag);
}

void Config::save_final_magnitude(QDomDocument* dom, QDomElement& root, std::string &method)
{
    auto tag = dom->createElement("final_magnitude");
    if (method.length() != 2)
    {
        method = "mn";
    }
    std::string type = method.substr(1,1);
    add_method_attributes(tag, type);
    root.appendChild(tag);
}

void Config::save_gaussian(QDomDocument* dom, QDomElement& root, std::string &method)
{
    auto tag = dom->createElement("gaussian");
    if (method.length() != 3)
    {
        method = "gac";
    }
    std::string type = method.substr(1,2);
    add_method_attributes(tag, type);
    root.appendChild(tag);
}

void Config::save_magnitude(QDomDocument* dom, QDomElement& root, std::string &method)
{
    auto tag = dom->createElement("magnitude");
    if (method.length() != 2)
    {
        method = "Ma";
    }
    std::string type = method.substr(1,1);
    add_method_attributes(tag, type);
    root.appendChild(tag);
}

void Config::save_angle(QDomDocument* dom, QDomElement& root, std::string &method)
{
    auto tag = dom->createElement("angle");
    if (method.length() != 2)
    {
        method = "Aa";
    }
    std::string type = method.substr(1,1);
    add_method_attributes(tag, type);
    root.appendChild(tag);
}

void Config::save_ratio(QDomDocument* dom, QDomElement& root, std::string &method)
{
    auto tag = dom->createElement("ratio");
    if (method.length() != 4)
    {
        method = "raba";
    }
    std::string type = method.substr(1,3);
    add_method_attributes(tag, type);
    root.appendChild(tag);
}

void Config::save_trap(QDomDocument* dom, QDomElement& root, std::string &method, bool inner)
{
    TrapConfig &t = m_trap_config;
    auto trap_tag = dom->createElement("trap");
    if (method.length() < 3)
    {
        method = "taa,cp=0,i=0.0,r=0.0,rot=0.0";
    }
    std::string type = method.substr(1,2);
    add_method_attributes(trap_tag, type);
    OrbitTrap::TRAP_TYPE trap = static_cast<OrbitTrap::TRAP_TYPE>(method[1] - 'a');
    t.setTrapType(inner, trap);
    t.configFromString(method.substr(3));
    LongComplex pos = t.getPosition();
    auto child_tag = dom->createElement("position");
    child_tag.setAttribute("r", longdoubleToQString(real(pos)));
    child_tag.setAttribute("i", longdoubleToQString(imag(pos)));
    long double width = t.getWidth();
    long double hieght = width; // to be added later when closest distance to an ellipse is implemented
    child_tag = dom->createElement("size");
    child_tag.setAttribute("w", longdoubleToQString(width));
    child_tag.setAttribute("h", longdoubleToQString(hieght));
    trap_tag.appendChild(child_tag);
    child_tag = dom->createElement("centre_point");
    child_tag.setAttribute("value", t.getCentrePoint());
    trap_tag.appendChild(child_tag);
    if (t.hasRotation())
    {
        child_tag = dom->createElement("rotation"); //  check
        child_tag.setAttribute("value", t.getRotation());
        trap_tag.appendChild(child_tag);
    }
    if (t.isSteinerChain())
    {
        child_tag = dom->createElement("chain");
        child_tag.setAttribute("n", t.getChainCircles());
        child_tag.setAttribute("invd", longdoubleToQString(t.getInversionDistance()));
        child_tag.setAttribute("crot", longdoubleToQString(t.getConstructionRotation()));
        child_tag.setAttribute("inner", t.getInnerCircle());
        child_tag.setAttribute("outer", t.getOuterCircle());
        child_tag.setAttribute("triangles", t.getTriangles());
        child_tag.setAttribute("cp", t.getCentrePoints());
        trap_tag.appendChild(child_tag);
    }
    root.appendChild(trap_tag);
}

void Config::add_method_attributes(QDomElement& child, const std::string &type)
{
    FractalsConfig &f = m_fractals_config;
    child.setAttribute("type", QString::fromStdString(type));
    child.setAttribute("scale", longdoubleToQString(f.getColourScale()));
    child.setAttribute("smooth", f.getSmooth());
}


void Config::load_fractal_colour_map(const QDomElement& child, bool parameters)
{
    FractalConfig& f = m_fractal_config;
    QString &map = (f.isInner() ? m_map2 : m_map1);
    if (parameters)
    {
        map = child.attribute("name");
        f.setRedOffset(child.attribute("red_offset").toUInt());
        f.setGreenOffset(child.attribute("green_offset").toUInt());
        f.setBlueOffset(child.attribute("blue_offset").toUInt());
        f.setComponentOrder(static_cast<COMPONENT_ORDER>(child.attribute("order").toUInt()));
    }
    else
    {
        f.setColourMapName(child.attribute("name"));
        f.setRedOffset(child.attribute("red_offset").toUInt());
        f.setGreenOffset(child.attribute("green_offset").toUInt());
        f.setBlueOffset(child.attribute("blue_offset").toUInt());
        f.setComponentOrder(static_cast<COMPONENT_ORDER>(child.attribute("order").toUInt()));
    }
}

void Config::load_trap_parameters(const QDomElement& root, bool inner)
{
    std::string method = "t";
    method += root.attribute("type").toStdString();
    m_fractal_config.setScale(stringToLongDouble(root.attribute("scale").toStdString()));
    m_fractal_config.setSmooth(root.attribute("smooth").toUInt() == 1);
    OrbitTrap::TRAP_TYPE trap = static_cast<OrbitTrap::TRAP_TYPE>(method[1] - 'a');
    m_trap_config.setTrapType(inner, trap);
    QString name;
    QDomElement child = root.firstChildElement();
    while (!child.isNull())
    {
        name = child.tagName();
        if (name == "position")
        {
            long double r = stringToLongDouble(child.attribute("r").toStdString());
            long double i = stringToLongDouble(child.attribute("i").toStdString());
            m_trap_config.setPosition(LongComplex(r, i));
        }
        else if (name == "size")
        {
            long double w = stringToLongDouble(child.attribute("w").toStdString());
            m_trap_config.setWidth(w);
        }
        else if (name == "centre_point")
        {
            m_trap_config.setCentrePoint(child.attribute("value").toUInt() == 1);
        }
        else if (name == "rotation")
        {
            m_trap_config.setRotation(child.attribute("value").toUInt());
        }
        else if (name == "chain")
        {
            m_trap_config.setChainCircles(child.attribute("n").toUInt());
            m_trap_config.setInversionDistance(stringToLongDouble(child.attribute("invd").toStdString()));
            m_trap_config.setConstructionRotation(stringToLongDouble(child.attribute("crot").toStdString()));
            m_trap_config.setInnerCircle(child.attribute("inner").toUInt() == 1);
            m_trap_config.setOuterCircle(child.attribute("outer").toUInt() == 1);
            m_trap_config.setTriangles(child.attribute("triangles").toUInt() == 1);
            m_trap_config.setCentrePoints(child.attribute("cp").toUInt() == 1);
        }
        child = child.nextSiblingElement();
    }
    method += "," + m_trap_config.configAsString();
    m_fractal_config.setMethod(method);
}

bool Config::load_parameter_file(const QString& path)
{
    m_fractal_config = FractalConfig();
    m_map1 = "";
    m_map2 = "";
    boost::scoped_ptr<QDomDocument> dom(new QDomDocument());
    QDir directory(QDir::homePath());
    bool ok = false;
    if (directory.exists(path))
    {
        boost::shared_ptr<Config> cfg = Config::instance();
        QFile file(path);
        if (file.open(QIODevice::ReadOnly))
        {
            ok = dom->setContent(&file);
            file.close();
        }
    }
    if (ok)
    {
        ok = false;
        QDomElement root = dom->documentElement();
        if (!root.isNull())
        {
            // Node should be saturn
            QString name = root.tagName();
            if (name == "neptune_parameters")
            {
                m_level = root.attribute("level").toInt();
                ok = ((m_level >= 7) && (m_level <= LEVEL));
                if (ok)
                {
                    QString str = root.attribute("hpixels");
                    if (!str.isEmpty())
                    {
                        m_fractal_config.setWidth(str.toUInt());
                        m_fractal_config.setHeight(root.attribute("vpixels").toUInt());
                    }
                    QDomElement child = root.firstChildElement();
                    while (!child.isNull())
                    {
                        QString name = child.tagName();
                        if (name == "fractal")
                        {
                            m_fractal_config.setDensity(1);
                            load_fractal_child(child, true);
                        }
                        else if (name == "colourmap")
                        {
                            load_colour_map_child(child, true);
                        }
                        child = child.nextSiblingElement();
                    }
                    if (m_fractal_config.isOrbitPlotting())
                    {
                        m_fractal_config.setOrbitColourMapName(m_map1);
                    }
                    else
                    {
                        m_fractal_config.setColourMapName(m_map1, false);
                        m_fractal_config.setColourMapName(m_map2, true);
                    }
                }
            }
        }
    }
    return ok;
}

void Config::save_scm_file(const QString& path, std::vector<QString>& maps)
{
    boost::scoped_ptr<QDomDocument> dom(new QDomDocument());
    auto root = dom->createElement("saturn_colour_maps");
    dom->appendChild(root);
    for (auto it = maps.begin(); it != maps.end(); it++)
    {
         save_colour_map_child(dom.get(), root, *it);
    }
    QByteArray doc_bytes = dom->toByteArray();
    QFile file(path);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc_bytes, doc_bytes.size());
    }
    else
    {
        std::cout << "error " << file.error() << std::endl;
    }
    file.close();
}


FractalConfig &Config::fractal_config()
{
    return m_fractal_config;
}