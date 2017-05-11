//
// File:   TrapConfig.h
// Author: M.R.Eggleston
//
// Created on 12 April 2011, 15:20
//

#pragma once

#include <map>
#include "OrbitTraps.h"
#include "TypesAndConstants.h"

class TrapConfig
{
public:
    TrapConfig();

    void setTrapType(bool inner, OrbitTrap::TRAP_TYPE value);
    OrbitTrap::TRAP_TYPE getTrapType();
    void setPosition(const LongComplex &value);
    LongComplex getPosition();
    void setWidth(const long double &value);
    long double getWidth();
    void setRotation(int value);
    int getRotation();
    void configFromString(const std::string& value);
    std::string configAsString();
    void setCentrePoint(bool value);
    bool getCentrePoint();

    bool hasRotation();
    bool hasSize();
    bool hasCentrePoint();

    // The Steiner Chain orbit trap has more configuration
    bool isSteinerChain();
    void setInnerCircle(bool value);
    bool getInnerCircle();
    void setOuterCircle(bool value);
    bool getOuterCircle();
    void setInversionDistance(long double value);
    long double getInversionDistance();
    void setChainCircles(int value);
    int getChainCircles();
    void setTriangles(bool value);
    bool getTriangles();
    void setCentrePoints(bool value);
    bool getCentrePoints();
    void setConstructionRotation(long double value);
    long double getConstructionRotation();

    virtual ~TrapConfig();
private:
    TrapConfig(const TrapConfig& orig);

    OrbitTrap::TRAP_TYPE m_type;
    int m_index;

    static LongComplex s_position[2];
    static long double s_width[2];
    static int s_rotation[2];
    static bool s_centre_point[2];
    static bool s_inner_circle[2];
    static bool s_outer_circle[2];
    static bool s_triangles[2];
    static bool s_centre_points[2];
    static int s_chain_circles[2];
    static long double s_inversion_distance[2];
    static long double s_construction_rotation[2];
};


