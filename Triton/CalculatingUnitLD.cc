#include "CalculatingUnitLD.h"

CalculatingUnitLD::CalculatingUnitLD()
{
    m_unit.set_generic(dynamic_cast<CalculatingUnitGeneric*>(this));
}

CalculatingUnitLD::~CalculatingUnitLD()
{
}

void CalculatingUnitLD::setFractal()
{
    m_unit.setFractal();
}

void CalculatingUnitLD::calculateOrbits()
{
    m_unit.calculateOrbits();
}

void CalculatingUnitLD::enableProgressSignals(bool value)
{
    m_unit.enableProgressSignals(value);
}

void CalculatingUnitLD::sendProgressSignals(bool value)
{
    m_unit.sendProgressSignals(value);
}
