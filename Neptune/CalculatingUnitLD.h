//
// File:   CalculatingUnitLD.h
// Author: M.R.Eggleston
//
// Wrapper for a template class required because Qt doesn't support templates.
//
// Created 29th January 2014, 15:37

#pragma once

#include "ldcomplex.h"
#include "CalculatingUnit.h"
#include "CalculatingUnitGeneric.h"

class CalculatingUnitLD : public CalculatingUnitGeneric
{
public:
    CalculatingUnitLD();
    virtual ~CalculatingUnitLD();

    void setFractal();

    void setValues(int iteration, int chunk_size);
    bool calculate();
    void calculateOrbits();
    void enableProgressSignals(bool value);
    void sendProgressSignals(bool value);

private:
    CalculatingUnit<long double,LongComplex> m_unit;
};

inline void CalculatingUnitLD::setValues(int iteration, int chunk_size)
{
    m_unit.setValues(iteration, chunk_size);
}

inline bool CalculatingUnitLD::calculate()
{
    return m_unit.calculate();
}
