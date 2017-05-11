//
// File:   OrbitTraps.cc
// Author: M.R.Eggleston
//
// Created on 08 April 2011, 18:47
//

#include <iostream>
#include <limits>
#include <vector>
#include "OrbitTraps.h"

boost::shared_ptr<Point> Point::s_outer_instance;
boost::shared_ptr<FourPoints> FourPoints::s_outer_instance;
boost::shared_ptr<Line> Line::s_outer_instance;
boost::shared_ptr<Cross> Cross::s_outer_instance;
boost::shared_ptr<Square> Square::s_outer_instance;
boost::shared_ptr<Circle> Circle::s_outer_instance;
boost::shared_ptr<Triangle> Triangle::s_outer_instance;
boost::shared_ptr<Triform> Triform::s_outer_instance;
boost::shared_ptr<Asterisk> Asterisk::s_outer_instance;
boost::shared_ptr<CircleLine> CircleLine::s_outer_instance;
boost::shared_ptr<CircleCross> CircleCross::s_outer_instance;
boost::shared_ptr<CircleTriform> CircleTriform::s_outer_instance;
boost::shared_ptr<QuarterCircles> QuarterCircles::s_outer_instance;
boost::shared_ptr<CircleTriangle> CircleTriangle::s_outer_instance;
boost::shared_ptr<CircleSquare> CircleSquare::s_outer_instance;
boost::shared_ptr<Octothorpe> Octothorpe::s_outer_instance;
boost::shared_ptr<RunningTrack> RunningTrack::s_outer_instance;
boost::shared_ptr<Pinch> Pinch::s_outer_instance;
boost::shared_ptr<SteinerChain> SteinerChain::s_outer_instance;

boost::shared_ptr<Point> Point::s_inner_instance;
boost::shared_ptr<FourPoints> FourPoints::s_inner_instance;
boost::shared_ptr<Line> Line::s_inner_instance;
boost::shared_ptr<Cross> Cross::s_inner_instance;
boost::shared_ptr<Square> Square::s_inner_instance;
//boost::shared_ptr<Grid> Grid::s_inner_instance;
//boost::shared_ptr<Mesh> Mesh::s_inner_instance;
boost::shared_ptr<Circle> Circle::s_inner_instance;
boost::shared_ptr<Triangle> Triangle::s_inner_instance;
boost::shared_ptr<Triform> Triform::s_inner_instance;
boost::shared_ptr<Asterisk> Asterisk::s_inner_instance;
boost::shared_ptr<CircleLine> CircleLine::s_inner_instance;
boost::shared_ptr<CircleCross> CircleCross::s_inner_instance;
boost::shared_ptr<CircleTriform> CircleTriform::s_inner_instance;
boost::shared_ptr<QuarterCircles> QuarterCircles::s_inner_instance;
boost::shared_ptr<CircleTriangle> CircleTriangle::s_inner_instance;
boost::shared_ptr<CircleSquare> CircleSquare::s_inner_instance;
boost::shared_ptr<Octothorpe> Octothorpe::s_inner_instance;
boost::shared_ptr<RunningTrack> RunningTrack::s_inner_instance;
boost::shared_ptr<Pinch> Pinch::s_inner_instance;
boost::shared_ptr<SteinerChain> SteinerChain::s_inner_instance;

OrbitTrap::TrapMap OrbitTrap::s_inner_traps;
OrbitTrap::TrapMap OrbitTrap::s_outer_traps;

const long double OrbitTrap::m_pi = 2.0L*acos(0.0L);

//==============================================================================
//==                        base OrbitTraps                                   ==
//==============================================================================

OrbitTrap::OrbitTrap()
:
m_size(1.0L),
m_central_point(false),
m_rotation(0),
m_r11(1.0L),
m_r12(0.0L),
m_r21(0.0L),
m_r22(1.0L),
m_sr11(1.0L),
m_sr12(0.0L),
m_sr21(0.0L),
m_sr22(1.0L)
{
}

boost::shared_ptr<OrbitTrap> OrbitTrap::getTrap(OrbitTrap::TRAP_TYPE type, bool inner)
{
    TrapMap &map = (inner ? s_inner_traps : s_outer_traps);
    auto it = map.find(type);
    bool create = (it == map.end());
    switch (type)
    {
    case POINT:
        if (create) Point::create();
        map[type] = Point::instance(inner);
        break;
    case FOUR_POINTS:
        if (create) FourPoints::create();
        map[type] = FourPoints::instance(inner);
        break;
    case LINE:
        if (create) Line::create();
        map[type] = Line::instance(inner);
        break;
    case CROSS:
        if (create) Cross::create();
        map[type] = Cross::instance(inner);
        break;
    case SQUARE:
        if (create) Square::create();
        map[type] = Square::instance(inner);
        break;
    case CIRCLE:
        if (create) Circle::create();
        map[type] = Circle::instance(inner);
        break;
    case TRIANGLE:
        if (create) Triangle::create();
        map[type] = Triangle::instance(inner);
        break;
    case TRIFORM:
        if (create) Triform::create();
        map[type] = Triform::instance(inner);
        break;
    case ASTERISK:
        if (create) Asterisk::create();
        map[type] = Asterisk::instance(inner);
        break;
    case CIRCLE_LINE:
        if (create) CircleLine::create();
        map[type] = CircleLine::instance(inner);
        break;
    case CIRCLE_CROSS:
        if (create) CircleCross::create();
        map[type] = CircleCross::instance(inner);
        break;
    case CIRCLE_TRIFORM:
        if (create) CircleTriform::create();
        map[type] = CircleTriform::instance(inner);
        break;
    case QUARTER_CIRCLES:
        if (create) QuarterCircles::create();
        map[type] = QuarterCircles::instance(inner);
        break;
    case CIRCLE_TRIANGLE:
    case TRIANGLE_CIRCLE:
        if (create) CircleTriangle::create();
        map[type] = CircleTriangle::instance(inner);
        CircleTriangle::instance(inner)->setCircleToInner(type == TRIANGLE_CIRCLE);
        break;
    case CIRCLE_SQUARE:
    case SQUARE_CIRCLE:
        if (create) CircleSquare::create();
        map[type] = CircleSquare::instance(inner);
        CircleSquare::instance(inner)->setCircleToInner(type == SQUARE_CIRCLE);
        break;
    case OCTOTHORPE:
        if (create) Octothorpe::create();
        map[type] = Octothorpe::instance(inner);
        break;
    case RUNNING_TRACK:
        if (create) RunningTrack::create();
        map[type] = RunningTrack::instance(inner);
        break;
    case PINCH:
        if (create) Pinch::create();
        map[type] = Pinch::instance(inner);
        break;
    case STEINER_CHAIN:
        if (create) SteinerChain::create();
        map[type] = SteinerChain::instance(inner);
        break;
    }
    return map[type];
}


