//
// File:   OrbitTraps.h
// Author: M.R.Eggleston
//
// Created on 08 April 2011, 18:47
//

#pragma once

#ifdef NOT_MOC
#include <boost/shared_ptr.hpp>
#endif
#include "Colours.h"

class OrbitTrap
{
public:
    enum TRAP_TYPE
    {
        POINT,
        FOUR_POINTS,
        LINE,
        CROSS,
        SQUARE,
        //GRID,
        //MESH,
        CIRCLE = 7,
        TRIANGLE,
        TRIFORM,
        ASTERISK,
        CIRCLE_LINE,
        CIRCLE_CROSS,
        CIRCLE_TRIFORM,
        QUARTER_CIRCLES,
        CIRCLE_TRIANGLE,
        TRIANGLE_CIRCLE,
        CIRCLE_SQUARE,
        SQUARE_CIRCLE,
        OCTOTHORPE,
        RUNNING_TRACK,
        PINCH,
        STEINER_CHAIN
    };

    static boost::shared_ptr<OrbitTrap> getTrap(TRAP_TYPE, bool inner);

    void SetPosition(const LongComplex &value);
    virtual void SetSize(long double value);
    virtual void SetRotation(unsigned int value);
    void EnableCentralPoint(bool value);
    bool isCentralPointEnabled();
    virtual bool hasCentrePoint() { return false; };

    LongComplex GetPosition();
    long double GetSize();
    virtual bool hasSize() { return true; };
    unsigned int GetRotation();
    virtual bool hasRotation() { return true; };

    virtual long double distance(const LongComplex &value) = 0;

    virtual ~OrbitTrap();

protected:
    OrbitTrap();
    OrbitTrap(const OrbitTrap &other);

    virtual void scale() = 0;
    void setPrimaryRotationMatrix(unsigned int angle);
    void setSecondaryRotationMatrix(unsigned int angle);
    LongComplex normalisePoint(const LongComplex &value);
    LongComplex rotatePoint(const LongComplex &value);
    long double checkCentralPoint(const LongComplex &position, long double dist);
    LongComplex unsign(const LongComplex &value);

    LongComplex m_position;
    long double m_size;
    bool m_central_point;
    unsigned int m_rotation;
    //
    long double m_r11; // cos(m_rotation)
    long double m_r12; // sin(m_rotation)
    long double m_r21; // -sin(m_rotation)
    long double m_r22; // cos(m_rotation)
    //
    long double m_sr11; // cos(m_secondary_rotation)
    long double m_sr12; // sin(m_secondary_rotation)
    long double m_sr21; // -sin(m_secondary_rotation)
    long double m_sr22; // cos(m_secondary_rotation)

    typedef std::map<OrbitTrap::TRAP_TYPE, boost::shared_ptr<OrbitTrap> > TrapMap;

    static TrapMap s_outer_traps;
    static TrapMap s_inner_traps;

    // Declare a constant for pi as some systems do not define M_PIl
    static const long double m_pi;
};

class Point : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<Point> instance(bool inner);

    bool hasSize() { return false; };
    bool hasRotation() { return false; };

    long double distance(const LongComplex &value);
private:
    Point();
    
    void scale() {};

    static boost::shared_ptr<Point> s_outer_instance;
    static boost::shared_ptr<Point> s_inner_instance;
};

class FourPoints : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<FourPoints> instance(bool inner);

    long double distance(const LongComplex &value);
    virtual bool hasCentrePoint() { return true; };
private:
    void scale();
    FourPoints();

    LongComplex m_default_p0;
    LongComplex m_p0;

    static boost::shared_ptr<FourPoints> s_outer_instance;
    static boost::shared_ptr<FourPoints> s_inner_instance;
};

class Line : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<Line> instance(bool inner);

    long double distance(const LongComplex &value);

private:
    Line();
    void scale();

    LongComplex m_default_p0;
    LongComplex m_p0;

    static boost::shared_ptr<Line> s_outer_instance;
    static boost::shared_ptr<Line> s_inner_instance;
};

class Cross : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<Cross> instance(bool inner);

    long double distance(const LongComplex &value);

private:
    Cross();
    void scale();

    LongComplex m_default_p0;
    LongComplex m_default_p1;
    LongComplex m_p0;
    LongComplex m_p1;

    static boost::shared_ptr<Cross> s_outer_instance;
    static boost::shared_ptr<Cross> s_inner_instance;
};

class Square : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<Square> instance(bool inner);

    long double distance(const LongComplex &value);
    virtual bool hasCentrePoint() { return true; };

private:
    Square();
    void scale();

    LongComplex m_default_p0;
    LongComplex m_p0;
    
    static boost::shared_ptr<Square> s_outer_instance;
    static boost::shared_ptr<Square> s_inner_instance;
};

