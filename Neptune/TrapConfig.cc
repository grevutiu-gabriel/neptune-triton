//
// File:   TrapConfig.cc
// Author: M.R.Eggleston
//
// Created on 12 April 2011, 15:20
//

#include <boost/lexical_cast.hpp>
#include <iomanip>
#include "Utilities.h"
#include "KeyString.h"
#include "TrapConfig.h"

LongComplex TrapConfig::s_position[2];
long double TrapConfig::s_width[2] = { 1.0L, 1.0L };
int TrapConfig::s_rotation[2] = { 0, 0 };
bool TrapConfig::s_centre_point[2] = { false, false };
bool TrapConfig::s_triangles[2] = { false, false };
bool TrapConfig::s_inner_circle[2] = { true, true };
bool TrapConfig::s_outer_circle[2] = { true, true };
bool TrapConfig::s_centre_points[2] = { false, false };
long double TrapConfig::s_construction_rotation[2] = { 0.0L, 0.0L };
int TrapConfig::s_chain_circles[2] = { 5, 5 };
long double TrapConfig::s_inversion_distance[2] = { 0.0L, 0.0L };

TrapConfig::TrapConfig()
:
m_type(OrbitTrap::POINT),
m_index(0)
{
}

TrapConfig::~TrapConfig()
{
}

void TrapConfig::setTrapType(bool inner, OrbitTrap::TRAP_TYPE value)
{
    m_type = value;
    m_index = static_cast<int>(inner);
}

void TrapConfig::setPosition(const LongComplex &value)
{
    s_position[m_index] = value;
}

LongComplex TrapConfig::getPosition()
{
    return s_position[m_index];
}

void TrapConfig::setWidth(const long double &value)
{
    s_width[m_index] = value;
}

long double TrapConfig::getWidth()
{
    return s_width[m_index];
}

void TrapConfig::setRotation(int value)
{
    s_rotation[m_index] = value;
}

int TrapConfig::getRotation()
{
    return s_rotation[m_index];
}

void TrapConfig::setCentrePoint(bool value)
{
    s_centre_point[m_index] = value;
}

bool TrapConfig::getCentrePoint()
{
    return s_centre_point[m_index];
}

void TrapConfig::configFromString(const std::string& value)
{
    KeyString keys(value);
    long double centre_r = boost::lexical_cast<long double>(keys.GetKey("r", "0.0"));
    long double centre_i = boost::lexical_cast<long double>(keys.GetKey("i", "0.0"));
    long double width = boost::lexical_cast<long double>(keys.GetKey("width", "0.0"));
    unsigned int rotation = boost::lexical_cast<unsigned int>(keys.GetKey("rot", "0"));
    bool cp = (boost::lexical_cast<unsigned int>(keys.GetKey("cp", "0")) != 0);
    setPosition(LongComplex(centre_r, centre_i));
    setWidth(width);
    setRotation(rotation);
    setCentrePoint(cp);
    if (isSteinerChain())
    {
        bool inner = (boost::lexical_cast<unsigned int>(keys.GetKey("inner", "1")) != 0);
        bool outer = (boost::lexical_cast<unsigned int>(keys.GetKey("outer", "1")) != 0);
        long double invd = boost::lexical_cast<long double>(keys.GetKey("invd", "0.0"));
        long double crot = boost::lexical_cast<long double>(keys.GetKey("crot", "0.0"));
        unsigned int n = boost::lexical_cast<unsigned int>(keys.GetKey("n", "5"));
        bool triangles = (boost::lexical_cast<unsigned int>(keys.GetKey("triangles", "0")) != 0);
        bool centre_points =  (boost::lexical_cast<unsigned int>(keys.GetKey("cccp", "0")) != 0);
        setInnerCircle(inner);
        setOuterCircle(outer);
        setInversionDistance(invd);
        setChainCircles(n);
        setTriangles(triangles);
        setCentrePoints(centre_points);
        setConstructionRotation(crot);
    }
}

std::string TrapConfig::configAsString()
{
    KeyString keys;
    LongComplex pos = getPosition();
    keys.SetKey("r", longdoubleToString(real(pos)));
    keys.SetKey("i", longdoubleToString(imag(pos)));
    keys.SetKey("width", longdoubleToString(getWidth()));
    keys.SetKey("rot", uintToString(getRotation()));
    keys.SetKey("cp", intToString(getCentrePoint()));
    if (isSteinerChain())
    {
        keys.SetKey("inner", intToString(getInnerCircle()));
        keys.SetKey("outer", intToString(getOuterCircle()));
        keys.SetKey("invd", longdoubleToString(getInversionDistance()));
        keys.SetKey("n", uintToString(getChainCircles()));
        keys.SetKey("triangles", intToString(getTriangles()));
        keys.SetKey("cccp", intToString(getCentrePoints()));
    }
    return keys.String();
}

bool TrapConfig::hasSize()
{
    return OrbitTrap::getTrap(m_type, static_cast<bool>(m_index))->hasSize();
}

bool TrapConfig::hasRotation()
{
    return OrbitTrap::getTrap(m_type, static_cast<bool>(m_index))->hasRotation();
}

bool TrapConfig::hasCentrePoint()
{
    return OrbitTrap::getTrap(m_type, static_cast<bool>(m_index))->hasCentrePoint();
}

bool TrapConfig::isSteinerChain()
{
    return (m_type == OrbitTrap::STEINER_CHAIN);
}

void TrapConfig::setInnerCircle(bool value)
{
    s_inner_circle[m_index] = value;
}

bool TrapConfig::getInnerCircle()
{
    return s_inner_circle[m_index];
}

void TrapConfig::setOuterCircle(bool value)
{
    s_outer_circle[m_index] = value;
}

bool TrapConfig::getOuterCircle()
{
    return s_outer_circle[m_index];
}

void TrapConfig::setInversionDistance(long double value)
{
    s_inversion_distance[m_index] = value;
}

long double TrapConfig::getInversionDistance()
{
    return s_inversion_distance[m_index];
}

void TrapConfig::setChainCircles(int value)
{
    if (value < SteinerChain::MIN_CHAIN_CIRCLES) value = SteinerChain::MIN_CHAIN_CIRCLES;
    if (value > SteinerChain::MAX_CHAIN_CIRCLES) value = SteinerChain::MAX_CHAIN_CIRCLES;
    s_chain_circles[m_index] = value;
}

int TrapConfig::getChainCircles()
{
    return s_chain_circles[m_index];
}

void TrapConfig::setTriangles(bool value)
{
    s_triangles[m_index] = value;
}

bool TrapConfig::getTriangles()
{
    return s_triangles[m_index];
}

void TrapConfig::setCentrePoints(bool value)
{
    s_centre_points[m_index] = value;
}

bool TrapConfig::getCentrePoints()
{
    return s_centre_points[m_index];
}

void TrapConfig::setConstructionRotation(long double value)
{
    s_construction_rotation[m_index] = value;
}

long double TrapConfig::getConstructionRotation()
{
    return s_construction_rotation[m_index];
}