//OrbitTrap::OrbitTrap(const OrbitTrap& orig)
//{
//}

OrbitTrap::~OrbitTrap()
{
}

void OrbitTrap::SetPosition(const LongComplex &value)
{
    m_position = value;
}

void OrbitTrap::SetSize(long double value)
{
    m_size = value;
    scale();
}

void OrbitTrap::SetRotation(unsigned int value)
{
    m_rotation = value;
    setPrimaryRotationMatrix(value);
}

unsigned int OrbitTrap::GetRotation()
{
    return m_rotation;
}

LongComplex OrbitTrap::GetPosition()
{
    return m_position;
}

long double OrbitTrap::GetSize()
{
    return m_size;
}

void OrbitTrap::EnableCentralPoint(bool value)
{
    m_central_point = value;
}

bool OrbitTrap::isCentralPointEnabled()
{
    return m_central_point;
}

LongComplex OrbitTrap::unsign(const LongComplex &value)
{
    return LongComplex(std::abs(real(value)), std::abs(imag(value)));
}

LongComplex OrbitTrap::normalisePoint(const LongComplex &value)
{
    LongComplex z = value - m_position;
    //
    // Apply transformation matrix
    //
    if (m_rotation != 0)
    {
        long double new_real = real(z)*m_r11 + imag(z)*m_r21;
        long double new_imag = real(z)*m_r12 + imag(z)*m_r22;
        z = LongComplex(new_real, new_imag);
    }
    return z;
}

LongComplex OrbitTrap::rotatePoint(const LongComplex &z)
{
    long double new_real = real(z)*m_sr11 + imag(z)*m_sr21;
    long double new_imag = real(z)*m_sr12 + imag(z)*m_sr22;
    return LongComplex(new_real, new_imag);
}

void OrbitTrap::setPrimaryRotationMatrix(unsigned int angle)
{
    using namespace std;
    long double radians = -(static_cast<long double>(angle)/180.0L)*m_pi;
    m_r11 = cos(radians);
    m_r12 = sin(radians);
    m_r21 = -m_r12;
    m_r22 = m_r11;
}

void OrbitTrap::setSecondaryRotationMatrix(unsigned int angle)
{
    using namespace std;
    long double radians = -(static_cast<long double>(angle)/180.0L)*m_pi;
    m_sr11 = cos(radians);
    m_sr12 = sin(radians);
    m_sr21 = -m_sr12;
    m_sr22 = m_sr11;
}

long double OrbitTrap::checkCentralPoint(const LongComplex &position, long double dist)
{
    if (m_central_point)
    {
        long double dist2 = ld::abs(position);
        if (dist2 < dist)
        {
            dist = dist2;
        }
    }
    return dist;
}

//==============================================================================
//==                                  Point                                   ==
//==============================================================================

Point::Point()
:
OrbitTrap()
{
}

void Point::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<Point>(new Point());
        s_inner_instance = boost::shared_ptr<Point>(new Point());
    }
}

boost::shared_ptr<Point> Point::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

long double Point::distance(const LongComplex& value)
{
    return ld::abs(value - m_position);
}

//==============================================================================
//==                                  Four Points                             ==
//==============================================================================

FourPoints::FourPoints()
:
OrbitTrap(),
m_default_p0(0.5L, 0.5L),
m_p0(m_default_p0)
{
}

void FourPoints::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<FourPoints>(new FourPoints());
        s_inner_instance = boost::shared_ptr<FourPoints>(new FourPoints());
    }
}

boost::shared_ptr<FourPoints> FourPoints::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void FourPoints::scale()
{
    m_p0 = m_default_p0*m_size;
}

long double FourPoints::distance(const LongComplex& value)
{
    LongComplex z = unsign(normalisePoint(value));
    return checkCentralPoint(z, ld::abs(z - m_p0));
}

//==============================================================================
//==                                  Line                                    ==
//==============================================================================

Line::Line()
:
OrbitTrap(),
m_default_p0(LongComplex(0.5L, 0.0L)),
m_p0(m_default_p0)
{
}

void Line::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<Line>(new Line());
        s_inner_instance = boost::shared_ptr<Line>(new Line());
    }
}

boost::shared_ptr<Line> Line::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void Line::scale()
{
    m_p0 = m_default_p0*m_size;
}

long double Line::distance(const LongComplex& value)
{
    using std::abs;
    LongComplex z = unsign(normalisePoint(value));
    long double dist;
    if (real(z) > real(m_p0))
    {
        dist = abs(z - m_p0);
    }
    else
    {
        dist = abs(imag(z));
    }
    return dist;
}

//==============================================================================
//==                                  Cross                                   ==
//==============================================================================

Cross::Cross()
:
OrbitTrap(),
m_default_p0(LongComplex(0.5L, 0.0L)),
m_default_p1(LongComplex(0.0L, 0.5L)),
m_p0(m_default_p0),
m_p1(m_default_p1)
{
}

void Cross::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<Cross>(new Cross());
        s_inner_instance = boost::shared_ptr<Cross>(new Cross());
    }
}

boost::shared_ptr<Cross> Cross::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void Cross::scale()
{
    m_p0 = m_default_p0*m_size;
    m_p1 = m_default_p1*m_size;
}

long double Cross::distance(const LongComplex& value)
{
    using std::abs;
    LongComplex z = unsign(normalisePoint(value));
    long double dist1;
    long double dist2;
    if (real(z) > real(m_p0))
    {
        dist1 = abs(z - m_p0);
    }
    else
    {
        dist1 = abs(imag(z));
    }
    if (imag(z) > imag(m_p1))
    {
        dist2 = abs(z - m_p1);
    }
    else
    {
        dist2 = abs(real(z));
    }
    return (dist1 < dist2) ? dist1 : dist2;
}

