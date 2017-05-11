// 
// File:   CriticalPointFinder.h
// Author: M.R.Eggleston
//
// Created on 07 August 2014, 16:30
//

#pragma once

#include <map>
#include <vector>
#include "FractalConfig.h"
#include "TypesAndConstants.h"
#include "Utilities.h"

template<class T>
class CriticalPointFinder
{
public:
    CriticalPointFinder(); 
    CriticalPointFinder(const FractalConfig& conf, int precision = BASE_PRECISION);
    CriticalPointFinder(const CriticalPointFinder& orig);
    const CriticalPointFinder &operator=(const CriticalPointFinder &other);
    virtual ~CriticalPointFinder();
    
    // the find routines returns the number of critical points in use
    unsigned int find(T& c);
    unsigned int find();
    T& operator[](unsigned int i);
    LongComplex value(unsigned int i);
    // the number of critical points that exist

private:
    void mc_4_3_2_1();
    void mc_4_3();
    void mc_4_2();
    void mc_5_4_3_2_1();
    void mc_sin();
    void mc_3_2_1();
    void mc_4_1();
    void mc_3_1();
    void mc_3_2();
    void mc_4_3_2();
    void mc_5_1();
    void mc_5_2();
    void mc_5_3();
    void mc_5_4();
    void mc_5_4_3();
    void mc_6_1();
    void mc_6_2();
    void mc_6_3();
    void mc_6_4();
    void mc_6_5();
    void mc_6_5_4();
    void mc_6_5_4_3_2_1();
    void mc_7_1();
    void mc_7_2();
    void mc_7_3();
    void mc_7_4();
    void mc_7_5();
    void mc_7_6();
    void mc_7_6_5();
    void mc_7_6_5_4_3_2_1();
    void mc_2_m2();
    void mc_2_m3();
    void mc_2_m4();
    void mc_3_m2();
    void mc_3_m3();
    void mc_3_m4();
    void mc_4_m2();
    void mc_4_m3();
    void mc_4_m4();
    void mc_p_c_2_m2();
    void mc_p_c_2_m3();
    void mc_p_c_2_m4();
    void mc_p_c_3_m2();
    void mc_p_c_3_m3();
    void mc_p_c_3_m4();
    void mc_p_c_4_m2();
    void mc_p_c_4_m3();
    void mc_p_c_4_m4();
    void mc_p_c_3_m1();
    void mc_p_c_4_m1();
    void mc_8_1();
    void mc_8_2();
    void mc_8_3();
    void mc_8_4();
    void mc_8_5();
    void mc_8_6();
    void mc_8_7();
    void mc_8_7_6();
    void mc_8_7_6_5_4_3_2_1();
    
    void filter_points();

    FRACTAL_TYPE m_type;
    VectorUInt m_ignore;
    VectorLongComplex m_p;
    VectorLongComplex m_roots;
    std::vector<T> m_points;
    VectorLongComplex m_coefficients;
    T m_value;
    int m_precision;
};

template<class T>
CriticalPointFinder<T>::CriticalPointFinder()
:
m_type(F_MC_4_3_2_1),
m_precision(BASE_PRECISION)
{
}

template<class T>
CriticalPointFinder<T>::CriticalPointFinder(const FractalConfig& conf, int precision)
:
m_type(conf.GetFractalType()),
m_precision(precision)
{
    unsigned int size = conf.getNumberOfComplexParameters();
    m_p.resize(size);
    for (auto i = 0U; i < size; i++)
    {
        auto p = conf.getComplexParameter(i);
        m_p[i] = stringComplexToLongComplex(p);
    }
    size = conf.getNumberOfCriticalPoints();
    m_ignore.resize(size);
    for (auto i = 0U; i < size; i++)
    {
        m_ignore[i] = (conf.ignoreCriticalPoint(i) ? 1U : 0U);
    }
}

template<class T>
CriticalPointFinder<T>::CriticalPointFinder(const CriticalPointFinder& other)
{
    operator=(other);
}

