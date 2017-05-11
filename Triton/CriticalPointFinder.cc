// 
// File:   CriticalPointFinder.cc
// Author: M.R.Eggleston
//
// Created on 07 August 2014, 16:30
//

#include "CriticalPointFinder.h"
#include "Utilities.h"
#include "FractalConfig.h"

template<>
LongComplex& CriticalPointFinder<LongComplex>::operator[](unsigned int i)
{
    m_value.real(0.0L);
    m_value.imag(0.0L);
    if (i < m_points.size())
    {
        m_value = m_points[i];
    }
    return m_value;
}

template<>
MpComplex& CriticalPointFinder<MpComplex>::operator[](unsigned int i)
{
    m_value = MpComplex(MpReal(0.0L, m_precision), MpReal(0.0L, m_precision));
    if (i < m_points.size())
    {
        m_value = m_points[i];
    }
    return m_value;
}

template<>
void CriticalPointFinder<LongComplex>::filter_points()
{
    int number_of_points = m_roots.size();
    m_points.clear();
    for (int i = 0; i < number_of_points; i++)
    {
        if (m_ignore[i] == 0U)
        {
            m_points.push_back(m_roots[i]);
        }
    }
}

template<>
void CriticalPointFinder<MpComplex>::filter_points()
{
    int number_of_points = m_roots.size();
    m_points.clear();
    for (int i = 0; i < number_of_points; i++)
    {
        if (!m_ignore[i])
        {
            m_points.push_back(MpComplex(MpReal(m_roots[i].real(), m_precision),
                                         MpReal(m_roots[i].imag(), m_precision)));
        }
    }
}
