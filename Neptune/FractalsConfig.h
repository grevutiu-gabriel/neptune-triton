//
// File:   FractalsConfig.h
// Author: M.R.Eggleston
//
// Created on 28 January 2014, 11:19
//

#pragma once

#ifdef NOT_MOC
#include <boost/shared_ptr.hpp>
#endif
#include "ldcomplex.h"
#include <map>
#include <string>
#include <QImage>
#include <QString>
#include "ColourMaps.h"
#include "FractalConfig.h"
#include "TypesAndConstants.h"

class FractalsConfig
{
public:
    struct FractalId
    {
        FRACTAL_TYPE type;
        QString name;
    };
        
    typedef std::vector<FractalId> VectorFractalId;

    FractalsConfig();

    void MetaData(QImage& image);

    VectorFractalId &getAllFractals();
    
    const FractalConfig &getFractalConfig(bool set_colour_maps = true);

    void SetFractal(FractalConfig& f);
    void SetFractalType(FRACTAL_TYPE value);
    FRACTAL_TYPE GetFractalType();

    // Fractal parameters
    void setLimit(long double value);
    long double getLimit();

    //
    const QString &getFormula();
    const QString &getName();
    //
    void defaultStartingComplexPlane();
    //
    void setStartingCentre(const StringComplex &value);
    void setStartingWidth(const std::string &value);
    void setStartingRotation(const unsigned int value);
    const StringComplex &getStartingCentre();
    const std::string &getStartingWidth();
    int getStartingRotation();
    //
    void setCentre(const StringComplex &value);
    void setCentre(const StringComplex &value, bool& changed);
    void setCurrentWidth(const std::string &value);
    void setRotation(const int value);
    const StringComplex &getCentre();
    const std::string &getCurrentWidth();
    int getRotation();

    unsigned int getNumberOfComplexParameters();
    void setComplexParameter(int index, const StringComplex &value);
    const StringComplex &getComplexParameter(int index);
    void resetFractalParameters();

    // Colouring
    void setColourMap(const QString& id);
    void setColourMap(const QString& id, bool inner);
    QString &getColourMap();
    QString &getColourMap(bool inner);
    void setSelectLimit(long double limit);
    long double getSelectLimit();
    void setMethod(const std::string &method);
    void setMethod(bool inner, const std::string &method);
    const std::string &getMethod();
    const std::string &getMethod(bool inner);
    void setColourScale(long double divisions);
    void setColourScale(bool  inner, long double scale);
    long double getColourScale();
    void setOrbitScale(long double scale);
    long double getOrbitScale();
    void setSmooth(bool value);
    void setSmooth(bool inner, bool value);
    bool getSmooth();
    void setAbsLog(bool value);
    void setAbsLog(bool inner, bool value);
    bool getAbsLog();

    void setMaxIterations(int value);
    int getMaxIterations();
    void setDefaultMaxIterations();
    unsigned int getIterationStepSize();
    //
    // Current fractal dimensions - not tied to fractal type.
    void setWidth(unsigned int value);
    long double width();
    int getWidth();
    void setHeight(unsigned int value);
    long double height();
    int getHeight();
    //

    int getOrbitColourInterval();
    char getOrbitColourType();
    void setOrbitColourInterval(unsigned int value);
    void setOrbitColourType(char value);

    void setOrbitPlotting(bool plotting);
    bool isOrbitPlotting();
    void setStartOrbitIteration(int value);
    int getStartOrbitIteration();
    
    void setInner(bool inner);
    bool isInner();
    
    void setBailout(BAILOUT value);
    BAILOUT getBailout();

    void setCalculatingArea(unsigned int size);
    int getCalculatingArea();
    void setOrbitPlots(ORBIT_PLOTS value);
    ORBIT_PLOTS getOrbitPlots();
    void setDensity(unsigned int value);
    unsigned int getDensity();

    void setComponentOrder(COMPONENT_ORDER order, bool inner);
    COMPONENT_ORDER getComponentOrder(bool inner);
    void setComponentOrder(COMPONENT_ORDER order);
    COMPONENT_ORDER getComponentOrder();
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
    int getPrecision();           // for precision override only
    void setPrecision(int value); // for precision override only
    unsigned int getNumberOfCriticalPoints();
    bool ignoreCriticalPoint(int index);
    void setIgnoreCriticalPoint(int index, bool ignore);
    void setInnerType(INNER_TYPE value);
    INNER_TYPE innerType();
    void setCompoundImage(COMPOUND_IMAGE value);
    COMPOUND_IMAGE compoundImage();
    
    std::string colourSignature();
    
private:
    typedef std::map<FRACTAL_TYPE, StringComplex> StringComplexMap;
    typedef std::map<int, StringComplex> ParameterMap;
    typedef std::map<FRACTAL_TYPE, ParameterMap> ParameterMapMap;
    typedef std::vector<unsigned int> VectorUInt;
    typedef std::map<FRACTAL_TYPE, VectorUInt> VectorUIntMap;
    typedef std::map<FRACTAL_TYPE, QString> QStringMap;
    typedef std::map<FRACTAL_TYPE, std::string> StdStringMap;
    typedef std::map<FRACTAL_TYPE, long double> LongDoubleMap;
    typedef std::map<FRACTAL_TYPE, unsigned int> UintMap;
    typedef std::map<FRACTAL_TYPE, int> IntMap;
    typedef std::map<FRACTAL_TYPE, bool> BoolMap;
    typedef std::map<FRACTAL_TYPE, char> CharMap;
    typedef std::map<FRACTAL_TYPE, BAILOUT> BailoutMap;
    typedef std::map<int, std::string> ParameterStringMap;
    typedef std::map<FRACTAL_TYPE, ParameterStringMap> FunctionParameterMapMap;
    typedef std::map<FRACTAL_TYPE, ParameterStringMap> ComplexFunctionParameterMapMap;
    typedef std::map<FRACTAL_TYPE, ORBIT_PLOTS> OrbitPlotsMap;
    typedef std::map<FRACTAL_TYPE, FractalConfig> FractalConfigMap;
    typedef std::map<FRACTAL_TYPE, COMPONENT_ORDER> ComponentOrderMap;
    typedef std::map<FRACTAL_TYPE, QColor> QColorMap;
    typedef std::map<FRACTAL_TYPE, INNER_TYPE> InnerTypeMap;
    typedef std::map<FRACTAL_TYPE, COMPOUND_IMAGE> CompoundImageMap;