template<class T>
const CriticalPointFinder<T>& CriticalPointFinder<T>::operator=(const CriticalPointFinder& other)
{
    m_type = other.m_type;
    m_p = other.m_p;
    m_ignore = other.m_ignore;
    m_precision = other.m_precision;
    return *this;
}

template<class T>
CriticalPointFinder<T>::~CriticalPointFinder()
{
}

template<class T>
unsigned int CriticalPointFinder<T>::find()
{
    static long double LIMIT = 1.0e-14;
    switch (m_type)
    {
    case F_MC_4_3_2_1: mc_4_3_2_1(); break;
    case F_MC_4_3: mc_4_3(); break;
    case F_MC_4_2: mc_4_2(); break;
    case F_MC_5_4_3_2_1: mc_5_4_3_2_1(); break;
    case F_MC_SIN: mc_sin(); break;
    case F_MC_3_2_1: mc_3_2_1(); break;
    case F_MC_4_1: mc_4_1(); break;
    case F_MC_3_1: mc_3_1(); break;
    case F_MC_3_2: mc_3_2(); break;
    case F_MC_4_3_2: mc_4_3_2(); break;
    case F_MC_5_1: mc_5_1(); break;
    case F_MC_5_2: mc_5_2(); break;
    case F_MC_5_3: mc_5_3(); break;
    case F_MC_5_4: mc_5_4(); break;
    case F_MC_5_4_3: mc_5_4_3(); break;
    case F_MC_6_1: mc_6_1(); break;
    case F_MC_6_2: mc_6_2(); break;
    case F_MC_6_3: mc_6_3(); break;
    case F_MC_6_4: mc_6_4(); break;
    case F_MC_6_5: mc_6_5(); break;
    case F_MC_6_5_4: mc_6_5_4(); break;
    case F_MC_6_5_4_3_2_1: mc_6_5_4_3_2_1(); break;
    case F_MC_7_1: mc_7_1(); break;
    case F_MC_7_2: mc_7_2(); break;
    case F_MC_7_3: mc_7_3(); break;
    case F_MC_7_4: mc_7_4(); break;
    case F_MC_7_5: mc_7_5(); break;
    case F_MC_7_6: mc_7_6(); break;
    case F_MC_7_6_5: mc_7_6_5(); break;
    case F_MC_7_6_5_4_3_2_1: mc_7_6_5_4_3_2_1(); break;
    case F_MC_2_M2: mc_2_m2(); break;
    case F_MC_2_M3: mc_2_m3(); break;
    case F_MC_2_M4: mc_2_m4(); break;
    case F_MC_3_M2: mc_3_m2(); break;
    case F_MC_3_M3: mc_3_m3(); break;
    case F_MC_3_M4: mc_3_m4(); break;
    case F_MC_4_M2: mc_4_m2(); break;
    case F_MC_4_M3: mc_4_m3(); break;
    case F_MC_4_M4: mc_4_m4(); break;
    case F_MC_P_C_2_M2: mc_p_c_2_m2(); break;
    case F_MC_P_C_2_M3: mc_p_c_2_m3(); break;
    case F_MC_P_C_2_M4: mc_p_c_2_m4(); break;
    case F_MC_P_C_3_M2: mc_p_c_3_m2(); break;
    case F_MC_P_C_3_M3: mc_p_c_3_m3(); break;
    case F_MC_P_C_3_M4: mc_p_c_3_m4(); break;
    case F_MC_P_C_4_M2: mc_p_c_4_m2(); break;
    case F_MC_P_C_4_M3: mc_p_c_4_m3(); break;
    case F_MC_P_C_4_M4: mc_p_c_4_m4(); break;
    case F_MC_P_C_3_M1: mc_p_c_3_m1(); break;
    case F_MC_P_C_4_M1: mc_p_c_4_m1(); break;
    case F_MC_8_1: mc_8_1(); break;
    case F_MC_8_2: mc_8_2(); break;
    case F_MC_8_3: mc_8_3(); break;
    case F_MC_8_4: mc_8_4(); break;
    case F_MC_8_5: mc_8_5(); break;
    case F_MC_8_6: mc_8_6(); break;
    case F_MC_8_7: mc_8_7(); break;
    case F_MC_8_7_6: mc_8_7_6(); break;
    case F_MC_8_7_6_5_4_3_2_1: mc_8_7_6_5_4_3_2_1(); break;
    default: break;
    }
    for (auto it = m_roots.begin(); it != m_roots.end(); it++)
    {
        auto& value = *it;
        if (std::abs(value.real()) < LIMIT)
        {
            value.real(0.0L);
        }
        if (std::abs(value.imag()) < LIMIT)
        {
            value.imag(0.0L);
        }
    }
    filter_points();
    return m_points.size();
}