//==============================================================================
//==                                  Square                                  ==
//==============================================================================

Square::Square()
:
OrbitTrap(),
m_default_p0(0.5L, 0.5L),
m_p0(m_default_p0)
{
}

void Square::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<Square>(new Square());
        s_inner_instance = boost::shared_ptr<Square>(new Square());
    }
}

boost::shared_ptr<Square> Square::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void Square::scale()
{
    m_p0 = m_default_p0*m_size;
}

long double Square::distance(const LongComplex& value)
{
    using std::abs;
    LongComplex z = unsign(normalisePoint(value));
    long double dist;
    if ((real(z) > real(m_p0)) && (imag(z) > imag(m_p0)))
    {
        dist = ld::abs(z - m_p0);
    }
    else if (real(z) > real(m_p0))
    {
        dist = real(z) - real(m_p0);
    }
    else if (imag(z) > imag(m_p0))
    {
        dist = imag(z) - imag(m_p0);
    }
    else
    {
        long double dist1 = real(m_p0) - real(z);
        long double dist2 = imag(m_p0) - imag(z);
        dist = (dist1 < dist2 ? dist1 : dist2);
    }
    return checkCentralPoint(z, dist);
}

//==============================================================================
//==                                  Grid                                    ==
//==============================================================================

//Grid::Grid()
//:
//OrbitTrap(),
//m_default_p0(0.5L, 0.5L),
//m_p0(m_default_p0)
//{
//}
//
//void Grid::create()
//{
//    if (!s_outer_instance)
//    {
//        s_outer_instance = boost::shared_ptr<Grid>(new Grid());
//        s_inner_instance = boost::shared_ptr<Grid>(new Grid());
//    }
//}
//
//boost::shared_ptr<Grid> Grid::instance(bool inner)
//{
//    return (inner ? s_inner_instance : s_outer_instance);
//}
//
//void Grid::scale()
//{
//    m_p0 = m_default_p0*m_size;
//}
//
//long double Grid::distance(const LongComplex& value)
//{
//    using std::abs;
//    LongComplex z = unsign(normalisePoint(value));
//    long double dist;
//    if ((real(z) > real(m_p0)) && (imag(z) > imag(m_p0)))
//    {
//        dist = abs(z - m_p0);
//    }
////    else if (real(value) > real(m_p0))
////    {
////        dist = real(value) - real(m_p0);
////    }
////    else if (imag(value) > imag(m_p0))
////    {
////        dist = imag(value) - imag(m_p0);
////    }
////    else
////    {
////        long dist1 = real(m_p0) - real(value);
////        long dist2 = imag(m_p0) - imag(value);
////    }
//    return adjustForThickness(checkCentralPoint(z, dist));
//}

//==============================================================================
//==                                  Mesh                                    ==
//==============================================================================

//Mesh::Mesh()
//:
//OrbitTrap(),
//m_default_p0(0.5L, 0.5L),
//m_p0(m_default_p0)
//{
//}
//
//void Mesh::create()
//{
//    if (!s_outer_instance)
//    {
//        s_outer_instance = boost::shared_ptr<Mesh>(new Mesh());
//        s_inner_instance = boost::shared_ptr<Mesh>(new Mesh());
//    }
//}
//
//boost::shared_ptr<Mesh> Mesh::instance(bool inner)
//{
//    return (inner ? s_inner_instance : s_outer_instance);
//}
//
//void Mesh::scale()
//{
//    m_p0 = m_default_p0*m_size;
//}
//
//long double Mesh::distance(const LongComplex& value)
//{
//    long double dist;
//    LongComplex z = unsign(normalisePoint(value));
//    if ((real(z) > real(m_p0)) && (imag(z) > imag(m_p0)))
//    {
//        dist = std::abs(z - m_p0);
//    }
//    else if (real(z) > real(m_p0))
//    {
//        dist = real(z) - real(m_p0);
//    }
//    else if (imag(z) > imag(m_p0))
//    {
//        dist = imag(z) - imag(m_p0);
//    }
//    else
//    {
//        long dist1 = real(m_p0) - real(z);
//        long dist2 = imag(m_p0) - imag(z);
//        dist = (dist1 < dist2 ? dist1 : dist2);
//    }
//    return adjustForThickness(checkCentralPoint(z, dist));
//}
//
//==============================================================================
//==                                  Circle                                  ==
//==============================================================================

Circle::Circle()
:
OrbitTrap()
{
}

void Circle::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<Circle>(new Circle());
        s_inner_instance = boost::shared_ptr<Circle>(new Circle());
    }
}

boost::shared_ptr<Circle> Circle::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

long double Circle::distance(const LongComplex& value)
{
    LongComplex z = normalisePoint(value);
    return checkCentralPoint(z, std::abs(m_size/2.0L - ld::abs(z)));
}

//==============================================================================
//==                                  Triangle                                ==
//==============================================================================

Triangle::Triangle()
:
OrbitTrap(),
m_default_p0(0.5L, -0.5L*tan(m_pi/6.0L)),
m_p0(m_default_p0)
{
    setSecondaryRotationMatrix(120);
}

void Triangle::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<Triangle>(new Triangle());
        s_inner_instance = boost::shared_ptr<Triangle>(new Triangle());
    }
}

boost::shared_ptr<Triangle> Triangle::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void Triangle::scale()
{
    m_p0 = m_default_p0*m_size;
}

