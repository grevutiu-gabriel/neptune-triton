// 
// File:   FractalConfig.h
// Author: M.R.Eggleston
//
// Created on 29 January 2014, 12:05
//

#pragma once

#include <QImage>
#include "ColourMap.h"
#include "TypesAndConstants.h"

class FractalConfig
{
public:
    FractalConfig();
    FractalConfig(FRACTAL_TYPE ft);
    FractalConfig(const FractalConfig& orig);
    const FractalConfig &operator=(const FractalConfig &other);

    virtual ~FractalConfig();
    
    int getWidth();
    void setWidth(int w);
    int getHeight();
    void setHeight(int h);
    const StringComplex &getCentre();
    void setCentre(const StringComplex& value);
    
    void setFromSeed(const QString& file_name);
    bool isOk();
    
    const QString& getName();
    void setName(const QString& name);
    
    const QString& getFormula();
    void setFormula(const QString& value);

    FRACTAL_TYPE GetFractalType() const;
    void SetFractalType(FRACTAL_TYPE ft);

    unsigned int getNumberOfComplexParameters() const;
    void setNumberOfComplexParameters(unsigned int value);
    const StringComplex getComplexParameter(unsigned int index) const;
    void setComplexParameter(unsigned int index, const StringComplex& value);
    VectorStringComplex getComplexParameters();

    long double getLimit();
    void setLimit(long double value);
    BAILOUT getBailout();
    void setBailout(BAILOUT VALUE);
    ORBIT_PLOTS getOrbitPlot();
    void setOrbitPlot(ORBIT_PLOTS plot);
    bool isOrbitPlotting();
    void setOrbitPlotting(bool plotting);
    unsigned int getArea();
    void setArea(unsigned int area);
    unsigned int getStartOrbitIteration();
    void setOmit(unsigned int omit);
    void setDensity(unsigned int value);
    unsigned int getDensity();
    const StringComplex &getImageCentre();
    void setImageCentre(const StringComplex &centre);
    const std::string& getImageWidth();
    void setImageWidth(const std::string& value);
    int getRotation();
    void setRotation(int value);
    unsigned int getMaxIterations();
    void setMaxIterations(unsigned int value);
    unsigned long getInterval();
    void setInterval(unsigned int value);
    bool isSmooth(bool inner);
    void setSmooth(bool inner, bool value);
    void setSmooth(bool value);
    bool isAbsLog(bool inner);
    void setAbsLog(bool inner, bool value);
    void setAbsLog(bool value);
    const std::string& getMethod(bool inner);
    void setMethod(bool inner, const std::string& value);
    void setMethod(const std::string& value);
    bool isOrbitTrap(bool inner);
    
    bool hasMultipleResults(bool inner);
    bool hasTwoResultsSets();
    
    QString getColourMapName(bool inner);
    QString getOrbitColourMap();
    
    long double getScale(bool inner);
    void setScale(bool inner, long double value);
    void setScale(long double value);
    bool getSmooth(bool inner);
    bool getAbsLog(bool inner);
    
    unsigned int getOrbitColourInterval();
    void setOrbitColourInterval(unsigned int interval);
    char getOrbitColourType();
    void setOrbitColourType(char type);
    long double getOrbitScale();
    void setOrbitScale(long double scale);
    unsigned int getOrbitRedOffset();
    void setOrbitRedOffset(unsigned int offset);
    unsigned int getOrbitGreenOffset();
    void setOrbitGreenOffset(unsigned int offset);
    unsigned int getOrbitBlueOffset();
    void setOrbitBlueOffset(unsigned int offset);
    
    bool hasColourMaps();
    ColourMap getColourMap(bool inner);
    void setColourMap(const ColourMap& map, bool inner);
    
    unsigned int widthInPixels();
    unsigned int heightInPixels();
    