//class Grid : public OrbitTrap
//{
//public:
//    static void create();
//    static boost::shared_ptr<Grid> instance(bool inner);
//
//    long double distance(const LongComplex &value);
//
//private:
//    Grid();
//    void scale();
//
//    LongComplex m_default_p0;
//    LongComplex m_p0;
//
//    static boost::shared_ptr<Grid> s_outer_instance;
//    static boost::shared_ptr<Grid> s_inner_instance;
//};
//
//class Mesh : public OrbitTrap
//{
//public:
//    static void create();
//    static boost::shared_ptr<Mesh> instance(bool inner);
//
//    long double distance(const LongComplex &value);
//
//private:
//    Mesh();
//    void scale();
//
//    LongComplex m_default_p0;
//    LongComplex m_p0;
//
//    static boost::shared_ptr<Mesh> s_outer_instance;
//    static boost::shared_ptr<Mesh> s_inner_instance;
//};

class Circle : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<Circle> instance(bool inner);

    bool hasRotation() { return false; };

    long double distance(const LongComplex &value);
    virtual bool hasCentrePoint() { return true; };

private:
    Circle();
    void scale(){};
    
    static boost::shared_ptr<Circle> s_outer_instance;
    static boost::shared_ptr<Circle> s_inner_instance;
};

class Triangle : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<Triangle> instance(bool inner);

    bool hasRotation() { return true; };

    long double distance(const LongComplex &value);
    virtual bool hasCentrePoint() { return true; };

private:
    Triangle();
    void scale();

    LongComplex m_default_p0;
    LongComplex m_p0;

    static boost::shared_ptr<Triangle> s_outer_instance;
    static boost::shared_ptr<Triangle> s_inner_instance;
};

class Triform : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<Triform> instance(bool inner);

    bool hasRotation() { return true; };

    long double distance(const LongComplex &value);
private:
    Triform();
    void scale();

    LongComplex m_default_p0;
    LongComplex m_p0;

    static boost::shared_ptr<Triform> s_outer_instance;
    static boost::shared_ptr<Triform> s_inner_instance;
};

class Asterisk : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<Asterisk> instance(bool inner);

    bool hasRotation() { return true; };

    long double distance(const LongComplex &value);
private:
    Asterisk();
    void scale();

    LongComplex m_default_p0;
    LongComplex m_p0;

    static boost::shared_ptr<Asterisk> s_outer_instance;
    static boost::shared_ptr<Asterisk> s_inner_instance;
};

class CircleLine : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<CircleLine> instance(bool inner);

    bool hasRotation() { return true; };

    long double distance(const LongComplex &value);
private:
    CircleLine();
    void scale();

    LongComplex m_default_p0;
    LongComplex m_p0;

    static boost::shared_ptr<CircleLine> s_outer_instance;
    static boost::shared_ptr<CircleLine> s_inner_instance;
};

class CircleCross : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<CircleCross> instance(bool inner);

    bool hasRotation() { return true; };

    long double distance(const LongComplex &value);
private:
    CircleCross();
    void scale();

    LongComplex m_default_p0;
    LongComplex m_default_p1;
    LongComplex m_p0;
    LongComplex m_p1;

    static boost::shared_ptr<CircleCross> s_outer_instance;
    static boost::shared_ptr<CircleCross> s_inner_instance;
};

class CircleTriform : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<CircleTriform> instance(bool inner);

    bool hasRotation() { return true; };

    long double distance(const LongComplex &value);
private:
    CircleTriform();
    void scale();

    LongComplex m_default_p0;
    LongComplex m_p0;

    static boost::shared_ptr<CircleTriform> s_outer_instance;
    static boost::shared_ptr<CircleTriform> s_inner_instance;
};

class QuarterCircles : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<QuarterCircles> instance(bool inner);

    bool hasRotation() { return true; };

    long double distance(const LongComplex &value);
private:
    QuarterCircles();
    void scale();

    LongComplex m_default_p0;
    LongComplex m_default_p1;
    LongComplex m_p0;
    LongComplex m_p1;

    static boost::shared_ptr<QuarterCircles> s_outer_instance;
    static boost::shared_ptr<QuarterCircles> s_inner_instance;
};

class CircleTriangle : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<CircleTriangle> instance(bool inner);

    bool hasRotation() { return true; };
    void setCircleToInner(bool inner);

    long double distance(const LongComplex &value);
    virtual bool hasCentrePoint() { return true; };