long double Triangle::distance(const LongComplex& value)
{
    // The Triangle trap is symmetical about the imaginary axis. The triangle is
    // centred on the origin and with equal scaling is equilateral with a horizontal
    // line at its base. The distance to the trap is is determined by the base
    // since the trap is symmetical about the imaginary axis only the right hand
    // end of the line is required, if the absolute value of the point to be tested
    // is greater than the end of the base line the distance is the distance to point
    // end of the line otherwise it is the absolute value of imaginary position of the
    // point minus the imaginary point of the line. Three distances are determined,
    // after each distance is found the point to be tested is rotated 120 degrees
    // about the origin thus each side can be tested easily by reference to the baseline.
    using std::abs;
    LongComplex z = normalisePoint(value);
    long double dist;
    long double dist2;
    if (std::abs(real(z)) > real(m_p0))
    {
        dist = ld::abs(z - m_p0);
    }
    else
    {
        dist = ld::abs(imag(z) - imag(m_p0));
    }
    for (int i=0; i<2; i++)
    {
        z = rotatePoint(z);
        if (ld::abs(real(z)) > real(m_p0))
        {
            dist2 = ld::abs(z - m_p0);
        }
        else
        {
            dist2 = std::abs(imag(z) - imag(m_p0));
        }
        if (dist2 < dist) dist = dist2;
    }
    return checkCentralPoint(z, dist);
}

//==============================================================================
//==                                  Triform                                 ==
//==============================================================================

Triform::Triform()
:
OrbitTrap(),
m_default_p0(0.0L, sin(m_pi/3.0L)-0.5L*tan(m_pi/6.0L)),
m_p0(m_default_p0)
{
    setSecondaryRotationMatrix(120);
}

void Triform::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<Triform>(new Triform());
        s_inner_instance = boost::shared_ptr<Triform>(new Triform());
    }
}

boost::shared_ptr<Triform> Triform::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void Triform::scale()
{
    m_p0 = m_default_p0*m_size;
}

long double Triform::distance(const LongComplex& value)
{
    // The Triform trap is symmetical about the imaginary axis. The Triform is
    // centred on the origin and with equal scaling consists of the line from
    // the corners of an equilateral triangle to its centre. The vertical line
    // from centre to apex is used as the basis for determining the distance to
    // trap only the apex point is required, for points above that point, the
    // distance is to the end point, for negative imaginary values it is the
    // distance to the origin otherwise it is the real component of the
    // point tested. The distance to the origin is determined to prevent an
    // incorrect minimum distance being found for points with a negative
    // imaginary part. Three distances are determined, after each distance is
    // found the point to be tested is rotated 120 degrees about the origin thus
    // each spoke can be tested easily by reference to the vertical spoke.
    using std::abs;
    LongComplex z = normalisePoint(value);
    long double dist;
    long double dist2;
    if (imag(z) > imag(m_p0))
    {
        dist = ld::abs(z - m_p0);
    }
    else if (imag(z) < 0.0L)
    {
        dist = ld::abs(z);
    }
    else
    {
        dist = ld::abs(real(z));
    }
    for (int i=0; i<2; i++)
    {
        z = rotatePoint(z);
        if (imag(z) > imag(m_p0))
        {
            dist2 = ld::abs(z - m_p0);
        }
        else if (imag(z) < 0.0L)
        {
            dist2 = ld::abs(z);
        }
        else
        {
            dist2 = ld::abs(real(z));
        }
        if (dist2 < dist) dist = dist2;
    }
    return dist;
}

//==============================================================================
//==                                  Asterisk                                ==
//==============================================================================

Asterisk::Asterisk()
:
OrbitTrap(),
m_default_p0(0.0L, sin(m_pi/3.0L)-0.5L*tan(m_pi/6.0L)),
m_p0(m_default_p0)
{
    setSecondaryRotationMatrix(120);
}

void Asterisk::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<Asterisk>(new Asterisk());
        s_inner_instance = boost::shared_ptr<Asterisk>(new Asterisk());
    }
}

boost::shared_ptr<Asterisk> Asterisk::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void Asterisk::scale()
{
    m_p0 = m_default_p0*m_size;
}

long double Asterisk::distance(const LongComplex& value)
{
    // The Asterisk trap is symmetical about the imaginary axis. The Asterisk is
    // centred on the origin and with equal scaling consists of the three lines
    // with 60 degree angles between them with on line vertical. The vertical line
    // is used as the basis for determining the distance to trap only the top
    // point is required, for points above that point, the
    // distance is to the end point otherwise it is the real component of the
    // point tested. Three distances are determined, after each distance is
    // found the point to be tested is rotated 60 degrees about the origin thus
    // each spoke can be tested easily by reference to the vertical spoke.
    using std::abs;
    LongComplex z = unsign(normalisePoint(value));
    long double dist;
    long double dist2;
    if (imag(z) > imag(m_p0))
    {
        dist = ld::abs(z - m_p0);
    }
    else
    {
        dist = real(z);
    }
    for (int i=0; i<2; i++)
    {
        z = unsign(rotatePoint(z));
        if (imag(z) > imag(m_p0))
        {
            dist2 = ld::abs(z - m_p0);
        }
        else
        {
            dist2 = real(z);
        }
        if (dist2 < dist) dist = dist2;
    }
    return dist;
}

//==============================================================================
//==                                  CircleLine                              ==
//==============================================================================

CircleLine::CircleLine()
:
OrbitTrap(),
m_default_p0(0.5L, 0.0L),
m_p0(m_default_p0)
{
}

void CircleLine::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<CircleLine>(new CircleLine());
        s_inner_instance = boost::shared_ptr<CircleLine>(new CircleLine());
    }
}

boost::shared_ptr<CircleLine> CircleLine::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void CircleLine::scale()
{
    m_p0 = m_default_p0*m_size;
}

long double CircleLine::distance(const LongComplex& value)
{
    // The CircleLine trap is a combination of a Circle and a Line the line
    // forms a horizontal diameter.
    using std::abs;
    LongComplex z = unsign(normalisePoint(value));
    long double dist;
    if (real(z) > real(m_p0))
    {
        dist = abs(z - m_p0);
    }
    else
    {
        dist = imag(z);
    }
    long double dist2 = abs(m_size/2.0L - ld::abs(z));
    if (dist2 < dist) dist = dist2;
    return dist;
}

//==============================================================================
//==                                  CircleCross                             ==
//==============================================================================

CircleCross::CircleCross()
:
OrbitTrap(),
m_default_p0(LongComplex(0.5L, 0.0L)),
m_default_p1(LongComplex(0.0L, 0.5L)),
m_p0(m_default_p0),
m_p1(m_default_p1)
{
}

