//
// File:   OptionComboboxes.h
// Author: M.R.Eggleston
//
// Created on 24 May 2011, 14:51
//

#pragma once

#include <QComboBox>
#include "FractalsConfig.h"
#include "OrbitTraps.h"
//#include "OptionCombobox.h"

class ColouringCombobox : public QComboBox
{
    Q_OBJECT
public:
    ColouringCombobox(QWidget* parent = nullptr);

    COLOURING getValue();
    void setValue(COLOURING value);
private:
    ColouringCombobox(const ColouringCombobox &other);
};

enum METHOD
{
    FIXED_COLOUR,
    ITERATION,
    FINAL_MAGNITUDE,
    MAGNITUDE,
    MAGNITUDE_CHANGE,
    FINAL_ANGLE,
    ANGLE,
    ANGLE_CHANGE,
    GAUSSIAN,
    ORBIT_TRAP,
    ORBIT_TRAP_CHANGE,
    RATIO,
    RATIO_CHANGE
};

class MethodCombobox : public QComboBox
{
    Q_OBJECT
public:
    MethodCombobox(QWidget* parent = nullptr);

    METHOD getValue();
    void setValue(METHOD value);
private:
    MethodCombobox(const MethodCombobox &other);
};

enum GAUSSIAN_TYPE
{
    ROUND,
    CEILING,
    FLOOR,
    TRUNCATE
};

class GaussianTypeCombobox : public QComboBox
{
    Q_OBJECT
public:
    GaussianTypeCombobox(QWidget* parent = nullptr);

    GAUSSIAN_TYPE getValue();
    void setValue(GAUSSIAN_TYPE value);
private:
    GaussianTypeCombobox(const GaussianTypeCombobox &other);
};

enum GAUSSIAN_METHOD
{
    MIN_DISTANCE,
    MIN_DISTANCE_ITERATION,
    MIN_DISTANCE_ANGLE,
    MAX_DISTANCE,
    MAX_DISTANCE_ITERATION,
    MAX_DISTANCE_ANGLE,
    AVERAGE_DISTANCE,
    MIN_ANGLE,
    AVERAGE_ANGLE,
    MAX_ANGLE,
    MAX_MIN_DISTANCE_RATIO
};

class GaussianMethodCombobox : public QComboBox
{
    Q_OBJECT
public:
    GaussianMethodCombobox(QWidget* parent = nullptr);

    GAUSSIAN_METHOD getValue();
    void setValue(GAUSSIAN_METHOD value);
private:
    GaussianMethodCombobox(const GaussianMethodCombobox &other);
};

enum FINAL_MAGNITUDE_TYPE
{
    MAG_NORM,
    MAG_SMALLER,
    MAG_GREATER,
    MAG_REAL,
    MAG_ABS_REAL,
    MAG_IMAG,
    MAG_ABS_IMAG,
    MAG_COMBINED,
    MAG_ABS_COMBINED,
    MAG_PRODUCT,
    MAG_ABS_PRODUCT
};

class FinalMagnitudeTypeCombobox : public QComboBox
{
    Q_OBJECT
public:
    FinalMagnitudeTypeCombobox(QWidget* parent = nullptr);

    FINAL_MAGNITUDE_TYPE getValue();
    void setValue(FINAL_MAGNITUDE_TYPE value);
private:
    FinalMagnitudeTypeCombobox(const FinalMagnitudeTypeCombobox &other);
};

enum STATISTICS_OPTION_TYPE
{
    SO_MINIMUM,
    SO_ITER_AT_MINIMUM,
    SO_MAXIMUM,
    SO_ITER_AT_MAXIMUM,
    SO_RANGE,
    SO_AVERAGE,
    SO_VARIANCE,
    SO_STANDARD_DEVIATION,
    SO_COEF_VARIATION,
    SO_FRACTAL_DIMENSION,
    SO_EXP_SUM,
    SO_EXP_INVERSE_SUM
};

class StatisticsOptionTypeCombobox : public QComboBox
{
    Q_OBJECT
public:
    StatisticsOptionTypeCombobox(QWidget* parent = nullptr);

    STATISTICS_OPTION_TYPE getValue();
    void setValue(STATISTICS_OPTION_TYPE value);
private:
    StatisticsOptionTypeCombobox(const StatisticsOptionTypeCombobox &other);
};

class OrbitColourCombobox : public QComboBox
{
    Q_OBJECT
public:
    OrbitColourCombobox(QWidget* parent = nullptr);

