// 
// File:   CalculatingUnit.cc
// Author: M.R.Eggleston
// 
// Created on 29 January 2014, 13:41
//

#include "CalculatingUnit.h"
#include "Utilities.h"
#include "mpreal.h"
#include "OrbitTraps.h"


//==============================================================================
// Specialisations
//==============================================================================

template<>
void CalculatingUnit<long double, LongComplex>::set_fractal_parameters()
{
    StringComplex zero;
    
    int p = StaticCalculatingUnit::s_config.getNumberOfComplexParameters();
    auto& conf = StaticCalculatingUnit::s_config;
    m_p0 = stringComplexToLongComplex((p > 0) ? conf.getComplexParameter(0) : zero);
    m_p1 = stringComplexToLongComplex((p > 1) ? conf.getComplexParameter(1) : zero);
    m_p2 = stringComplexToLongComplex((p > 2) ? conf.getComplexParameter(2) : zero);
    m_p3 = stringComplexToLongComplex((p > 3) ? conf.getComplexParameter(3) : zero);
    m_p4 = stringComplexToLongComplex((p > 4) ? conf.getComplexParameter(4) : zero);
    m_p5 = stringComplexToLongComplex((p > 5) ? conf.getComplexParameter(5) : zero);
    m_p6 = stringComplexToLongComplex((p > 6) ? conf.getComplexParameter(6) : zero);
    m_p7 = stringComplexToLongComplex((p > 7) ? conf.getComplexParameter(7) : zero);
    m_p8 = stringComplexToLongComplex((p > 8) ? conf.getComplexParameter(8) : zero);
}

template<>
void CalculatingUnit<MpReal, MpComplex>::set_fractal_parameters()
{
    int precision = StaticCalculatingUnit::s_precision;
    StringComplex zero;
    
    int p = StaticCalculatingUnit::s_config.getNumberOfComplexParameters();
    auto& conf = StaticCalculatingUnit::s_config;
    m_p0 = stringComplexToMpComplex((p > 0) ? conf.getComplexParameter(0) : zero, precision);
    m_p1 = stringComplexToMpComplex((p > 1) ? conf.getComplexParameter(1) : zero, precision);
    m_p2 = stringComplexToMpComplex((p > 2) ? conf.getComplexParameter(2) : zero, precision);
}

template<>
void CalculatingUnit<long double, LongComplex>::setNumbers()
{
    m_zero = 0.0L;
    m_one = 1.0L;
    m_two = 2.0L;
    m_three = 3.0L;
    m_ln2 = std::log(m_two);
    m_max = std::numeric_limits<long double>::max();
    m_pi = mpfr::const_pi(128).toLDouble();
    m_inf = std::numeric_limits<long double>::infinity();
    m_ld_inf = m_inf;
}

template<>
void CalculatingUnit<MpReal, MpComplex>::setNumbers()
{
    int precision = StaticCalculatingUnit::s_precision;
    m_zero = MpReal("0", precision);
    m_one = MpReal("1", precision);
    m_two = MpReal("2", precision);
    m_three = MpReal("3", precision);
    m_max = mpfr::maxval(precision);
    m_ln2 = mpfr::log(m_two);
    m_pi = mpfr::const_pi(precision).toLDouble();
    m_inf = m_one/m_zero;
    m_ld_inf = std::numeric_limits<long double>::infinity();
}