    void setComponentOrder(COMPONENT_ORDER order, bool inner);
    void setComponentOrder(COMPONENT_ORDER order);
    COMPONENT_ORDER getComponentOrder(bool inner);
    void setRedOffset(unsigned int red, bool inner);
    void setGreenOffset(unsigned int green, bool inner);
    void setBlueOffset(unsigned int blue, bool inner);
    void setRedOffset(unsigned int red);
    void setGreenOffset(unsigned int green);
    void setBlueOffset(unsigned int blue);
    unsigned int getRedOffset(bool inner);
    unsigned int getGreenOffset(bool inner);
    unsigned int getBlueOffset(bool inner);
    unsigned int getRedOffset();
    unsigned int getGreenOffset();
    unsigned int getBlueOffset();
    void setOrbitBackground(const QColor& colour);
    void enableOrbitBackground(bool value);
    const QColor &getOrbitBackground();
    bool hasOrbitBackground();
    void setInner(bool inner);
    bool isInner();
    void setStartingCentre(const StringComplex& value);
    void setStartingWidth(const std::string& value);
    void setStartingRotation(const unsigned int value);
    const StringComplex& getStartingCentre();
    const std::string& getStartingWidth();
    int getStartingRotation();
    void setColourMapName(const QString& name, bool inner);
    void setColourMapName(const QString& name);
    void setOrbitColourMapName(const QString& name);
    int getPrecision() const;
    void setPrecision(int value);
    void setCriticalPoints(const VectorUInt& cp);
    void setIgnoreCriticalPoint(unsigned int index, bool ignore);
    bool ignoreCriticalPoint(unsigned int index) const;
    unsigned int getNumberOfCriticalPoints() const;
   
    void setInnerType(INNER_TYPE value);
    INNER_TYPE innerType();
    void setCompoundImage(COMPOUND_IMAGE value);
    COMPOUND_IMAGE compoundImage();

private:
    void readKeys(const QString& file_name);
    int neptuneLevel();
    QString option(const QString& name);
    std::string std_option(const QString& name);
    void getBailoutFractalMetadata(unsigned int ft);
    void getColourMapMetadata(bool inner);
    void getComponentColour(RGB colour, const QString& base_name, ColourMap& map);
    void getColourMethodMetadata(bool inner);
    void getOrbitColourMethod();
    void getTransforms();
    void getParameterMetadata();
    void getCriticalPointsMetadata();
    
    enum CHUNK_TYPE
    {
        CT_TEXT,
        CT_ZTXT,
        CT_IEND,
        CT_IGNORE
    };
    
    CHUNK_TYPE chunk_type(unsigned char* data);
    
    std::string uncompress(std::vector<char>& buffer);

    typedef std::map<int, bool> BoolMap;

    int m_width_pixels;
    int m_height_pixels;
    QString m_name;
    QString m_formula;
    FRACTAL_TYPE m_ft;

    unsigned int m_number_of_complex_parameters;
    StringComplex m_cp[MAX_PARAMETERS];
    VectorUInt m_ignore_critical_point;

    long double m_limit;
    BAILOUT m_bailout;
    ORBIT_PLOTS m_plot_type;
    bool m_orbit_plotting;
    unsigned int m_area;
    unsigned int m_omit;
    StringComplex m_centre;
    std::string m_width;
    int m_rotation;
    unsigned int m_max_iterations;
    unsigned int m_interval;
    bool m_smooth[2];
    bool m_abs_log[2];
    std::string m_method[2];
    long double m_scale[2];
    INNER_TYPE m_inner_type;
    COMPOUND_IMAGE m_compound_image;
    
    char m_orbit_colour_type;
    unsigned int m_red_orbit_offset;
    unsigned int m_green_orbit_offset;
    unsigned int m_blue_orbit_offset;
    long double m_orbit_scale;
    unsigned int m_density;
    
    QString m_colour_map_name[2];
    QString m_orbit_colour_map;
    QColor m_orbit_background;
    bool m_has_orbit_background;
    
    ColourMap m_colour_map[2];
    
    unsigned int m_width_in_pixels;
    unsigned int m_height_in_pixels;
    COMPONENT_ORDER m_order[2];
    unsigned int m_red_offset[2];
    unsigned int m_green_offset[2];
    unsigned int m_blue_offset[2];
    
    int m_index;
    StringComplex m_scentre;
    std::string m_swidth;
    int m_srotation;

    std::map<QString, QString> m_map;
    int m_level;
    bool m_ok;
    int m_precision;
    
    unsigned int m_number_of_critical_points;
};