void CircleCross::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<CircleCross>(new CircleCross());
        s_inner_instance = boost::shared_ptr<CircleCross>(new CircleCross());
    }
}

boost::shared_ptr<CircleCross> CircleCross::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void CircleCross::scale()
{
    m_p0 = m_default_p0*m_size;
    m_p1 = m_default_p1*m_size;
}

long double CircleCross::distance(const LongComplex& value)
{
    // The CircleCross trap is a combination of a Circle and a Line the line
    // forms a horizontal diameter.
    using std::abs;
    LongComplex z = unsign(normalisePoint(value));
    long double dist;
    long double dist1;
    long double dist2;
    if (real(z) > real(m_p0))
    {
        dist1 = abs(z - m_p0);
    }
    else
    {
        dist1 = abs(imag(z));
    }
    if (imag(z) > imag(m_p1))
    {
        dist2 = abs(z - m_p1);
    }
    else
    {
        dist2 = abs(real(z));
    }
    dist = (dist1 < dist2) ? dist1 : dist2;
    dist2 = std::abs(m_size/2.0L - ld::abs(z));
    if (dist2 < dist) dist = dist2;
    return dist;
}

//==============================================================================
//==                                  CircleTriform                           ==
//==============================================================================

CircleTriform::CircleTriform()
:
OrbitTrap(),
m_default_p0(0.0L, 0.5L),
m_p0(m_default_p0)
{
    setSecondaryRotationMatrix(120);
}

void CircleTriform::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<CircleTriform>(new CircleTriform());
        s_inner_instance = boost::shared_ptr<CircleTriform>(new CircleTriform());
    }
}

boost::shared_ptr<CircleTriform> CircleTriform::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void CircleTriform::scale()
{
    m_p0 = m_default_p0*m_size;
}

long double CircleTriform::distance(const LongComplex& value)
{
    // The CircleTriform trap is symmetical about the imaginary axis. The Triform is
    // centred on the origin and with equal scaling consists of the line from
    // the corners of an equilateral triangle to its centre. The vertical line
    // from centre to apex is used as the basis for determining the distance to
    // trap only the apex point is required, for points above that point, the
    // distance is to the end point, for negative imaginary values it is the
    // distance to the origin otherwise it is the real component of the
    // point tested. The distance to the origin is determined to prevent an
    // incorrect minimum distance being found for points with a negative
    // imaginary part. Three distances are determined, after each distance is
    // found the point to be tested is rotated 120 degrees about the origin thus
    // each spoke can be tested easily by reference to the vertical spoke.
    using std::abs;
    LongComplex z = normalisePoint(value);
    long double dist;
    long double dist2;
    if (imag(z) > imag(m_p0))
    {
        dist = ld::abs(z - m_p0);
    }
    else if (imag(z) < 0.0L)
    {
        dist = ld::abs(z);
    }
    else
    {
        dist = ld::abs(real(z));
    }
    for (int i=0; i<2; i++)
    {
        z = rotatePoint(z);
        if (imag(z) > imag(m_p0))
        {
            dist2 = ld::abs(z - m_p0);
        }
        else if (imag(z) < 0.0L)
        {
            dist2 = ld::abs(z);
        }
        else
        {
            dist2 = std::abs(real(z));
        }
        if (dist2 < dist) dist = dist2;
    }
    dist2 = std::abs(m_size/2.0L - ld::abs(z));
    if (dist2 < dist) dist = dist2;
    return dist;
}

//==============================================================================
//==                                  QuarterCircles                          ==
//==============================================================================

QuarterCircles::QuarterCircles()
:
OrbitTrap(),
m_default_p0(LongComplex(0.5L, 0.0L)),
m_default_p1(LongComplex(0.0L, 0.5L)),
m_p0(m_default_p0),
m_p1(m_default_p1)
{
}

void QuarterCircles::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<QuarterCircles>(new QuarterCircles());
        s_inner_instance = boost::shared_ptr<QuarterCircles>(new QuarterCircles());
    }
}

boost::shared_ptr<QuarterCircles> QuarterCircles::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void QuarterCircles::scale()
{
    m_p0 = m_default_p0*m_size;
    m_p1 = m_default_p1*m_size;
}

long double QuarterCircles::distance(const LongComplex& value)
{
    // The QuarterCircles trap is a combination of a Circle and a Line the line
    // forms a horizontal diameter.
    using std::abs;
    LongComplex z = normalisePoint(value);
    LongComplex z_signed = z;
    z = unsign(z);
    long double dist;
    long double dist1;
    long double dist2;
    if (real(z) > real(m_p0))
    {
        dist1 = abs(z - m_p0);
    }
    else
    {
        dist1 = abs(imag(z));
    }
    if (imag(z) > imag(m_p1))
    {
        dist2 = abs(z - m_p1);
    }
    else
    {
        dist2 = abs(real(z));
    }
    dist = (dist1 < dist2) ? dist1 : dist2;
    if (((real(z_signed) > 0.0L) && (imag(z_signed) < 0.0L)) ||
        ((real(z_signed) < 0.0L) && (imag(z_signed) > 0.0L)))
    {
        dist2 = abs(m_size/2.0L - ld::abs(z));
        if (dist2 < dist) dist = dist2;
    }
    return dist;
}

//==============================================================================
//==                                  CircleTriangle                          ==
//==============================================================================

CircleTriangle::CircleTriangle()
:
OrbitTrap(),
m_outer_default_p0(0.5L, -0.5L*tan(m_pi/6.0L)),
m_inner_default_p0(0.5L*cos(m_pi/6.0L), -0.5L*sin(m_pi/6.0L)),
m_p0(m_inner_default_p0),
m_inner_circle(false),
m_outer_radius(0.5L),
m_inner_radius(std::abs(imag(m_outer_default_p0))),
m_radius(m_outer_radius)
{
    setSecondaryRotationMatrix(120);
}

void CircleTriangle::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<CircleTriangle>(new CircleTriangle());
        s_inner_instance = boost::shared_ptr<CircleTriangle>(new CircleTriangle());
    }
}

