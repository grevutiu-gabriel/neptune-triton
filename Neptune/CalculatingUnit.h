//
// File:   CalculatingUnit.h
// Author: M.R.Eggleston
//
// Created on 08 March 2012, 17:50
//

#pragma once

#include <iostream>
#include <limits>
#include "CriticalPointFinder.h"
#include "CalculatingUnitGeneric.h"
#include "StaticCalculatingUnit.h"
#include "Utilities.h"

// template with MpReal or long double passed as the type T ...
// the interface however will have MpReal in and long double out.


template<class T1, class T2>
class CalculatingUnit
{
public:
    CalculatingUnit();
    virtual ~CalculatingUnit();
    
    void setFractal();
    
    void setValues(long iteration, long chunk_size);
    bool calculate();
    void calculateOrbits();
    void enableProgressSignals(bool value);
    void sendProgressSignals(bool value);

    void set_generic(CalculatingUnitGeneric *p);

private:
    CalculatingUnit(const CalculatingUnit& orig);
    
    // These routines use a local ResultsSet which is reused
    void calculate0(); // collect no extra data
    void calculate1(); // collect 1 results set
    void calculate2(); // collect 2 results set

    void collectResults1();
    void collectResults2();
    
    void formula();
    void mc_4_3_2_1();
    void mc_4_3(); // MC 4.3
    void mc_4_2(); // MC 4.2
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

    bool bailout();
    bool gt_abs();
    bool gt_norm();
    bool gt_imag();
    bool gt_imag2();
    bool gt_real();
    bool gt_real2();
    bool gt_manhattan();
    bool gt_manhattanish();
    bool gt_abs_manhattanish();
    bool gt_max2();
    bool gt_min2();
    bool gt_absproduct();
    bool gt_absdiff();
    
    void set_fractal_parameters();
    void setNumbers();
    
    void angleResults1();
    void angleResults2();
    void magnitudeResults1();
    void magnitudeResults2();
    void gaussianResults1();
    void gaussianResults2();
    void orbitTrapResults1();
    void orbitTrapResults2();
    void resultStatistics1(long double value);
    void resultStatistics2(long double value);
    long double magnitude(const LongComplex& z, bool second);
    long double magnitude_option(const LongComplex& z, char option);
    LongComplex gi_ceil(const LongComplex& z);
    LongComplex gi_floor(const LongComplex& z);
    LongComplex gi_round(const LongComplex& z);
    LongComplex gi_trunc(const LongComplex& z);
    void progress(int row);
    void so_progress(long plot, long no_plot);

    T2 m_z;
    T2 m_z2; // z squared
    T2 m_z3; // z cubed
    T2 m_z4; // z to the fourth power
    T2 m_old_z;
    T2 m_c;
    T1 m_c_r;
    T1 m_c_i;
    
    // Working variables for sub values in fractal formula
    T2 m_a;
    T2 m_b;
    T2 m_q; // quotient
    T2 m_d; // divisor, can also be used as a spare working variable
    
    // Non complex Working variables in fractal formula 
    T1 m_x;
    T1 m_y;
    T1 m_r;
    T1 m_i;
    
    // Formula parameters
    T2 m_p0;
    T2 m_p1;
    T2 m_p2;
    T2 m_p3;
    T2 m_p4;
    T2 m_p5;
    T2 m_p6;
    T2 m_p7;
    T2 m_p8;
    
    // Numbers these will need to be reset every time the configuration is loaded
    // by calling setNumbers, this will ensure the correct precision is used
    // when the class is instantiated with the multi-precision type.
    T1 m_zero;
    T1 m_one;
    T1 m_two;
    T1 m_three;
    T1 m_max;
    T1 m_ln2;
    T1 m_inf;
    long double m_ld_inf;
    long double m_pi;

    // Statistics
    ResultsSet *m_rs;
    
    long m_iteration;
    long m_max_iteration;
    
    long double m_z_r;
    long double m_z_i;
    long double m_old_z_r;
    long double m_old_z_i;
    long double m_limit;
    char m_quotient[2];
    char m_divisor[2];
    char m_magnitude_type[2];
    //
    // Orbit data
    VectorLongComplex m_orbit;
    int m_orbit_length;
    
    //
    boost::mutex m_progress_signal_mutex;
    bool m_progress_signals_enabled;
    bool m_send_progress;

    CalculatingUnitGeneric *m_generic;
    bool m_finished;
};

template<class T1, class T2>
CalculatingUnit<T1, T2>::CalculatingUnit()
:
m_iteration(0),
m_progress_signals_enabled(false),
m_send_progress(false),
m_finished(true)
{
}

template<class T1, class T2>
CalculatingUnit<T1, T2>::~CalculatingUnit()
{
}

template<class T1, class T2>
void CalculatingUnit<T1, T2>::setFractal()
{
    setNumbers();
    set_fractal_parameters();
    for (int i = 0; i < 2; i++)
    {
        m_divisor[i] = StaticCalculatingUnit::s_divisor[i];
        m_quotient[i] =  StaticCalculatingUnit::s_quotient[i];
        m_magnitude_type[i] = StaticCalculatingUnit::s_magnitude_type[i];
    }
}

template<class T1, class T2>
void CalculatingUnit<T1, T2>::setValues(long iteration, long chunk_size)
{
    m_iteration = iteration;
    m_max_iteration = iteration + chunk_size;
}

//==============================================================================
// Formulae
//==============================================================================


template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::formula()
{
    switch (StaticCalculatingUnit::s_type)
    {
    default:
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
    }
}

// The formula functions support both Mandelbrot and Julia algorithms if none
// of the parameters is c (by substitution) then it's Julia.