template<class T>
LongComplex CriticalPointFinder<T>::value(unsigned int index)
{
    unsigned int size = m_roots.size();
    return (index < size ? m_roots[index] : LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_4_3_2_1()
{
    m_coefficients.resize(4);
    m_coefficients[0] = m_p[3];
    m_coefficients[1] = 2.0L*m_p[2];
    m_coefficients[2] = 3.0L*m_p[1];
    m_coefficients[3] = 4.0L*m_p[0];
    polyroots(m_coefficients, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_4_3()
{
    m_roots.resize(2);
    m_roots[0] = LongComplex();
    m_roots[1] = (-3.0*m_p[1])/(4.0L*m_p[0]);
}

template<class T>
void CriticalPointFinder<T>::mc_4_2()
{
    roots(-(2.0*m_p[1])/(4.0L*m_p[0]), 2, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_5_4_3_2_1()
{
    m_coefficients.resize(5);
    m_coefficients[0] = m_p[4];
    m_coefficients[1] = 2.0L*m_p[3];
    m_coefficients[2] = 3.0L*m_p[2];
    m_coefficients[3] = 4.0L*m_p[1];
    m_coefficients[4] = 5.0L*m_p[0];
    polyroots(m_coefficients, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_sin()
{
    m_roots.resize(2);
    m_roots[0] = LongComplex(-1.5707963268L, 0.0L);
    m_roots[1] = LongComplex(1.5707963268L, 0.0L);
}

template<class T>
void CriticalPointFinder<T>::mc_3_2_1()
{
    quadratic(3.0L*m_p[0], 2.0L*m_p[1], m_p[2], m_roots); 
}

template<class T>
void CriticalPointFinder<T>::mc_4_1()
{
    roots(-m_p[1]/(4.0L*m_p[0]), 3, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_3_1()
{
    roots(-m_p[1]/(3.0L*m_p[0]), 2, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_3_2()
{
    m_roots.resize(2);
    m_roots[0] = LongComplex();
    m_roots[1] = (-2.0*m_p[1])/(3.0L*m_p[0]);
}

template<class T>
void CriticalPointFinder<T>::mc_4_3_2()
{
    quadratic(4.0L*m_p[0], 3.0L*m_p[1], 2.0L*m_p[2], m_roots); 
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_5_1()
{
    roots(-m_p[1]/(5.0L*m_p[0]), 4, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_5_2()
{
    roots(-(2.0L*m_p[1])/(5.0L*m_p[0]), 3, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_5_3()
{
    roots(-(3.0L*m_p[1])/(5.0L*m_p[0]), 2, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_5_4()
{
    m_roots.resize(2);
    m_roots[0] = LongComplex();
    m_roots[1] = (-4.0*m_p[1])/(5.0L*m_p[0]);
}

template<class T>
void CriticalPointFinder<T>::mc_5_4_3()
{
    quadratic(5.0L*m_p[0], 4.0L*m_p[1], 3.0L*m_p[2], m_roots); 
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_6_1()
{
    roots(-m_p[1]/(6.0L*m_p[0]), 5, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_6_2()
{
    roots(-(2.0L*m_p[1])/(6.0L*m_p[0]), 4, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_6_3()
{
    roots(-(3.0L*m_p[1])/(6.0L*m_p[0]), 3, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_6_4()
{
    roots(-(4.0L*m_p[1])/(6.0L*m_p[0]), 2, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_6_5()
{
    m_roots.resize(2);
    m_roots[0] = LongComplex();
    m_roots[0] = (-5.0*m_p[1])/(6.0L*m_p[0]);
}

template<class T>
void CriticalPointFinder<T>::mc_6_5_4()
{
    quadratic(6.0L*m_p[0], 5.0L*m_p[1], 4.0L*m_p[2], m_roots); 
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_6_5_4_3_2_1()
{
    m_coefficients.resize(6);
    m_coefficients[0] = m_p[5];
    m_coefficients[1] = 2.0L*m_p[4];
    m_coefficients[2] = 3.0L*m_p[3];
    m_coefficients[3] = 4.0L*m_p[2];
    m_coefficients[4] = 5.0L*m_p[1];
    m_coefficients[5] = 6.0L*m_p[0];
    polyroots(m_coefficients, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_7_1()
{
    roots(-m_p[1]/(7.0L*m_p[0]), 6, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_7_2()
{
    roots(-(2.0L*m_p[1])/(7.0L*m_p[0]), 5, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_7_3()
{
    roots(-(3.0L*m_p[1])/(7.0L*m_p[0]), 4, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_7_4()
{
    roots(-(4.0L*m_p[1])/(7.0L*m_p[0]), 3, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_7_5()
{
    roots(-(5.0L*m_p[1])/(7.0L*m_p[0]), 2, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_7_6()
{
    m_roots.resize(2);
    m_roots[0] = LongComplex();
    m_roots[0] = (-6.0*m_p[1])/(7.0L*m_p[0]);
}

template<class T>
void CriticalPointFinder<T>::mc_7_6_5()
{
    quadratic(7.0L*m_p[0], 6.0L*m_p[1], 5.0L*m_p[2], m_roots); 
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_7_6_5_4_3_2_1()
{
    m_coefficients.resize(7);
    m_coefficients[0] = m_p[6];
    m_coefficients[1] = 2.0L*m_p[5];
    m_coefficients[2] = 3.0L*m_p[4];
    m_coefficients[3] = 4.0L*m_p[3];
    m_coefficients[4] = 5.0L*m_p[2];
    m_coefficients[5] = 6.0L*m_p[1];
    m_coefficients[6] = 7.0L*m_p[0];
    ::polyroots(m_coefficients, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_2_m2()
{
    roots(m_p[1]/m_p[0], 4, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_2_m3()
{
    roots((3.0L*m_p[1])/(2.0L*m_p[0]), 5, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_2_m4()
{
    roots((4.0L*m_p[1])/(2.0L*m_p[0]), 6, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_3_m2()
{
    roots((2.0L*m_p[1])/(3.0L*m_p[0]), 5, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_3_m3()
{
    roots(m_p[1]/m_p[0], 6, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_3_m4()
{
    roots((4.0L*m_p[1])/(3.0L*m_p[0]), 7, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_4_m2()
{
    roots((2.0L*m_p[1])/(4.0L*m_p[0]), 6, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_4_m3()
{
    roots((3.0L*m_p[1])/(4.0L*m_p[0]), 7, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_4_m4()
{
    roots(m_p[1]/m_p[0], 8, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_p_c_2_m2()
{
    roots(m_p[2]/m_p[1], 4, m_roots);
    for (auto it = m_roots.begin(); it != m_roots.end(); it++)
    {
        *it -= m_p[0];
    }
}

template<class T>
void CriticalPointFinder<T>::mc_p_c_2_m3()
{
    roots((3.0L*m_p[2])/(2.0L*m_p[1]), 5, m_roots);
    for (auto it = m_roots.begin(); it != m_roots.end(); it++)
    {
        *it -= m_p[0];
    }
}

template<class T>
void CriticalPointFinder<T>::mc_p_c_2_m4()
{
    roots((4.0L*m_p[2])/(2.0L*m_p[1]), 6, m_roots);
    for (auto it = m_roots.begin(); it != m_roots.end(); it++)
    {
        *it -= m_p[0];
    }
}

template<class T>
void CriticalPointFinder<T>::mc_p_c_3_m2()
{
    roots((2.0L*m_p[2])/(3.0L*m_p[1]), 5, m_roots);
    for (auto it = m_roots.begin(); it != m_roots.end(); it++)
    {
        *it -= m_p[0];
    }
}

template<class T>
void CriticalPointFinder<T>::mc_p_c_3_m3()
{
    roots(m_p[2]/m_p[1], 6, m_roots);
    for (auto it = m_roots.begin(); it != m_roots.end(); it++)
    {
        *it -= m_p[0];
    }
}

template<class T>
void CriticalPointFinder<T>::mc_p_c_3_m4()
{
    roots((4.0*m_p[2])/(3.0L*m_p[1]), 7, m_roots);
    for (auto it = m_roots.begin(); it != m_roots.end(); it++)
    {
        *it -= m_p[0];
    }
}

template<class T>
void CriticalPointFinder<T>::mc_p_c_4_m2()
{
    roots((2.0*m_p[2])/(4.0L*m_p[1]), 6, m_roots);
    for (auto it = m_roots.begin(); it != m_roots.end(); it++)
    {
        *it -= m_p[0];
    }
}

template<class T>
void CriticalPointFinder<T>::mc_p_c_4_m3()
{
    roots((3.0*m_p[2])/(4.0L*m_p[1]), 7, m_roots);
    for (auto it = m_roots.begin(); it != m_roots.end(); it++)
    {
        *it -= m_p[0];
    }
}

template<class T>
void CriticalPointFinder<T>::mc_p_c_4_m4()
{
    roots(m_p[2]/m_p[1], 8, m_roots);
    for (auto it = m_roots.begin(); it != m_roots.end(); it++)
    {
        *it -= m_p[0];
    }
}

template<class T>
void CriticalPointFinder<T>::mc_p_c_3_m1()
{
    roots(m_p[2]/(4.0L*m_p[1]), 4, m_roots);
    for (auto it = m_roots.begin(); it != m_roots.end(); it++)
    {
        *it -= m_p[0];
    }
}

template<class T>
void CriticalPointFinder<T>::mc_p_c_4_m1()
{
    roots(m_p[2]/(4.0L*m_p[1]), 5, m_roots);
    for (auto it = m_roots.begin(); it != m_roots.end(); it++)
    {
        *it -= m_p[0];
    }
}

template<class T>
void CriticalPointFinder<T>::mc_8_1()
{
    roots(-m_p[1]/(8.0L*m_p[0]), 7, m_roots);
}

template<class T>
void CriticalPointFinder<T>::mc_8_2()
{
    roots(-(2.0L*m_p[1])/(8.0L*m_p[0]), 6, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_8_3()
{
    roots(-(3.0L*m_p[1])/(8.0L*m_p[0]), 5, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_8_4()
{
    roots(-(4.0L*m_p[1])/(8.0L*m_p[0]), 4, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_8_5()
{
    roots(-(5.0L*m_p[1])/(8.0L*m_p[0]), 3, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_8_6()
{
    roots(-(6.0L*m_p[1])/(8.0L*m_p[0]), 2, m_roots);
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_8_7()
{
    m_roots.resize(2);
    m_roots[0] = LongComplex();
    m_roots[1] = (-7.0*m_p[1])/(8.0L*m_p[0]);
}

template<class T>
void CriticalPointFinder<T>::mc_8_7_6()
{
    quadratic(8.0L*m_p[0], 7.0L*m_p[1], 6.0L*m_p[2], m_roots); 
    m_roots.insert(m_roots.begin(), LongComplex());
}

template<class T>
void CriticalPointFinder<T>::mc_8_7_6_5_4_3_2_1()
{
    m_coefficients.resize(8);
    m_coefficients[0] = m_p[7];
    m_coefficients[1] = 2.0L*m_p[6];
    m_coefficients[2] = 3.0L*m_p[5];
    m_coefficients[3] = 4.0L*m_p[4];
    m_coefficients[4] = 5.0L*m_p[3];
    m_coefficients[5] = 6.0L*m_p[2];
    m_coefficients[6] = 7.0L*m_p[1];
    m_coefficients[7] = 8.0L*m_p[0];
    ::polyroots(m_coefficients, m_roots);
}