    ORBIT_COLOUR getValue();
    void setValue(ORBIT_COLOUR value);
private:
    OrbitColourCombobox(const OrbitColourCombobox &other);
};


class OrbitTrapTypeCombobox : public QComboBox
{
    Q_OBJECT    
public:
    OrbitTrapTypeCombobox(QWidget* parent = nullptr);

    OrbitTrap::TRAP_TYPE getValue();
    void setValue(OrbitTrap::TRAP_TYPE value);

private:
    OrbitTrapTypeCombobox(const OrbitTrapTypeCombobox &other);

    std::vector<OrbitTrap::TRAP_TYPE> m_values;
};


enum RATIO_PART_TYPE
{
    RP_SMALLER,
    RP_LARGER,
    RP_ABS_REAL,
    RP_ABS_IMAG,
    RP_ABS_SUM,
    RP_ABS_DIFF,
    RP_SUM_ABS,
    RP_ABS_DIFF_ABS,
    RP_PRODUCT,
    RP_MODULUS,
    RP_NORMAL
};

class RatioPartTypeCombobox : public QComboBox
{
    Q_OBJECT
public:
    RatioPartTypeCombobox(QWidget* parent = nullptr);

    RATIO_PART_TYPE getValue();
    void setValue(RATIO_PART_TYPE value);
private:
    RatioPartTypeCombobox(const RatioPartTypeCombobox &other);
};

class DivergeBailoutConditionCombobox : public QComboBox
{
    Q_OBJECT
public:
    DivergeBailoutConditionCombobox(QWidget* parent = nullptr);

    BAILOUT getValue();
    void setValue(BAILOUT value);
private:
    DivergeBailoutConditionCombobox(const DivergeBailoutConditionCombobox &other);
};

class OrbitPlotCombobox : public QComboBox
{
    Q_OBJECT
public:
    OrbitPlotCombobox(QWidget* parent = nullptr);

    ORBIT_PLOTS getValue();
    void setValue(ORBIT_PLOTS value);
private:
    OrbitPlotCombobox(const OrbitColourCombobox &other);
};

enum EXPORT_FILES
{
    EF_SCM,
    EF_UGR
};

class ExportCombobox : public QComboBox
{
public:
    ExportCombobox(QWidget* parent = nullptr);

    EXPORT_FILES getValue();
    void setValue(EXPORT_FILES value);
private:
    ExportCombobox(const ExportCombobox &other);
};

enum NAVIGATION
{
    N_QUICK,
    N_ZOOM_IN,
    N_CENTRE_IN,
    N_CENTRE_OUT,
    N_OFF
};

class NavigationCombobox : public QComboBox
{
public:
    NavigationCombobox(QWidget* parent = nullptr);
    
    NAVIGATION getValue();
    void setValue(NAVIGATION value);
private:
    NavigationCombobox(const NavigationCombobox& other);
};

enum COLOUR_MAP_EDIT
{
    CM_AUTO,
    CM_MANUAL_1,
    CM_MANUAL_2,
};

class ColourMapEditComboBox : public QComboBox
{
public:
   ColourMapEditComboBox(QWidget* parent = nullptr);

   COLOUR_MAP_EDIT getValue();
   void setValue(COLOUR_MAP_EDIT value);
private:
    ColourMapEditComboBox(const ColourMapEditComboBox& other);
};

class WaveComboBox : public QComboBox
{
public:
   WaveComboBox(QWidget* parent = nullptr);

   Waveform::WAVE getValue();
   void setValue(Waveform::WAVE value);
private:
    WaveComboBox(const WaveComboBox& other);
};

        
class ComponentOrderComboBox : public QComboBox
{
public:
   ComponentOrderComboBox(QWidget* parent = nullptr);

   COMPONENT_ORDER getValue();
   void setValue(COMPONENT_ORDER value);
private:
    ComponentOrderComboBox(const ComponentOrderComboBox& other);
};

class InnerTypeComboBox : public QComboBox
{
public:
   InnerTypeComboBox(QWidget* parent = nullptr);

   INNER_TYPE getValue();
   void setValue(INNER_TYPE value);
private:
    InnerTypeComboBox(const InnerTypeComboBox& other);
};

class CompoundImageComboBox : public QComboBox
{
public:
   CompoundImageComboBox(QWidget* parent = nullptr);

   COMPOUND_IMAGE getValue();
   void setValue(COMPOUND_IMAGE value);
private:
    CompoundImageComboBox(const CompoundImageComboBox& other);
};