private:
    CircleTriangle();
    void scale();

    // Base line when triangle is outside the circle
    LongComplex m_outer_default_p0;
    // Base line when triangle is inside the circle.
    LongComplex m_inner_default_p0;
    LongComplex m_p0;

    bool m_inner_circle; // Determines whether the circle is inside or outside.
    long double m_outer_radius;
    long double m_inner_radius;
    long double m_radius;

    static boost::shared_ptr<CircleTriangle> s_outer_instance;
    static boost::shared_ptr<CircleTriangle> s_inner_instance;
};

class CircleSquare : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<CircleSquare> instance(bool inner);

    bool hasRotation() { return true; };
    void setCircleToInner(bool inner);

    long double distance(const LongComplex &value);
    virtual bool hasCentrePoint() { return true; };

private:
    CircleSquare();
    void scale();

    // Base line when Square is outside the circle
    LongComplex m_outer_default_p0;
    // Base line when Square is inside the circle.
    LongComplex m_inner_default_p0;
    LongComplex m_p0;

    bool m_inner_circle; // Determines whether the circle is inside or outside.

    static boost::shared_ptr<CircleSquare> s_outer_instance;
    static boost::shared_ptr<CircleSquare> s_inner_instance;
};

class Octothorpe : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<Octothorpe> instance(bool inner);
    bool hasRotation() { return true; };
    void setCircleToInner(bool inner);

    long double distance(const LongComplex &value);
    virtual bool hasCentrePoint() { return true; };

private:
    Octothorpe();
    void scale();

    LongComplex m_default_p0;
    LongComplex m_default_p1;
    LongComplex m_p0;
    LongComplex m_p1;

    static boost::shared_ptr<Octothorpe> s_outer_instance;
    static boost::shared_ptr<Octothorpe> s_inner_instance;
};

class RunningTrack : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<RunningTrack> instance(bool inner);
    bool hasRotation() { return true; };
    void setCircleToInner(bool inner);

    long double distance(const LongComplex &value);
    virtual bool hasCentrePoint() { return true; };

private:
    RunningTrack();
    void scale();

    LongComplex m_default_p0;
    LongComplex m_default_p1;
    LongComplex m_p0;
    LongComplex m_p1;

    static boost::shared_ptr<RunningTrack> s_outer_instance;
    static boost::shared_ptr<RunningTrack> s_inner_instance;
};

class Pinch : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<Pinch> instance(bool inner);
    bool hasRotation() { return true; };
    void setCircleToInner(bool inner);

    long double distance(const LongComplex &value);
private:
    Pinch();
    void scale();

    LongComplex m_default_p0;
    LongComplex m_default_p1;
    LongComplex m_p0;
    LongComplex m_p1;

    static boost::shared_ptr<Pinch> s_outer_instance;
    static boost::shared_ptr<Pinch> s_inner_instance;
};


//==============================================================================
// IMPORTANT: the calculateCircles method must be called after the definition
// has been completed and before distance is called.
//==============================================================================

class SteinerChain : public OrbitTrap
{
public:
    static void create();
    static boost::shared_ptr<SteinerChain> instance(bool inner);
    ~SteinerChain();

    bool hasRotation() { return true; };

    void setInversionDistance(long double value);
    void setChainLength(unsigned int value);
    void setConstructionRotation(long double value);
    void setTriangles(bool value);
    void setInnerCircle(bool value);
    void setOuterCircle(bool value);
    void setCircleCentrePoint(bool value);

    long double getInversionDistance();
    unsigned int getChainLength();
    long double getConstructionRotation();
    bool getTriangles();
    bool getInnerCircle();
    bool getOuterCircle();
    bool getCircleCentrePoint();

    void calculateCircles();
    long double distance(const LongComplex &value);
    void SetSize(long double value);

    static boost::shared_ptr<SteinerChain> getTrap(bool inner);

    static const int MIN_CHAIN_CIRCLES;
    static const int MAX_CHAIN_CIRCLES;

private:
    SteinerChain();
    void scale();
    LongComplex relativeToComponent(int index, const LongComplex &z);

    struct Component
    {
        Component() : radius(1.0L), enabled(true), offset_rotation(0.0L) {}
        LongComplex centre;
        long double radius;
        bool enabled;
        LongComplex p0;
        long double offset_rotation;
    };
    typedef std::vector<Component> VectorComponent;

    long double m_inversion_distance;
    unsigned int m_number_of_components;
    VectorComponent m_components;
    bool m_component_central_point;
    bool m_inner_circle;
    bool m_outer_circle;
    bool m_triangles;
    long double m_construction_rotation;
    const long double m_reference_angle; // The angle to the left hand apex of an equilateral triangle

    static boost::shared_ptr<SteinerChain> s_outer_instance;
    static boost::shared_ptr<SteinerChain> s_inner_instance;
};