boost::shared_ptr<CircleTriangle> CircleTriangle::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void CircleTriangle::scale()
{
    // NOTE: when the circle is inner p0 is outer.
    m_p0 = (m_inner_circle ? m_outer_default_p0 : m_inner_default_p0)*m_size;
    m_radius = (m_inner_circle ? m_inner_radius : m_outer_radius)*m_size;
}

void CircleTriangle::setCircleToInner(bool inner)
{
    m_inner_circle = inner;
    scale();
}

long double CircleTriangle::distance(const LongComplex& value)
{
    // This trap is same as the Triangle trap with the addition of a circle
    // which meets the triangle at its corners.
    using std::abs;
    LongComplex z = normalisePoint(value);
    long double dist;
    long double dist2;
    if (std::abs(real(z)) > real(m_p0))
    {
        dist = ld::abs(z - m_p0);
    }
    else
    {
        dist = std::abs(imag(z) - imag(m_p0));
    }
    for (int i=0; i<2; i++)
    {
        z = rotatePoint(z);
        if (std::abs(real(z)) > real(m_p0))
        {
            dist2 = ld::abs(z - m_p0);
        }
        else
        {
            dist2 = std::abs(imag(z) - imag(m_p0));
        }
        if (dist2 < dist) dist = dist2;
    }
    dist2 = abs(m_radius - ld::abs(z));
    if (dist2 < dist) dist = dist2;
    return checkCentralPoint(z, dist);
}

//==============================================================================
//==                                  CircleSquare                          ==
//==============================================================================

CircleSquare::CircleSquare()
:
OrbitTrap(),
m_outer_default_p0(0.5L, 0.5L),
m_inner_default_p0(0.5L*cos(m_pi/4.0L), 0.5L*sin(m_pi/4.0L)),
m_p0(m_inner_default_p0),
m_inner_circle(false)
{
}

void CircleSquare::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<CircleSquare>(new CircleSquare());
        s_inner_instance = boost::shared_ptr<CircleSquare>(new CircleSquare());
    }
}

boost::shared_ptr<CircleSquare> CircleSquare::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void CircleSquare::scale()
{
    // NOTE: when the circle is inner p0 is outer.
    m_p0 = (m_inner_circle ? m_outer_default_p0 : m_inner_default_p0)*m_size;
    // NOTE: regardless of whether the circle is inner or outer its radius
    // does not change.
}

void CircleSquare::setCircleToInner(bool inner)
{
    m_inner_circle = inner;
    scale();
}

long double CircleSquare::distance(const LongComplex& value)
{
    // This trap is same as the Square trap with the addition of a circle
    // which meets the Square at its corners.
    using std::abs;
    LongComplex z = unsign(normalisePoint(value));
    long double dist;
    long double dist2;
    if ((real(z) > real(m_p0)) && (imag(z) > imag(m_p0)))
    {
        dist = ld::abs(z - m_p0);
    }
    else if (real(z) > real(m_p0))
    {
        dist = real(z) - real(m_p0);
    }
    else if (imag(z) > imag(m_p0))
    {
        dist = imag(z) - imag(m_p0);
    }
    else
    {
        dist = real(m_p0) - real(z);
        dist2 = imag(m_p0) - imag(z);
        dist = (dist < dist2 ? dist : dist2);
    }
    dist2 = abs(m_size/2.0 - ld::abs(z));
    if (dist2 < dist) dist = dist2;
    return checkCentralPoint(z, dist);
}

//==============================================================================
//==                                  Octothorpe                              ==
//==============================================================================

Octothorpe::Octothorpe()
:
OrbitTrap(),
m_default_p0(0.25L, 0.5L),
m_default_p1(0.5L, 0.25L),
m_p0(m_default_p0),
m_p1(m_default_p1)
{
}

void Octothorpe::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<Octothorpe>(new Octothorpe());
        s_inner_instance = boost::shared_ptr<Octothorpe>(new Octothorpe());
    }
}

boost::shared_ptr<Octothorpe> Octothorpe::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void Octothorpe::scale()
{
    m_p0 = m_default_p0*m_size;
    m_p1 = m_default_p1*m_size;
}

long double Octothorpe::distance(const LongComplex& value)
{
    using std::abs;
    LongComplex z = unsign(normalisePoint(value));
    long double dist = 0.0L;
    if ((imag(z) < imag(m_p0)) && (real(z) < real(m_p1)))
    {
        long double dist1 = std::abs(real(z) - real(m_p0));
        long double dist2 = std::abs(imag(z) - imag(m_p1));
        dist = (dist1 < dist2 ? dist1 : dist2);
    }
    else if (imag(z) > real(z))
    {
        dist = ld::abs(z - m_p0);
    }
    else
    {
        dist = ld::abs(z - m_p1);
    }
    return checkCentralPoint(z, dist);
}

//==============================================================================
//==                                  RunningTrack                            ==
//==============================================================================

// i.e. to straight sides and two semi-circles the circumference of the circle
// is equal the sum of the straight sides.

RunningTrack::RunningTrack()
:
OrbitTrap(),
m_default_p0(0.5L - 1.0L/(2.0L + m_pi), 1.0/(2.0L + m_pi)),
m_default_p1(real(m_default_p0), 0.0L),
m_p0(m_default_p0),
m_p1(m_default_p1)
{
}

void RunningTrack::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<RunningTrack>(new RunningTrack());
        s_inner_instance = boost::shared_ptr<RunningTrack>(new RunningTrack());
    }
}

boost::shared_ptr<RunningTrack> RunningTrack::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void RunningTrack::scale()
{
    m_p0 = m_default_p0*m_size;
    m_p1 = m_default_p1*m_size;
}

long double RunningTrack::distance(const LongComplex& value)
{
    using std::abs;
    LongComplex z = unsign(normalisePoint(value));
    long double dist = 0.0L;
    if (real(z) > real(m_p0))
    {
        // imag(m_p0) is the radius of the circle
        dist = std::abs(imag(m_p0) - ld::abs(z - m_p1));
    }
    else
    {
        dist = std::abs(imag(z) - imag(m_p0));
    }
    return checkCentralPoint(z, dist);
}

//==============================================================================
//==                                  Pinch                                   ==
//==============================================================================

// i.e. to straight sides and two semi-circles the circumference of the circle
// is equal the sum of the straight sides.

