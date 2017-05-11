//
// File:   CalculatingUnitMP.h
// Author: M.R.Eggleston
//
// Created on 29 January 2014, 15:37
//

#pragma once
#include "mpreal.h"
#include "mpcomplex.h"
#include "CalculatingUnit.h"
#include "CalculatingUnitGeneric.h"

class CalculatingUnitMP : public CalculatingUnitGeneric
{
public:
    CalculatingUnitMP();
    virtual ~CalculatingUnitMP();

    void setFractal();

    void setValues(int iteration, int chunk_size);
    bool calculate();
    void calculateOrbits();
    void enableProgressSignals(bool value);
    void sendProgressSignals(bool value);

private:
    CalculatingUnit<MpReal, MpComplex> m_unit;
};

inline void CalculatingUnitMP::setValues(int iteration, int chunk_size)
{
    m_unit.setValues(iteration, chunk_size);
}

inline bool CalculatingUnitMP::calculate()
{
    return m_unit.calculate();
}
