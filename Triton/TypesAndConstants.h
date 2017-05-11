//
// File:   TypesAndConstants.h
// Author: M.R.Eggleston
//
// Created on 29 July 2014, 11:57
//

#pragma once

#include <map>
#include <vector>
#include <QString>
#include <QColor>
#include "ldcomplex.h"
#include "mpcomplex.h"

enum FRACTAL_TYPE
{
    F_MC_4_3_2_1,
    F_MC_4_3,
    F_MC_4_2,
    F_MC_5_4_3_2_1,
    F_MC_SIN,
    F_MC_3_2_1,
    F_MC_4_1,
    F_MC_3_1,
    F_MC_3_2,
    F_MC_4_3_2,
    F_MC_5_1,
    F_MC_5_2,
    F_MC_5_3,
    F_MC_5_4,
    F_MC_5_4_3,
    F_MC_6_1,
    F_MC_6_2,
    F_MC_6_3,
    F_MC_6_4,
    F_MC_6_5,
    F_MC_6_5_4,
    F_MC_6_5_4_3_2_1,
    F_MC_7_1,
    F_MC_7_2,
    F_MC_7_3,
    F_MC_7_4,
    F_MC_7_5,
    F_MC_7_6,
    F_MC_7_6_5,
    F_MC_7_6_5_4_3_2_1,
    F_MC_2_M2,
    F_MC_2_M3,
    F_MC_2_M4,
    F_MC_3_M2,
    F_MC_3_M3,
    F_MC_3_M4,
    F_MC_4_M2,
    F_MC_4_M3,
    F_MC_4_M4,
    F_MC_P_C_2_M2,
    F_MC_P_C_2_M3,
    F_MC_P_C_2_M4,
    F_MC_P_C_3_M2,
    F_MC_P_C_3_M3,
    F_MC_P_C_3_M4,
    F_MC_P_C_4_M2,
    F_MC_P_C_4_M3,
    F_MC_P_C_4_M4,
    F_MC_P_C_3_M1,
    F_MC_P_C_4_M1,
    F_MC_8_1,
    F_MC_8_2,
    F_MC_8_3,
    F_MC_8_4,
    F_MC_8_5,
    F_MC_8_6,
    F_MC_8_7,
    F_MC_8_7_6,
    F_MC_8_7_6_5_4_3_2_1
};
    
enum BAILOUT
{
    B_NONE,
    B_ABS,
    B_NORM,
    B_IMAG,
    B_IMAG_SQUARED,
    B_REAL,
    B_REAL_SQUARED,
    B_ABS_PRODUCT,
    B_MANHATTAN,
    B_MANHATTANISH,
    B_ABS_MANHATTANISH,
    B_MAX_SQUARED,
    B_MIN_SQUARED,
    B_ABS_DIFF
};

enum ORBIT_PLOTS
{
    OP_ALL,
    OP_ESCAPED,
    OP_CAPTIVE
};

enum ORBIT_COLOUR
{
    OC_ACCUMULATION,
    OC_SQRT,
    OC_AVERAGE,
    OC_LOG
};

enum COLOURING
{
    OUTER,
    INNER
};

enum RGB
{
    RGB_RED,
    RGB_GREEN,
    RGB_BLUE
};

enum COLOUR_TYPE
{
    CT_RED,
    CT_GREEN,
    CT_BLUE,
    CT_ALL
};

enum COMPONENT_ORDER
{
    CO_RGB,
    CO_RBG,
    CO_GRB,
    CO_GBR,
    CO_BRG,
    CO_BGR
};

enum INNER_TYPE
{
    IT_OVERLAP,
    IT_ONE_OR_MORE,
    IT_ALL
};

enum COMPOUND_IMAGE
{
    CI_AVERAGE,
    CI_HIGHEST
};

typedef ld::complex LongComplex;
typedef mpfr::mpcomplex MpComplex;
typedef mpfr::mpreal MpReal;

typedef std::vector<QRgb> VectorColour;
typedef std::vector<int> VectorInt;
typedef std::map<int, int> IntMap;
typedef std::map<int, LongComplex> ComplexMap;


struct StringComplex
{
    StringComplex() : r("0"), i("0") {};
    StringComplex(const std::string& s1, const std::string& s2) : r(s1), i(s2) {};
    std::string r;
    std::string i;
};

typedef std::map<int, long double> LongDoubleMap;
typedef std::map<int, LongComplex> ComplexMap;
typedef std::map<int, StringComplex> StringComplexMap;
typedef std::vector<StringComplex> VectorStringComplex;
typedef std::vector<LongComplex> VectorLongComplex;
typedef std::vector<MpComplex> VectorMpComplex;
typedef std::vector<VectorLongComplex> ComplexMatrix;
typedef std::vector<bool> VectorBool;
typedef std::vector<unsigned int> VectorUInt;

struct Statistics
{
    long escape;
    long double z_r;
    long double z_i;
    LongComplex min_dist;
    LongComplex max_dist;
    long double minimum;
    long double maximum;
    long double angle_total;
    long double square_total;
    long double total;
    int iteration_at_min;
    int iteration_at_max;
    long double es_sum1;
    long double es_sum2;
    long double last;     // Last value as a magnitude or an angle, first multiple data
    long double es2_last;
};

struct ResultsSet
{
    bool done;
    unsigned int escaped;
    long double c_r;
    long double c_i;
    Statistics set[2];
};

typedef std::vector<ResultsSet> VectorResultsSet;

enum COLOURING_TYPE // for orbit plotting
{
    ACCUMULATION,
    LAST,
    AVERAGE
};

struct ColourAccumulator    // for orbit plotting
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
};

typedef std::vector<ColourAccumulator> VectorColourAccumulator;

#define MAX_PARAMETERS 8

#define BASE_PRECISION (sizeof(long double) == 8 ? 64 : 80)

#define LEVEL 9