Pinch::Pinch()
:
OrbitTrap(),
m_default_p0(0.5L, 0.5L),
m_default_p1(0.0L, 0.5L),
m_p0(m_default_p0),
m_p1(m_default_p1)
{
}

void Pinch::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<Pinch>(new Pinch());
        s_inner_instance = boost::shared_ptr<Pinch>(new Pinch());
    }
}

boost::shared_ptr<Pinch> Pinch::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void Pinch::scale()
{
    m_p0 = m_default_p0*m_size;
    m_p1 = m_default_p1*m_size;
}

long double Pinch::distance(const LongComplex& value)
{
    using std::abs;
    LongComplex z = unsign(normalisePoint(value));
    long double dist = 0.0L;
    if (imag(z) > imag(m_p0))
    {
        dist = ld::abs(z - m_p0);
    }
    else if (real(z) > real(m_p0))
    {
        dist = real(z) - real(m_p0);
    }
    else
    {
        long double dist1 = std::abs(imag(m_p1) - ld::abs(z - m_p1));
        long double dist2 = real(m_p0) - real(z);
        dist = (dist1 < dist2 ? dist1 : dist2);
    }
    return checkCentralPoint(z, dist);
}

//==============================================================================
// Steiner Chain
//==============================================================================

const int SteinerChain::MIN_CHAIN_CIRCLES = 5;
const int SteinerChain::MAX_CHAIN_CIRCLES = 64;


SteinerChain::SteinerChain()
:
OrbitTrap(),
m_inversion_distance(0.0L),
m_number_of_components(10),
m_component_central_point(false),
m_inner_circle(true),
m_outer_circle(true),
m_triangles(false),
m_reference_angle(ld::arg(LongComplex(cos(m_pi/6.0L), -sin(m_pi/6.0L))))
{
    setSecondaryRotationMatrix(120);
}

SteinerChain::~SteinerChain()
{
}

void SteinerChain::create()
{
    if (!s_outer_instance)
    {
        s_outer_instance = boost::shared_ptr<SteinerChain>(new SteinerChain());
        s_inner_instance = boost::shared_ptr<SteinerChain>(new SteinerChain());
    }
}

boost::shared_ptr<SteinerChain> SteinerChain::instance(bool inner)
{
    return (inner ? s_inner_instance : s_outer_instance);
}

void SteinerChain::scale()
{
    for (unsigned int i = 0; i < m_number_of_components; i++)
    {
        m_components[i].centre = m_components[i].centre*m_size;
        m_components[i].radius = m_components[i].radius*m_size;
        m_components[i].p0 = m_components[i].p0*m_size;
    }
}

void SteinerChain::setInversionDistance(long double value)
{
    m_inversion_distance = value;
}

long double SteinerChain::getInversionDistance()
{
    return m_inversion_distance;
}

void SteinerChain::setChainLength(unsigned int value)
{
    // The chain of circles is bound between an inner and outer circle
    // hence the + 2
    m_number_of_components = value + 2;
}

unsigned int SteinerChain::getChainLength()
{
    // The chain of circles is bound between an inner and outer circle
    // hence the + 2
    return m_number_of_components- 2;
}

long double SteinerChain::distance(const LongComplex &value)
{
    LongComplex z = normalisePoint(value);
    long double dist = std::numeric_limits<long double>::max();
    for (unsigned int i = 0; i < m_number_of_components; i++)
    {
        long double dist2;
        Component &component = m_components[i];
        if (component.enabled)
        {
            dist2 = std::abs(ld::abs(z - component.centre) - component.radius);
            if (dist2 < dist) dist = dist2;
            if (i > 1)
            {
                if (m_component_central_point)
                {
                    dist2 = ld::abs(z - component.centre);
                    if (dist2 < dist) dist = dist2;
                }
                if (m_triangles)
                {
                    LongComplex p0 = m_components[i].p0;
                    // The component containing the triangle is not at the
                    // and at an angle to determine the distance to the
                    // relative position to the triangle is required.
                    LongComplex z1 = relativeToComponent(i, z);
                    for (int i=0; i<3; i++)
                    {
                        if (std::abs(real(z1)) > real(p0))
                        {
                            dist2 = ld::abs(z1 - p0);
                        }
                        else
                        {
                            dist2 = std::abs(imag(z1) - imag(p0));
                        }
                        if (dist2 < dist) dist = dist2;
                        if (i<2) z1 = rotatePoint(z1);
                    }
                }
            }
        }
    }
    return dist;
};