    void setCommon(const StringComplex& centre,
                   const std::string& side,
                   int iterations,
                   unsigned int cp);
    void defineBailout(BAILOUT type);
    void defineParameter(int index, const StringComplex& value);

    void defineMc4_3_2_1();
    void defineMc4_3();
    void defineMc4_2();
    void defineMc5_4_3_2_1();
    void defineMcSin();
    void defineMc3_2_1();
    void defineMc4_1();
    void defineMc3_1();
    void defineMc3_2();
    void defineMc4_3_2();
    void defineMc5_1();
    void defineMc5_2();
    void defineMc5_3();
    void defineMc5_4();
    void defineMc5_4_3();
    void defineMc6_1();
    void defineMc6_2();
    void defineMc6_3();
    void defineMc6_4();
    void defineMc6_5();
    void defineMc6_5_4();
    void defineMc6_5_4_3_2_1();
    void defineMc7_1();
    void defineMc7_2();
    void defineMc7_3();
    void defineMc7_4();
    void defineMc7_5();
    void defineMc7_6();
    void defineMc7_6_5();
    void defineMc7_6_5_4_3_2_1();
    void defineMc2_m2();
    void defineMc2_m3();
    void defineMc2_m4();
    void defineMc3_m2();
    void defineMc3_m3();
    void defineMc3_m4();
    void defineMc4_m2();
    void defineMc4_m3();
    void defineMc4_m4();
    void defineMcp2_m2();
    void defineMcp2_m3();
    void defineMcp2_m4();
    void defineMcp3_m2();
    void defineMcp3_m3();
    void defineMcp3_m4();
    void defineMcp4_m2();
    void defineMcp4_m3();
    void defineMcp4_m4();
    void defineMcp3_m1();
    void defineMcp4_m1();
    void defineMc8_1();
    void defineMc8_2();
    void defineMc8_3();
    void defineMc8_4();
    void defineMc8_5();
    void defineMc8_6();
    void defineMc8_7();
    void defineMc8_7_6();
    void defineMc8_7_6_5_4_3_2_1();
    
    void addBailoutFractalMetadata(QImage& image);
    void addCriticalPointsMetadata(QImage& image);
    void addParameterMetadata(QImage& image);
    void addColoursMetadata(QImage& image);
    void addOrbitColoursMetadata(QImage& image);
    void addColourMapMetadata(QImage& image, QString prefix, QString map_name);
    void addComponentColours(QImage& image, RGB colour, const QString& prefix, ColourMap& map);
    
    static QStringMap m_formula;
    static QStringMap m_name;

    // Default starting complex plane
    static StringComplexMap m_default_scentre;
    static StdStringMap m_default_swidth;
    // Starting complex plane
    static StringComplexMap m_scentre;
    static StdStringMap m_swidth;
    // Current complex plane
    static StringComplexMap m_ccentre;
    static StdStringMap m_cwidth;
    // Limit
    static LongDoubleMap m_limit;

    static long double m_width;
    static long double m_height;

    // Colouring outer -> index 0 inner -> index 1
    static QStringMap m_colour_map[2];
    static StdStringMap m_method[2];
    static LongDoubleMap m_colour_scale[2];
    static BoolMap m_abs_log[2];
    static BoolMap m_smooth[2];

    static IntMap m_max_iterations;
    static IntMap m_default_max_iterations;
    static UintMap m_iteration_step_size;

    // Rotation values.
    static IntMap m_crotation;
    static IntMap m_srotation;

    // Bailout
    static BailoutMap m_bailout;
    static BailoutMap m_default_bailout;

    static IntMap m_interval;
    static CharMap m_orbit_colour_type;
    static UintMap m_default_interval;
    static CharMap m_default_orbit_colour_type;
    static BoolMap m_orbit_plotting;
    static QStringMap m_orbit_colour_map;
    static ComponentOrderMap m_order[2];
    static ComponentOrderMap m_orbit_order;
    static UintMap m_red_offset[2];
    static UintMap m_green_offset[2];
    static UintMap m_blue_offset[2];

    static QStringMap m_orbit_colour;
    static UintMap m_red_orbit_offset;
    static UintMap m_green_orbit_offset;
    static UintMap m_blue_orbit_offset;
    static IntMap m_calculating_area;
    static LongDoubleMap m_orbit_scale;
    static OrbitPlotsMap m_orbit_plots;
    static IntMap m_start_orbit_iteration;
    static UintMap m_density;
    static BoolMap m_has_orbit_background;
    static QColorMap m_background_colour;
    static VectorUIntMap m_critical_point;
    static UintMap m_number_of_critical_points;
    static ParameterMapMap m_parameters;
    static ParameterMapMap m_default_parameters;
    static UintMap m_number_of_parameters;
    static InnerTypeMap m_inner_type;
    static CompoundImageMap m_compound_image;
    
    static FractalConfig m_fractal;
    
    static IntMap m_precision;
    
    FRACTAL_TYPE m_type;
    int m_index;
    VectorFractalId m_fractal_ids;
    boost::shared_ptr<ColourMaps> m_colour_maps;
};