template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_4_3_2_1()
{
    m_z2 = m_p0*m_z;
    m_z2 = (m_z2 + m_p1)*m_z;
    m_z2 = (m_z2 + m_p2)*m_z;
    m_z = (m_z2 + m_p3)*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_4_3()
{
    m_z2 = m_z*m_z;
    m_z = m_p0*m_z2*m_z2 + m_p1*m_z2*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_4_2()
{
    m_z2 = m_z*m_z;
    m_z = m_p0*m_z2*m_z2 + m_p1*m_z2 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_5_4_3_2_1()
{
    m_z2 = m_p0*m_z;
    m_z2 = (m_z2 + m_p1)*m_z;
    m_z2 = (m_z2 + m_p2)*m_z;
    m_z2 = (m_z2 + m_p3)*m_z;
    m_z = (m_z2 + m_p4)*m_z+ m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_sin()
{
    m_z = sin(m_z) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_3_2_1()
{
    m_z2 = m_p0*m_z;
    m_z2 = (m_z2 + m_p1)*m_z;
    m_z = (m_z2 + m_p2)*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_4_1()
{
    m_z2 = m_z*m_z;
    m_z = m_p0*m_z2*m_z2 + m_p1*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_3_1()
{
    m_z2 = m_z*m_z;
    m_z = (m_p0*m_z2 + m_p1)*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_3_2()
{
    m_z2 = m_z*m_z;
    m_z = (m_p0*m_z + m_p1)*m_z2 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_4_3_2()
{
    m_z2 = m_z*m_z;
    m_z = (m_p0*m_z2 + m_p1*m_z + m_p2)*m_z2 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_5_1()
{
    m_z2 = m_z*m_z;
    m_z = (m_p0*m_z2*m_z2 + m_p1)*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_5_2()
{
    m_z2 = m_z*m_z;
    m_z = (m_p0*m_z2*m_z + m_p1)*m_z2 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_5_3()
{
    m_z2 = m_z*m_z;
    m_z = (m_p0*m_z2 + m_p1)*m_z2*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_5_4()
{
    m_z2 = m_z*m_z;
    m_z4 = m_z2*m_z2;
    m_z = (m_p0*m_z + m_p1)*m_z4 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_5_4_3()
{
    m_z2 = m_p0*m_z;
    m_z2 = (m_z2 + m_p1)*m_z;
    m_z = (m_z2 + m_p2)*m_z*m_z*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_6_1()
{
    m_z2 = m_z*m_z;
    m_z = m_p0*m_z2*m_z2*m_z2 + m_p1*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_6_2()
{
    m_z2 = m_z*m_z;
    m_z = (m_p0*m_z2*m_z2 + m_p1)*m_z2 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_6_3()
{
    m_z2 = m_z*m_z;
    m_z = (m_p0*m_z2*m_z2 + m_p1*m_z)*m_z2 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_6_4()
{
    m_z2 = m_z*m_z;
    m_z4 = m_z2*m_z2;
    m_z = (m_p0*m_z2 + m_p1)*m_z4 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_6_5()
{
    m_z2 = m_z*m_z;
    m_z4 = m_z2*m_z2;
    m_z = (m_p0*m_z2 + m_p1*m_z)*m_z4 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_6_5_4()
{
    m_z2 = m_z*m_z;
    m_z3 = m_p0*m_z;
    m_z3 = (m_z3 + m_p1)*m_z;
    m_z = (m_z3 + m_p2)*m_z2*m_z2 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_6_5_4_3_2_1()
{
    m_z2 = m_p0*m_z;
    m_z2 = (m_z2 + m_p1)*m_z;
    m_z2 = (m_z2 + m_p2)*m_z;
    m_z2 = (m_z2 + m_p3)*m_z;
    m_z2 = (m_z2 + m_p4)*m_z;
    m_z = (m_z2 + m_p5)*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_7_1()
{
    m_z2 = m_z*m_z;
    m_z = (m_p0*m_z2*m_z2*m_z2 + m_p1)*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_7_2()
{
    m_z2 = m_z*m_z;
    m_z = (m_p0*m_z2*m_z2*m_z + m_p1)*m_z2 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_7_3()
{
    m_z2 = m_z*m_z;
    m_z = (m_p0*m_z2*m_z2 + m_p1)*m_z2*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_7_4()
{
    m_z2 = m_z*m_z;
    m_z = (m_p0*m_z2*m_z + m_p1)*m_z2*m_z2 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_7_5()
{
    m_z2 = m_z*m_z;
    m_z = (m_p0*m_z2 + m_p1)*m_z2*m_z2*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_7_6()
{
    m_z3 = m_z*m_z*m_z;
    m_z = (m_p0*m_z + m_p1)*m_z3*m_z3 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_7_6_5()
{
    m_z2 = m_z*m_z;
    m_z3 = m_p0*m_z;
    m_z3 = (m_z3 + m_p1)*m_z;
    m_z = (m_z3 + m_p2)*m_z2*m_z2*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_7_6_5_4_3_2_1()
{
    m_z2 = m_p0*m_z;
    m_z2 = (m_z2 + m_p1)*m_z;
    m_z2 = (m_z2 + m_p2)*m_z;
    m_z2 = (m_z2 + m_p3)*m_z;
    m_z2 = (m_z2 + m_p4)*m_z;
    m_z2 = (m_z2 + m_p5)*m_z;
    m_z = (m_z2 + m_p6)*m_z + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_2_m2()
{
    m_z2 = m_z*m_z;
    m_z = m_p0*m_z2 + m_p1/m_z2 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_2_m3()
{
    m_z2 = m_z*m_z;
    m_z = m_p0*m_z2 + m_p1/(m_z2*m_z) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_2_m4()
{
    m_z2 = m_z*m_z;
    m_z = m_p0*m_z2 + m_p1/(m_z2*m_z2) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_3_m2()
{
    m_z2 = m_z*m_z;
    m_z = m_p0*m_z2*m_z + m_p1/(m_z2) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_3_m3()
{
    m_z2 = m_z*m_z*m_z;
    m_z = m_p0*m_z2 + m_p1/(m_z2) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_3_m4()
{
    m_z2 = m_z*m_z*m_z;
    m_z = m_p0*m_z2 + m_p1/(m_z2*m_z) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_4_m2()
{
    m_z2 = m_z*m_z;
    m_z = m_p0*m_z2*m_z2 + m_p1/(m_z2) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_4_m3()
{
    m_z2 = m_z*m_z*m_z;
    m_z = m_p0*m_z2*m_z + m_p1/(m_z2) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_4_m4()
{
    m_z2 = m_z*m_z;
    m_z2 *= m_z2;
    m_z = m_p0*m_z2 + m_p1/(m_z2) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_p_c_2_m2()
{
    m_z += m_p0;
    m_z2 = m_z*m_z;
    m_z = m_c*(m_p1*m_z2 + m_p2/m_z2);
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_p_c_2_m3()
{
    m_z += m_p0;
    m_z2 = m_z*m_z;
    m_z = m_c*(m_p1*m_z2 + m_p2/(m_z2*m_z));
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_p_c_2_m4()
{
    m_z += m_p0;
    m_z2 = m_z*m_z;
    m_z = m_c*(m_p1*m_z2 + m_p2/(m_z2*m_z2));
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_p_c_3_m2()
{
    m_z += m_p0;
    m_z2 = m_z*m_z;
    m_z = m_c*(m_p1*m_z2*m_z + m_p2/m_z2);
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_p_c_3_m3()
{
    m_z += m_p0;
    m_z2 = m_z*m_z*m_z;
    m_z = m_c*(m_p1*m_z2 + m_p2/(m_z2));
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_p_c_3_m4()
{
    m_z += m_p0;
    m_z2 = m_z*m_z;
    m_z = m_c*(m_p1*m_z2*m_z + m_p2/(m_z2*m_z2));
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_p_c_4_m2()
{
    m_z += m_p0;
    m_z2 = m_z*m_z;
    m_z = m_c*(m_p1*m_z2*m_z2 + m_p2/m_z2);
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_p_c_4_m3()
{
    m_z += m_p0;
    m_z2 = m_z*m_z*m_z;
    m_z = m_c*(m_p1*m_z2*m_z + m_p2/(m_z2));
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_p_c_4_m4()
{
    m_z += m_p0;
    m_z2 = m_z*m_z;
    m_z2 *= m_z2;
    m_z = m_c*(m_p1*m_z2 + m_p2/(m_z2));
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_p_c_3_m1()
{
    m_z += m_p0;
    m_z = m_c*(m_p1*m_z*m_z*m_z + m_p2/(m_z));
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_p_c_4_m1()
{
    m_z += m_p0;
    m_z2 = m_z*m_z;
    m_z2 *= m_z2;
    m_z = m_c*(m_p1*m_z2 + m_p2/(m_z));
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_8_1()
{
    m_z2 = m_z*m_z*m_z;   // z^3
    m_z3 = m_z2*m_z2*m_z; // z^7
    m_z = m_z*(m_p0*m_z3 + m_p1) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_8_2()
{
    m_z2 = m_z*m_z;   // z^2
    m_z3 = m_z2*m_z2*m_z2;  // z^6
    m_z = m_z2*(m_p0*m_z3 + m_p1) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_8_3()
{
    m_z2 = m_z*m_z;   // z^2
    m_z3 = m_z2*m_z;  // z^3
    m_z = m_z3*(m_p0*m_z3*m_z2 + m_p1) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_8_4()
{
    m_z2 = m_z*m_z;   // z^2
    m_z3 = m_z2*m_z2;  // z^4
    m_z = m_z3*(m_p0*m_z3 + m_p1) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_8_5()
{
    m_z2 = m_z*m_z;   // z^2
    m_z3 = m_z2*m_z;  // z^3
    m_z = m_z2*m_z3*(m_p0*m_z3 + m_p1) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_8_6()
{
    m_z2 = m_z*m_z;   // z^2
    m_z3 = m_z2*m_z2*m_z2; // z^6
    m_z = m_z3*(m_p0*m_z2 + m_p1) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_8_7()
{
    m_z2 = m_z*m_z;   // z^2
    m_z3 = m_z2*m_z2; // z^4
    m_z2 *= m_z*m_z3; // z^7
    m_z = m_z2*(m_p0*m_z + m_p1) + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_8_7_6()
{
    m_z2 = m_z*m_z*m_z;
    m_z3 = m_p0*m_z;
    m_z3 = (m_z3 + m_p1)*m_z;
    m_z = (m_z3 + m_p2)*m_z2*m_z2 + m_c;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::mc_8_7_6_5_4_3_2_1()
{
    m_z2 = m_p0*m_z;
    m_z2 = (m_z2 + m_p1)*m_z;
    m_z2 = (m_z2 + m_p2)*m_z;
    m_z2 = (m_z2 + m_p3)*m_z;
    m_z2 = (m_z2 + m_p4)*m_z;
    m_z2 = (m_z2 + m_p5)*m_z;
    m_z2 = (m_z2 + m_p6)*m_z;
    m_z = (m_z2 + m_p7)*m_z + m_c;
}

//==============================================================================
// Bailouts - all except no_bailout are specialised
//==============================================================================

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::bailout()
{
    switch (StaticCalculatingUnit::s_bailout_type)
    {
    case B_NONE:
        return false;
    case B_ABS:
        return gt_abs();
    default:
    case B_NORM:
        return gt_norm();
    case B_IMAG:
        return gt_imag();
    case B_IMAG_SQUARED:
        return gt_imag2();
    case B_REAL:
        return gt_real();
    case B_REAL_SQUARED:
        return gt_real2();
    case B_MANHATTAN:
        return gt_manhattan();
    case B_MANHATTANISH:
        return gt_manhattanish();
    case B_ABS_MANHATTANISH:
        return gt_abs_manhattanish();
    case B_MAX_SQUARED:
        return gt_max2();
    case B_MIN_SQUARED:
        return gt_min2();
    case B_ABS_PRODUCT:
        return gt_absproduct();
    case B_ABS_DIFF:
        return gt_absdiff();
    }
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::gt_abs()
{
    if (std::abs(m_z_r) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_r != m_z_r)
    {
        return true;
    }
    else if (std::abs(m_z_i) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_i != m_z_i)
    {
        return true;
    }
    return (std::sqrt(m_z_r*m_z_r + m_z_i*m_z_i) > m_limit);
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::gt_norm()
{
    if (std::abs(m_z_r) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_r != m_z_r)
    {
        return true;
    }
    else if (std::abs(m_z_i) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_i != m_z_i)
    {
        return true;
    }
    return ((m_z_r*m_z_r + m_z_i*m_z_i) > m_limit);
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::gt_imag()
{
    return (m_z_i == m_ld_inf) || (m_z_i != m_z_i) || (m_z_i > m_limit);
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::gt_imag2()
{
    return (m_z_i == m_ld_inf) || (m_z_i != m_z_i) || (m_z_i*m_z_i > m_limit);
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::gt_real()
{
    return (m_z_r == m_ld_inf) || (m_z_r != m_z_r) || (m_z_r > m_limit);
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::gt_real2()
{
    return (m_z_r == m_ld_inf) || (m_z_r != m_z_r) || (m_z_r*m_z_r > m_limit);
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::gt_manhattanish()
{
    if (std::abs(m_z_r) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_r != m_z_r)
    {
        return true;
    }
    else if (std::abs(m_z_i) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_i != m_z_i)
    {
        return true;
    }
    return ((m_z_r + m_z_i) > m_limit);
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::gt_max2()
{
    if (std::abs(m_z_r) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_r != m_z_r)
    {
        return true;
    }
    else if (std::abs(m_z_i) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_i != m_z_i)
    {
        return true;
    }
    long double r2 = m_z_r*m_z_r;
    long double i2 = m_z_i*m_z_i;
    return ((r2 > i2 ? r2 : i2) > m_limit);
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::gt_min2()
{
    if (std::abs(m_z_r) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_r != m_z_r)
    {
        return true;
    }
    else if (std::abs(m_z_i) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_i != m_z_i)
    {
        return true;
    }
    long double r2 = m_z_r*m_z_r;
    long double i2 = m_z_i*m_z_i;
    return ((r2 < i2 ? r2 : i2) > m_limit);
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::gt_manhattan()
{
    if (std::abs(m_z_r) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_r != m_z_r)
    {
        return true;
    }
    else if (std::abs(m_z_i) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_i != m_z_i)
    {
        return true;
    }
    return ((std::abs(m_z_r) + std::abs(m_z_i)) > m_limit);
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::gt_abs_manhattanish()
{
    if (std::abs(m_z_r) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_r != m_z_r)
    {
        return true;
    }
    else if (std::abs(m_z_i) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_i != m_z_i)
    {
        return true;
    }
    return (std::abs(m_z_r + m_z_i) > m_limit );
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::gt_absproduct()
{
    if (std::abs(m_z_r) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_r != m_z_r)
    {
        return true;
    }
    else if (std::abs(m_z_i) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_i != m_z_i)
    {
        return true;
    }
    return (std::abs(m_z_r*m_z_i) > m_limit);
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::gt_absdiff()
{
    if (std::abs(m_z_r) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_r != m_z_r)
    {
        return true;
    }
    else if (std::abs(m_z_i) == m_ld_inf)
    {
        return true;
    }
    else if (m_z_i != m_z_i)
    {
        return true;
    }
    return (std::abs(m_z_r - m_z_i) > m_limit);
}

//==============================================================================
// calculateN routines are essentially the same, the code is duplicated with
// the variations in the number of results sets collected purely for performance
// reasons.
//==============================================================================


template<>
inline void CalculatingUnit<long double, LongComplex>::calculate0()
{
    int row = StaticCalculatingUnit::nextRow();
    int w = StaticCalculatingUnit::s_width;
    int h = StaticCalculatingUnit::s_height;
    long double pitch = StaticCalculatingUnit::s_pitch.toLDouble();
    bool& stop = StaticCalculatingUnit::s_stop;
    CriticalPointFinder<LongComplex> critical_points(StaticCalculatingUnit::s_config);
    unsigned int number_of_critical_points = critical_points.find();
    StaticCalculatingUnit::s_number_of_critical_points = number_of_critical_points;
    ResultsSet rs;
    m_rs = &rs;
#ifdef BATCH
    ResultsSet single_rs;
    ResultsSet* compound_rs = &single_rs;
#endif
    int max_iteration = 0;
    while (row < h)
    {
#ifndef BATCH
        ResultsSet* compound_rs = &(StaticCalculatingUnit::s_results[row*w]);
#endif
        m_c_r = StaticCalculatingUnit::s_topleft_r.toLDouble();
        m_c_i = StaticCalculatingUnit::s_topleft_i.toLDouble() - static_cast<long double>(row)*pitch;
        for (int col = 0; col < w; col++)
        {
            StaticCalculatingUnit::initialiseResultsSet(*compound_rs, 0.0L, 0.0L, true);
            m_c = LongComplex(m_c_r, m_c_i);
            for (unsigned int cp = 0; cp < number_of_critical_points; cp++)
            {
                StaticCalculatingUnit::setC(m_c);
                m_z = critical_points[cp];
                m_z_r = m_z.real();
                m_z_i = m_z.imag();
                StaticCalculatingUnit::initialiseResultsSet(rs, m_z_r, m_z_i);
                m_iteration = 0;
                while (m_iteration < m_max_iteration)
                {
                    formula();
                    m_z_r = m_z.real();
                    m_z_i = m_z.imag();
                    m_iteration++;
                    if (bailout())
                    {
                        rs.escaped = 1;
                        if (m_iteration > max_iteration)
                        {
                            max_iteration = m_iteration;
                        }
                        break;
                    }
                    if (stop) goto stop_calculation;
                }
                if (rs.escaped == 0)
                {
                    max_iteration = m_max_iteration;
                }
                rs.set[0].escape = m_iteration;
                rs.set[0].z_r = m_z_r;
                rs.set[0].z_i = m_z_i;
                StaticCalculatingUnit::updateResults(*compound_rs, *m_rs);
            }
            StaticCalculatingUnit::processResults(*compound_rs);
            m_finished = m_finished && (m_rs->escaped == number_of_critical_points);
            StaticCalculatingUnit::updateImage(compound_rs, col, row);
            m_c_r += pitch;
#ifndef BATCH
            compound_rs++;
#endif
        }
        StaticCalculatingUnit::updateMaxIteration(max_iteration);
        progress(row);
        row = StaticCalculatingUnit::nextRow();
    }
stop_calculation:
    ;
}

template<>
inline void CalculatingUnit<long double, LongComplex>::calculate1()
{
    int row = StaticCalculatingUnit::nextRow();
    int w = StaticCalculatingUnit::s_width;
    int h = StaticCalculatingUnit::s_height;
    bool& stop = StaticCalculatingUnit::s_stop;
    long double pitch = StaticCalculatingUnit::s_pitch.toLDouble();
    CriticalPointFinder<LongComplex> critical_points(StaticCalculatingUnit::s_config);
    unsigned int number_of_critical_points = critical_points.find();
    StaticCalculatingUnit::s_number_of_critical_points = number_of_critical_points;
    ResultsSet rs;
    m_rs = &rs;
#ifdef BATCH
    ResultsSet single_rs;
    ResultsSet* compound_rs = &single_rs;
#endif
    int max_iteration = 0;
    while (row < h)
    {
#ifndef BATCH
        ResultsSet* compound_rs = &(StaticCalculatingUnit::s_results[row*w]);
#endif
        m_c_r = StaticCalculatingUnit::s_topleft_r.toLDouble();
        m_c_i = StaticCalculatingUnit::s_topleft_i.toLDouble() - static_cast<long double>(row)*pitch;
        for (int col = 0; col < w; col++)
        {
            StaticCalculatingUnit::initialiseResultsSet(*compound_rs, 0.0L, 0.0L, true);
            for (unsigned int cp = 0; cp < number_of_critical_points; cp++)
            {
                m_c = LongComplex(m_c_r, m_c_i);
                StaticCalculatingUnit::setC(m_c);
                m_z = critical_points[cp];
                m_z_r = m_z.real();
                m_z_i = m_z.imag();
                StaticCalculatingUnit::initialiseResultsSet(rs, m_z_r, m_z_i);
                m_iteration = 0;
                while (m_iteration < m_max_iteration)
                {
                    formula();
                    m_z_r = m_z.real();
                    m_z_i = m_z.imag();
                    m_iteration++;
                    collectResults1();
                    if (bailout())
                    {
                        rs.escaped = 1;
                        if (m_iteration > max_iteration)
                        {
                            max_iteration = m_iteration;
                        }
                        break;
                    }
                    if (stop) goto stop_calculating;
                }
                if (rs.escaped == 0)
                {
                    max_iteration = m_max_iteration;
                }
                rs.set[0].escape = m_iteration;
                rs.set[0].z_r = m_z_r;
                rs.set[0].z_i = m_z_i;
                StaticCalculatingUnit::updateResults(*compound_rs, *m_rs);
            }
            StaticCalculatingUnit::processResults(*compound_rs);
            m_finished = m_finished && (m_rs->escaped == number_of_critical_points);
            StaticCalculatingUnit::updateImage(compound_rs, col, row);
            m_c_r += pitch;
#ifndef BATCH
            compound_rs++;
#endif
        }
        StaticCalculatingUnit::updateMaxIteration(max_iteration);
        progress(row);
        row = StaticCalculatingUnit::nextRow();
    }
stop_calculating:
    ;
}

template<>
inline void CalculatingUnit<long double, LongComplex>::calculate2()
{
    int row = StaticCalculatingUnit::nextRow();
    int w = StaticCalculatingUnit::s_width;
    int h = StaticCalculatingUnit::s_height;
    bool& stop = StaticCalculatingUnit::s_stop;
    long double pitch = StaticCalculatingUnit::s_pitch.toLDouble();
    CriticalPointFinder<LongComplex> critical_points(StaticCalculatingUnit::s_config);
    unsigned int number_of_critical_points = critical_points.find();
    StaticCalculatingUnit::s_number_of_critical_points = number_of_critical_points;
    ResultsSet rs;
    m_rs = &rs;
#ifdef BATCH
    ResultsSet single_rs;
    ResultsSet* compound_rs = &single_rs;
#endif
    int max_iteration = 0;
    while (row < h)
    {
#ifndef BATCH
        ResultsSet* compound_rs = &(StaticCalculatingUnit::s_results[row*w]);
#endif
        m_c_r = StaticCalculatingUnit::s_topleft_r.toLDouble();
        m_c_i = StaticCalculatingUnit::s_topleft_i.toLDouble() - static_cast<long double>(row)*pitch;
        for (int col = 0; col < w; col++)
        {
            StaticCalculatingUnit::initialiseResultsSet(*compound_rs, 0.0L, 0.0L, true);
            for (unsigned int cp = 0; cp < number_of_critical_points; cp++)
            {
                m_c = LongComplex(m_c_r, m_c_i);
                StaticCalculatingUnit::setC(m_c);
                m_z = critical_points[cp];
                m_z_r = m_z.real();
                m_z_i = m_z.imag();
                StaticCalculatingUnit::initialiseResultsSet(rs, m_z_r, m_z_i);
                m_iteration = 0;
                while (m_iteration < m_max_iteration)
                {
                    formula();
                    m_z_r = m_z.real();
                    m_z_i = m_z.imag();
                    m_iteration++;
                    collectResults1();
                    collectResults2();
                    if (bailout())
                    {
                        rs.escaped = 1;
                        if (m_iteration > max_iteration)
                        {
                            max_iteration = m_iteration;
                        }
                        break;
                    }
                    if (stop) goto stop_calculation;
                }
                if (rs.escaped == 0)
                {
                    max_iteration = m_max_iteration;
                }
                rs.set[0].escape = m_iteration;
                rs.set[0].z_r = m_z_r;
                rs.set[0].z_i = m_z_i;
                StaticCalculatingUnit::updateResults(*compound_rs, *m_rs);
            }
            StaticCalculatingUnit::processResults(*compound_rs);
            m_finished = m_finished && (m_rs->escaped == number_of_critical_points);
            StaticCalculatingUnit::updateImage(compound_rs, col, row);
            m_c_r += pitch;
#ifndef BATCH
            compound_rs++;
#endif
        }
        StaticCalculatingUnit::updateMaxIteration(max_iteration);
        progress(row);
        row = StaticCalculatingUnit::nextRow();
    }
stop_calculation:
    ;
}

template<>
inline void CalculatingUnit<MpReal, MpComplex>::calculate0()
{
    int row = StaticCalculatingUnit::nextRow();
    int w = StaticCalculatingUnit::s_width;
    int h = StaticCalculatingUnit::s_height;
    bool& stop = StaticCalculatingUnit::s_stop;
    auto& conf = StaticCalculatingUnit::s_config;
    int& p = StaticCalculatingUnit::s_precision;
    CriticalPointFinder<MpComplex> critical_points(conf, p);
    long double pitch = StaticCalculatingUnit::s_pitch.toLDouble();
    unsigned int number_of_critical_points = critical_points.find();
    StaticCalculatingUnit::s_number_of_critical_points = number_of_critical_points;
    ResultsSet rs;
    m_rs = &rs;
#ifdef BATCH
    ResultsSet single_rs;
    ResultsSet* compound_rs = &single_rs;
#endif
    int max_iteration = 0;
    while (row < h)
    {
#ifndef BATCH
        ResultsSet* compound_rs = &(StaticCalculatingUnit::s_results[row*w]);
#endif
        m_c_r = StaticCalculatingUnit::s_topleft_r;
        m_c_i = StaticCalculatingUnit::s_topleft_i - row*pitch;
        for (int col = 0; col < w; col++)
        {
            StaticCalculatingUnit::initialiseResultsSet(*compound_rs, 0.0L, 0.0L, true);
            for (unsigned int cp = 0; cp < number_of_critical_points; cp++)
            {
                m_c = MpComplex(m_c_r, m_c_i);
                StaticCalculatingUnit::setC(m_c);
                m_z = critical_points[cp];
                m_z_r = m_z.real().toLDouble();
                m_z_i = m_z.imag().toLDouble();
                StaticCalculatingUnit::initialiseResultsSet(rs, m_z_r, m_z_i);
                m_iteration = 0;
                while (m_iteration < m_max_iteration)
                {
                    formula();
                    m_z_r = m_z.real().toLDouble();
                    m_z_i = m_z.imag().toDouble();
                    m_iteration++;
                    if (bailout())
                    {
                        rs.escaped = 1;
                        if (m_iteration > max_iteration)
                        {
                            max_iteration = m_iteration;
                        }
                        break;
                    }
                    if (stop) goto stop_calculating;
                }
                if (rs.escaped == 0)
                {
                    max_iteration = m_max_iteration;
                }
                rs.set[0].escape = m_iteration;
                rs.set[0].z_r = m_z_r;
                rs.set[0].z_i = m_z_i;
                StaticCalculatingUnit::updateResults(*compound_rs, *m_rs);
            }
            StaticCalculatingUnit::processResults(*compound_rs);
            m_finished = m_finished && (m_rs->escaped == number_of_critical_points);
            StaticCalculatingUnit::updateImage(compound_rs, col, row);
            m_c_r += pitch;
#ifndef BATCH
            compound_rs++;
#endif
        }
        StaticCalculatingUnit::updateMaxIteration(max_iteration);
        progress(row);
        row = StaticCalculatingUnit::nextRow();
    }
stop_calculating:
    ;
}

template<>
inline void CalculatingUnit<MpReal, MpComplex>::calculate1()
{
    int row = StaticCalculatingUnit::nextRow();
    int w = StaticCalculatingUnit::s_width;
    int h = StaticCalculatingUnit::s_height;
    bool& stop = StaticCalculatingUnit::s_stop;
    auto& conf = StaticCalculatingUnit::s_config;
    int& p = StaticCalculatingUnit::s_precision;
    CriticalPointFinder<MpComplex> critical_points(conf, p);
    long double pitch = StaticCalculatingUnit::s_pitch.toLDouble();
    unsigned int number_of_critical_points = critical_points.find();
    StaticCalculatingUnit::s_number_of_critical_points = number_of_critical_points;
    ResultsSet rs;
    m_rs = &rs;
#ifdef BATCH
    ResultsSet single_rs;
    ResultsSet* compound_rs = &single_rs;
#endif
    int max_iteration = 0;
    while (row < h)
    {
#ifndef BATCH
        ResultsSet* compound_rs = &(StaticCalculatingUnit::s_results[row*w]);
#endif
        m_c_r = StaticCalculatingUnit::s_topleft_r;
        m_c_i = StaticCalculatingUnit::s_topleft_i - row*pitch;
        for (int col = 0; col < w; col++)
        {
            StaticCalculatingUnit::initialiseResultsSet(*compound_rs, 0.0L, 0.0L, true);
            for (unsigned int cp = 0; cp < number_of_critical_points; cp++)
            {
                m_c = MpComplex(m_c_r, m_c_i);
                StaticCalculatingUnit::setC(m_c);
                m_z = critical_points[cp];
                m_z_r = m_z.real().toLDouble();
                m_z_i = m_z.imag().toLDouble();
                StaticCalculatingUnit::initialiseResultsSet(rs, m_z_r, m_z_r);
                m_iteration = 0;
                while (m_iteration < m_max_iteration)
                {
                    formula();
                    m_z_r = m_z.real().toLDouble();
                    m_z_i = m_z.imag().toLDouble();
                    m_iteration++;
                    collectResults1();
                    if (bailout())
                    {
                        rs.escaped = 1;
                        if (m_iteration > max_iteration)
                        {
                            max_iteration = m_iteration;
                        }
                        break;
                    }
                    if (stop) goto stop_calculating;
                }
                if (rs.escaped == 0)
                {
                    max_iteration = m_max_iteration;
                }
                rs.set[0].escape = m_iteration;
                rs.set[0].z_r = m_z_r;
                rs.set[0].z_i = m_z_i;
                StaticCalculatingUnit::updateResults(*compound_rs, *m_rs);
            }
            StaticCalculatingUnit::processResults(*compound_rs);
            m_finished = m_finished && (m_rs->escaped == number_of_critical_points);
            StaticCalculatingUnit::updateImage(compound_rs, col, row);
            m_c_r += pitch;
#ifndef BATCH
            compound_rs++;
#endif
        }
        StaticCalculatingUnit::updateMaxIteration(max_iteration);
        progress(row);
        row = StaticCalculatingUnit::nextRow();
    }
stop_calculating:
    ;
}

template<>
inline void CalculatingUnit<MpReal, MpComplex>::calculate2()
{
    int row = StaticCalculatingUnit::nextRow();
    int w = StaticCalculatingUnit::s_width;
    int h = StaticCalculatingUnit::s_height;
    bool& stop = StaticCalculatingUnit::s_stop;
    auto& conf = StaticCalculatingUnit::s_config;
    int& p = StaticCalculatingUnit::s_precision;
    CriticalPointFinder<MpComplex> critical_points(conf, p);
    long double pitch = StaticCalculatingUnit::s_pitch.toLDouble();
    unsigned int number_of_critical_points = critical_points.find();
    StaticCalculatingUnit::s_number_of_critical_points = number_of_critical_points;
    ResultsSet rs;
    m_rs = &rs;
#ifdef BATCH
    ResultsSet single_rs;
    ResultsSet* compound_rs = &single_rs;
#endif
    int max_iteration = 0;
    while (row < h)
    {
#ifndef BATCH
        ResultsSet* compound_rs = &(StaticCalculatingUnit::s_results[row*w]);
#endif
        m_c_r = StaticCalculatingUnit::s_topleft_r.toLDouble();
        m_c_i = StaticCalculatingUnit::s_topleft_i.toLDouble() - static_cast<long double>(row)*pitch;
        for (int col = 0; col < w; col++)
        {
            StaticCalculatingUnit::initialiseResultsSet(*compound_rs, 0.0L, 0.0L, true);
            for (unsigned int cp = 0; cp < number_of_critical_points; cp++)
            {
                m_c = MpComplex(m_c_r, m_c_i);
                StaticCalculatingUnit::setC(m_c);
                m_z = critical_points[cp];
                m_z_r = m_z.real().toLDouble();
                m_z_i = m_z.imag().toLDouble();
                StaticCalculatingUnit::initialiseResultsSet(rs, m_z_r, m_z_i);
                m_iteration = 0;
                while (m_iteration < m_max_iteration)
                {
                    formula();
                    m_z_r = m_z.real().toLDouble();
                    m_z_i = m_z.imag().toLDouble();
                    m_iteration++;
                    collectResults1();
                    collectResults2();
                    if (bailout())
                    {
                        rs.escaped = 1;
                        if (m_iteration > max_iteration)
                        {
                            max_iteration = m_iteration;
                        }
                        break;
                    }
                    if (stop) goto stop_calculating;
                }
                if (rs.escaped == 0)
                {
                    max_iteration = m_max_iteration;
                }
                rs.set[0].escape = m_iteration;
                rs.set[0].z_r = m_z_r;
                rs.set[0].z_i = m_z_i;
                StaticCalculatingUnit::updateResults(*compound_rs, *m_rs);
            }
            StaticCalculatingUnit::processResults(*compound_rs);
            m_finished = m_finished && (m_rs->escaped == number_of_critical_points);
            StaticCalculatingUnit::updateImage(compound_rs, col, row);
            m_c_r += pitch;
#ifndef BATCH
            compound_rs++;
#endif
        }
        StaticCalculatingUnit::updateMaxIteration(max_iteration);
        progress(row);
        row = StaticCalculatingUnit::nextRow();
    }
stop_calculating:
    ;
}

template<class T1, class T2>
inline bool CalculatingUnit<T1, T2>::calculate()
{
    m_finished = true; // Assume finished cleared if calculation hasn't finished
    m_limit = StaticCalculatingUnit::s_limit;
    switch (StaticCalculatingUnit::s_number_of_results_sets)
    {
    case 0: calculate0(); break;
    case 1: calculate1(); break;
    case 2: calculate2();
    default: break;
    }
    return m_finished;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::collectResults1()
{
    switch (StaticCalculatingUnit::s_results_set[0])
    {
    case StaticCalculatingUnit::RS_ANGLE: angleResults1(); return;
    case StaticCalculatingUnit::RS_MAGNITUDE: magnitudeResults1(); return;
    case StaticCalculatingUnit::RS_GAUSSIAN: gaussianResults1(); return;
    case StaticCalculatingUnit::RS_ORBIT_TRAP: orbitTrapResults1(); return;
    default: return;
    }
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::angleResults1()
{
    if (m_z_r != m_z_r)
    {
        return;
    }
    if (m_z_i != m_z_i)
    {
        return;
    }
    long double angle = arg(LongComplex(m_z_r, m_z_i));
    angle = (angle < 0.0 ? angle + 2.0L*m_pi : angle);
    resultStatistics1(angle);
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::magnitudeResults1()
{
    if (m_z_r != m_z_r)
    {
        return;
    }
    if (m_z_i != m_z_i)
    {
        return;
    }
    resultStatistics1(magnitude(LongComplex(m_z_r, m_z_i), false));
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::gaussianResults1()
{
    if (m_z_r != m_z_r)
    {
        return;
    }
    if (m_z_i != m_z_i)
    {
        return;
    }
    using std::abs;
    Statistics &s = m_rs->set[0];
    LongComplex z(m_z_r, m_z_i);
    LongComplex distance;
    switch(StaticCalculatingUnit::s_gaussian_type[0])
    {
    case StaticCalculatingUnit::GI_CEIL:
        distance = gi_ceil(z);
        break;
    case StaticCalculatingUnit::GI_FLOOR:
        distance = gi_floor(z);
        break;
    case StaticCalculatingUnit::GI_ROUND:
        distance = gi_round(z);
        break;
    case StaticCalculatingUnit::GI_TRUNC:
        distance = gi_trunc(z);
        break;
    }
    long double normd = norm(distance);
    long double absd = sqrt(normd);
    s.square_total += normd;
    s.total += absd;
    if (normd < norm(s.min_dist))
    {
        s.min_dist = distance;
        s.iteration_at_min = m_iteration;
    }
    if (normd > norm(s.max_dist))
    {
        s.max_dist = distance;
        s.iteration_at_max = m_iteration;
    }
    long double angle = arg(distance) + m_pi;
    s.angle_total += angle;
    if (angle < s.minimum)
    {
        s.minimum = angle;
    }
    if (angle > s.maximum)
    {
        s.maximum = angle;
    }
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::orbitTrapResults1()
{
    if (m_z_r != m_z_r)
    {
        return;
    }
    if (m_z_i != m_z_i)
    {
        return;
    }
    resultStatistics1(StaticCalculatingUnit::s_trap[0]->distance(LongComplex(m_z_r, m_z_i)));
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::collectResults2()
{
    switch (StaticCalculatingUnit::s_results_set[1])
    {
    case StaticCalculatingUnit::RS_ANGLE: angleResults2(); return;
    case StaticCalculatingUnit::RS_MAGNITUDE: magnitudeResults2(); return;
    case StaticCalculatingUnit::RS_GAUSSIAN: gaussianResults2(); return;
    case StaticCalculatingUnit::RS_ORBIT_TRAP: orbitTrapResults2(); return;
    default: return;
    }
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::angleResults2()
{
    if (m_z_r != m_z_r)
    {
        return;
    }
    if (m_z_i != m_z_i)
    {
        return;
    }
    long double angle = arg(LongComplex(m_z_r, m_z_i));
    angle = (angle < 0.0 ? angle + 2.0L*m_pi : angle);
    resultStatistics2(angle);
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::magnitudeResults2()
{
    if (m_z_r != m_z_r)
    {
        return;
    }
    if (m_z_i != m_z_i)
    {
        return;
    }
    resultStatistics2(magnitude(LongComplex(m_z_r, m_z_i), true));
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::resultStatistics1(long double value)
{
    if (m_z_r != m_z_r)
    {
        return;
    }
    if (m_z_i != m_z_i)
    {
        return;
    }
    bool& change = StaticCalculatingUnit::s_change[0];
    Statistics &s = m_rs->set[0];
    long double value2 = (change ? std::abs(value - s.last) : value);
    long double normd = value2*value2;
    s.total += value2;
    s.square_total += normd;
    if (value2 < s.minimum)
    {
        s.minimum = value2;
        s.iteration_at_min = m_iteration;
    }
    if (value2 > s.maximum)
    {
        s.maximum = value2;
        s.iteration_at_max = m_iteration;
    }
    s.es_sum1 += (change ? std::abs(exp(-value) - exp(-s.last)) : exp(-value));
    long double exp_inverse_change = exp(-1.0L/(std::abs(value - s.last)));
    s.es_sum2 += (change ? std::abs(exp_inverse_change - s.es2_last) : exp_inverse_change);
    s.last = value;
    s.es2_last = exp_inverse_change;
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::gaussianResults2()
{
    if (m_z_r != m_z_r)
    {
        return;
    }
    if (m_z_i != m_z_i)
    {
        return;
    }
    using std::abs;
    Statistics &s = m_rs->set[1];
    LongComplex z(m_z_r, m_z_i);
    LongComplex distance;
    switch(StaticCalculatingUnit::s_gaussian_type[1])
    {
    case StaticCalculatingUnit::GI_CEIL:
        distance = gi_ceil(z);
        break;
    case StaticCalculatingUnit::GI_FLOOR:
        distance = gi_floor(z);
        break;
    case StaticCalculatingUnit::GI_ROUND:
        distance = gi_round(z);
        break;
    case StaticCalculatingUnit::GI_TRUNC:
        distance = gi_trunc(z);
        break;
    }
    long double normd = norm(distance);
    long double absd = sqrt(normd);
    s.square_total += normd;
    s.total += absd;
    if (normd < norm(s.min_dist))
    {
        s.min_dist = distance;
        s.iteration_at_min = m_iteration;
    }
    if (normd > norm(s.max_dist))
    {
        s.max_dist = distance;
        s.iteration_at_max = m_iteration;
    }
    long double angle = arg(distance) + m_pi;
    s.angle_total += angle;
    if (angle < s.minimum)
    {
        s.minimum = angle;
    }
    if (angle > s.maximum)
    {
        s.maximum = angle;
    }
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::orbitTrapResults2()
{
    if (m_z_r != m_z_r)
    {
        return;
    }
    if (m_z_i != m_z_i)
    {
        return;
    }
    resultStatistics2(StaticCalculatingUnit::s_trap[1]->distance(LongComplex(m_z_r, m_z_i)));
}

template<class T1, class T2>
inline void CalculatingUnit<T1, T2>::resultStatistics2(long double value)
{
    if (m_z_r != m_z_r)
    {
        return;
    }
    if (m_z_i != m_z_i)
    {
        return;
    }
    bool& change = StaticCalculatingUnit::s_change[1];
    Statistics &s = m_rs->set[1];
    long double value2 = (change ? std::abs(value - s.last) : value);
    long double normd = value2*value2;
    s.total += value2;
    s.square_total += normd;
    if (value2 < s.minimum)
    {
        s.minimum = value2;
        s.iteration_at_min = m_iteration;
    }
    if (value2 > s.maximum)
    {
        s.maximum = value2;
        s.iteration_at_max = m_iteration;
    }
    s.es_sum1 += (change ? std::abs(exp(-value) - exp(-s.last)) : exp(-value));
    long double exp_inverse_change = exp(-1.0L/(std::abs(value - s.last)));
    s.es_sum2 += (change ? std::abs(exp_inverse_change - s.es2_last) : exp_inverse_change);
    s.last = value;
    s.es2_last = exp_inverse_change;
}

template<class T1, class T2>
inline long double CalculatingUnit<T1, T2>::magnitude(const LongComplex& z, bool second)
{
    if (m_magnitude_type[second] == 'M')
    {
        return ld::abs(z);
    }
    else
    {
        return magnitude_option(z, m_quotient[second]) /
               magnitude_option(z, m_divisor[second]);
    }
}

template<class T1, class T2>
inline long double CalculatingUnit<T1, T2>::magnitude_option(const LongComplex& z, char option)
{
    switch (option)
    {
    case 'a': // smaller
        return (std::abs(real(z)) < std::abs(imag(z)) ? std::abs(real(z)) : std::abs(imag(z)));
    case 'b': // larger
        return (std::abs(real(z)) > std::abs(imag(z)) ? std::abs(real(z)) : std::abs(imag(z)));
    case 'c': // abs real
        return std::abs(real(z));
    case 'd': // abs imag
        return std::abs(imag(z));
    case 'e': // sum
        return std::abs(real(z) + imag(z));
    case 'f': // abs diff
        return std::abs(real(z) - imag(z));
    case 'g': // sum abs
        return std::abs(real(z)) + std::abs(imag(z));
    case 'h': // abs diff abs
        return std::abs(std::abs(real(z)) - std::abs(imag(z)));
    case 'i': // product
        return std::abs(real(z)*imag(z));
    default:  // abs
    case 'j':
        return ld::abs(z);
    case 'k': // norm
        return ld::norm(z);
    }
}

template<class T1, class T2>
inline LongComplex CalculatingUnit<T1, T2>::gi_round(const LongComplex& z)
{
    long double int_r, int_i;
    long double z_r = real(z);
    z_r += (z_r < 0.0L ? -0.5 : 0.5);
    long double z_i = imag(z);
    z_i += (z_i < 0.0L ? -0.5 : 0.5);
    std::modf(z_r, &int_r); // discard return value
    std::modf(z_i, &int_i); // discard return value
    LongComplex gi = LongComplex(int_r, int_i);
    return (z - gi);
}

template<class T1, class T2>
inline LongComplex CalculatingUnit<T1, T2>::gi_ceil(const LongComplex& z)
{
    LongComplex gi(ceil(real(z)), ceil(imag(z)));
    return (z - gi);
}

template<class T1, class T2>
inline LongComplex CalculatingUnit<T1, T2>::gi_floor(const LongComplex& z)
{
    LongComplex gi(floor(real(z)), floor(imag(z)));
    return (z - gi);
}

template<class T1, class T2>
inline LongComplex CalculatingUnit<T1, T2>::gi_trunc(const LongComplex& z)
{
    LongComplex gi;
    long double int_r, int_i;
    long double z_r = real(z);
    long double z_i = imag(z);
    std::modf(z_r, &int_r); // discard return value
    std::modf(z_i, &int_i); // discard return value
    gi = LongComplex(int_r, int_i);
    return (z - gi);
}

template<class T1, class T2>
void CalculatingUnit<T1, T2>::enableProgressSignals(bool value)
{
    boost::mutex::scoped_lock lock(m_progress_signal_mutex);
    m_progress_signals_enabled = value;
    m_send_progress = value;
}

template<class T1, class T2>
void CalculatingUnit<T1, T2>::sendProgressSignals(bool value)
{
    boost::mutex::scoped_lock lock(m_progress_signal_mutex);
    if (m_progress_signals_enabled)
    {
        m_send_progress = value;
    }
}

template<>
inline void CalculatingUnit<long double, LongComplex>::calculateOrbits()
{
    // Although this method has a template type parameter it isn't used,
    // only long double precision calculations will be performed i.e. it
    // is not intended that the method will be called on an object instantiated
    // with a type other than long double.
    long double density_factor = StaticCalculatingUnit::s_density_factor;
    int row = StaticCalculatingUnit::nextRow();
    int width = StaticCalculatingUnit::s_calculating_width;
    int height = StaticCalculatingUnit::s_calculating_height;
    // adjust number of elements calculated based on density
    height = static_cast<int>(static_cast<long double>(height)*density_factor);
    width = static_cast<int>(static_cast<long double>(width)*density_factor);
    long double pitch = StaticCalculatingUnit::s_pitch.toLDouble()/density_factor;
    int omit_number = StaticCalculatingUnit::s_omit;
    int orbit_length = m_max_iteration;
    m_orbit.resize(orbit_length - omit_number);
    m_limit = StaticCalculatingUnit::s_limit;
    CriticalPointFinder<LongComplex> critical_points(StaticCalculatingUnit::s_config);
    unsigned int number_of_critical_points = critical_points.find();
    StaticCalculatingUnit::s_number_of_critical_points = number_of_critical_points;
    while (row < height)
    {
        m_c_r = StaticCalculatingUnit::s_orbit_topleft_r;
        m_c_i = StaticCalculatingUnit::s_orbit_topleft_i - static_cast<long double>(row)*pitch;
        for (int col = 0; (col < width) && !StaticCalculatingUnit::s_stop; col++)
        {
            for (unsigned int cp = 0; cp < number_of_critical_points; cp++)
            {
                m_c = LongComplex(m_c_r, m_c_i);
                StaticCalculatingUnit::setC(m_c);
                m_z = critical_points[cp];
                int count = 0;
                int iteration = 0;
                bool escaped = false;
                while (iteration < orbit_length)
                {
                    formula();
                    m_z_r = m_z.real();
                    m_z_i = m_z.imag(); 
                    if (iteration >= omit_number)
                    {
                        m_orbit[count] = m_z;
                        count++;
                    }
                    iteration++;
                    if (bailout())
                    {
                        escaped = true;
                        break;
                    }
                }
                if (count > 0)
                {
                    StaticCalculatingUnit::plot(m_orbit, count, escaped);
                }
            }
            m_c_r += pitch;
        }
        progress(row);
        row = StaticCalculatingUnit::nextRow();
    }
}

template<>
inline void CalculatingUnit<MpReal, MpComplex>::calculateOrbits()
{
    // should never be called
}

template<class T1, class T2>
inline void CalculatingUnit<T1,  T2>::progress(int row)
{
    if (m_send_progress && m_progress_signals_enabled && !StaticCalculatingUnit::s_stop)
    {
        m_send_progress = false;
        m_generic->send_signal_progress(row);
    }
}

template<class T1, class T2>
void CalculatingUnit<T1,  T2>::set_generic(CalculatingUnitGeneric *p)
{
    m_generic = p;
}