void SteinerChain::calculateCircles()
{
    // The Steiner Chain is constructed using an Annular Steiner Chain
    // where the chain circles are of equal size. This annular chain is
    // then inverted using an inversion circle at the inversion distance from
    // from the centre. For simplicity the centre of the inversion circle
    // is on the real axis. The circles that are inverted are also circles,
    // however all the points of the circumference map to the new circumference
    // but the centre point does not map to the centre of the new circle. For
    // the inner and outer circles the two points chosen to be inverted are
    // where the circle intersects the real axis, when these points are inverted
    // their positions are such that a line between the points pass through the
    // inverted circle. For the chain circles the two points where they meet the
    // inner and outer circles are chosen, however when they are inverted they
    // map to a chord from point where the inverted circle meets the new inner
    // and outer circles, the line does not pass through the centre of the
    // circle, the centre of the circle can be found from where a line passing
    // the centre of the inner circle and inner end of the chord and a line from
    // the centre of the outer circle to the outer end of the cord intersect.
    // The radius is the distance from the centre of the chain circle to where
    // it meets the outer circle.
    using std::cos;
    using std::sin;
    m_components.resize(m_number_of_components);
    long double chain_length = static_cast<long double>(m_number_of_components - 2);
    long double sinConstant = sin(m_pi/chain_length);

    long double ratio = (1.0L - sinConstant)/(1.0L + sinConstant);

    long double inner_circle_radius = 1.0L;
    long double outer_circle_radius = 1.0L/ratio;
    long double chain_circle_radius = (outer_circle_radius - inner_circle_radius)*0.5L;
    LongComplex p0;
    LongComplex p1;
    long double r;
    // Inner circle
    r = inner_circle_radius;
    p0 = LongComplex(-r, 0.0L) - m_inversion_distance;
    p1 = LongComplex(r, 0.0L) - m_inversion_distance;
    // inversion radius omitted as its square is 1
    p0 = p0/norm(p0) + m_inversion_distance;
    p1 = p1/norm(p1) + m_inversion_distance;
    m_components[0].centre = (p0 + p1)/2.0L;
    m_components[0].radius = ld::abs(p0 - p1)/2.0L;
    // Outer circle
    r = outer_circle_radius;
    p0 = LongComplex(-r, 0.0L) - m_inversion_distance;
    p1 = LongComplex(r, 0.0L) - m_inversion_distance;
    // inversion radius omitted as its square is 1
    p0 = p0/norm(p0) + m_inversion_distance;
    p1 = p1/norm(p1) + m_inversion_distance;
    m_components[1].centre = (p0 + p1)/2.0L;
    m_components[1].radius = ld::abs(p0 - p1)/2.0L;
    //
    long double angle = 0.0L;
    long double delta_angle = (2.0L*m_pi)/chain_length;
    // c0 is the inner circle c1 is the outer circle
    bool still_inner = m_components[0].radius < m_components[1].radius;
    m_components[0].enabled = (still_inner ? m_inner_circle : m_outer_circle);
    m_components[1].enabled = (still_inner ? m_outer_circle : m_inner_circle);
    LongComplex c0 = (still_inner ? m_components[0].centre : m_components[1].centre);
    LongComplex c1 = (still_inner ? m_components[1].centre : m_components[0].centre);
    bool annular = (c0 == c1);
    for (unsigned int i = 2; i < m_number_of_components; i++)
    {
        long double distance_to_p0 = inner_circle_radius;
        long double distance_to_p1 = inner_circle_radius + 2.0L*chain_circle_radius;
        p0 = LongComplex(distance_to_p0*cos(angle), distance_to_p0*sin(angle)) - m_inversion_distance;
        p1 = LongComplex(distance_to_p1*cos(angle), distance_to_p1*sin(angle)) - m_inversion_distance;
        // inversion radius omitted as its square is 1
        p0 = p0/norm(p0) + m_inversion_distance;
        p1 = p1/norm(p1) + m_inversion_distance;
        // p0 and p1 are points where chain circle meets the inner and outer circles
        // Check for horizontal lines
        LongComplex &inner = (ld::abs(p0 - c0) < ld::abs(p1 - c0) ? p0 : p1);
        LongComplex &outer = (ld::abs(p0 - c0) > ld::abs(p1 - c0) ? p0 : p1);
        // The centre is where two lines meet
        // y = m0*x + c (inner circle centre through inner circle touch point)
        // y = m1*x + d (outer circle centre through outer circle touch point)
        //
        long double m0 = imag(inner - c0)/real(inner - c0);
        long double m1 = imag(outer - c1)/real(outer - c1);
        if ((m0 == m1) || annular)
        {
            // When both lines have the same gradient the inner and outer
            // circles are centred on the origin so p0 and p1 are the ends
            // of a line passing through the chain circles centre
            m_components[i].centre = (p0 + p1)/2.0L;
            m_components[i].radius = ld::abs(p0 - p1)/2.0L;
        }
        else
        {
            long double c = imag(inner) - m0*real(inner);
            long double d = imag(outer) - m1*real(outer);
            long double x = (d - c)/(m0 - m1);
            long double y = x*m0 + c;
            LongComplex centre = LongComplex(x, y);
            m_components[i].centre = centre;
            m_components[i].radius = ld::abs(inner - centre);
        }
        long double r = m_components[i].radius;
        LongComplex c = m_components[i].centre;
        m_components[i].enabled = true;
        m_components[i].p0 = LongComplex(r*cos(m_pi/6.0L), -r*sin(m_pi/6.0L));
        m_components[i].offset_rotation = m_reference_angle - ld::arg(inner - c);
        angle += delta_angle;
    }
    // Now scale to a width of 1 and centre at the origin
    long double scale_factor = 0.5/m_components[0].radius;
    LongComplex centre = m_components[0].centre;
    for (unsigned int i = 0; i < m_number_of_components; i++)
    {
        m_components[i].centre = (m_components[i].centre - centre)*scale_factor;
        m_components[i].radius *= scale_factor;
        m_components[i].p0 *= scale_factor;
    }
    scale();
}

void SteinerChain::SetSize(long double value)
{
    m_size = value;
}

LongComplex SteinerChain::relativeToComponent(int index, const LongComplex &value)
{
    long double r11 = cos(m_components[index].offset_rotation);
    long double r12 = sin(m_components[index].offset_rotation);
    long double r21 = -r12;
    long double r22 = r11;

    LongComplex z = value - m_components[index].centre;

    long double new_real = real(z)*r11 + imag(z)*r21;
    long double new_imag = real(z)*r12 + imag(z)*r22;
    return LongComplex(new_real, new_imag);
}

boost::shared_ptr<SteinerChain> SteinerChain::getTrap(bool inner)
{
    TrapMap &map = (inner ? s_inner_traps : s_outer_traps);
    auto it = map.find(STEINER_CHAIN);
    bool create (it == map.end());
    if (create) SteinerChain::create();
    map[STEINER_CHAIN] = SteinerChain::instance(inner);
    return SteinerChain::instance(inner);
}

void SteinerChain::setInnerCircle(bool value)
{
    m_inner_circle = value;
}

bool SteinerChain::getInnerCircle()
{
    return m_inner_circle;
}

void SteinerChain::setOuterCircle(bool value)
{
    m_outer_circle = value;
}

bool SteinerChain::getOuterCircle()
{
    return m_outer_circle;
}

void SteinerChain::setTriangles(bool value)
{
    m_triangles = value;
}

bool SteinerChain::getTriangles()
{
    return m_triangles;
}

void SteinerChain::setConstructionRotation(long double value)
{
    m_construction_rotation = value;
}

long double SteinerChain::getConstructionRotation()
{
    return m_construction_rotation;
}

void SteinerChain::setCircleCentrePoint(bool value)
{
    m_component_central_point = value;
}

bool SteinerChain::getCircleCentrePoint()
{
    return m_component_central_point;
}
