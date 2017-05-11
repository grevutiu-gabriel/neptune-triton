// 
// File:   StaticCalculatingUnit.cc
// Author: M.R.Eggleston
// 
// Created on 02 July 2012, 08:44
//

#include <limits>
#include "KeyString.h"
#include "StaticCalculatingUnit.h"
#include "Utilities.h"

// Static data common to all calculating units
FRACTAL_TYPE StaticCalculatingUnit::s_type;
FractalConfig StaticCalculatingUnit::s_config;
VectorResultsSet StaticCalculatingUnit::s_results;
long double StaticCalculatingUnit::s_limit;
BAILOUT StaticCalculatingUnit::s_bailout_type;
int StaticCalculatingUnit::s_width = 0;
int StaticCalculatingUnit::s_height = 0;
int StaticCalculatingUnit::s_calculating_width = 0;
int StaticCalculatingUnit::s_calculating_height = 0;
int StaticCalculatingUnit::s_size = 0;
int StaticCalculatingUnit::s_number_of_results_sets;
StaticCalculatingUnit::RESULTS_SET StaticCalculatingUnit::s_results_set[2];
bool StaticCalculatingUnit::s_change[2];
bool StaticCalculatingUnit::s_angle[2];
boost::mutex StaticCalculatingUnit::s_row_mutex;
int StaticCalculatingUnit::s_current_row;
//boost::mutex StaticCalculatingUnit::s_stop_mutex;
bool StaticCalculatingUnit::s_stop = false;
MpReal StaticCalculatingUnit::s_topleft_r;
MpReal StaticCalculatingUnit::s_topleft_i;
MpReal StaticCalculatingUnit::s_pitch;
MpReal StaticCalculatingUnit::s_r11;
MpReal StaticCalculatingUnit::s_r21;
MpReal StaticCalculatingUnit::s_tx;
MpReal StaticCalculatingUnit::s_r12;
MpReal StaticCalculatingUnit::s_r22;
MpReal StaticCalculatingUnit::s_ty;
long double StaticCalculatingUnit::s_orbit_topleft_r;
long double StaticCalculatingUnit::s_orbit_topleft_i;
long double StaticCalculatingUnit::s_ld_topleft_r;
long double StaticCalculatingUnit::s_ld_topleft_i;
long double StaticCalculatingUnit::s_ld_r11;
long double StaticCalculatingUnit::s_ld_r21;
long double StaticCalculatingUnit::s_ld_tx;
long double StaticCalculatingUnit::s_ld_r12;
long double StaticCalculatingUnit::s_ld_r22;
long double StaticCalculatingUnit::s_ld_ty;
bool StaticCalculatingUnit::s_is_rotated;
QImage StaticCalculatingUnit::s_image;
Colours StaticCalculatingUnit::s_colours[2];
std::string StaticCalculatingUnit::s_colouring_method[2];
char StaticCalculatingUnit::s_magnitude_type[2];
char StaticCalculatingUnit::s_quotient[2];
char StaticCalculatingUnit::s_divisor[2];
StaticCalculatingUnit::GAUSSIAN_INTEGER_TYPE StaticCalculatingUnit::s_gaussian_type[2];
boost::shared_ptr<OrbitTrap> StaticCalculatingUnit::s_trap[2];
boost::mutex StaticCalculatingUnit::s_colour_mutex;
VectorInt StaticCalculatingUnit::s_hits;
VectorColourAccumulator StaticCalculatingUnit::s_colour_totals;
ORBIT_COLOUR StaticCalculatingUnit::s_colouring_type = OC_ACCUMULATION;
VectorInt StaticCalculatingUnit::s_orbit_reds;
VectorInt StaticCalculatingUnit::s_orbit_greens;
VectorInt StaticCalculatingUnit::s_orbit_blues;
unsigned int StaticCalculatingUnit::s_number_of_orbit_reds = 0;
unsigned int StaticCalculatingUnit::s_number_of_orbit_greens = 0;
unsigned int StaticCalculatingUnit::s_number_of_orbit_blues = 0;
int StaticCalculatingUnit::s_omit = 0;
ORBIT_PLOTS StaticCalculatingUnit::s_orbit_plot_type = OP_ALL;
long StaticCalculatingUnit::s_interval = 0L;
int StaticCalculatingUnit::s_red = 0;
int StaticCalculatingUnit::s_green = 0;
int StaticCalculatingUnit::s_blue = 0;
long StaticCalculatingUnit::s_colour_change_interval = 10000;
long double StaticCalculatingUnit::s_ld_pitch = 0.0L;
long double StaticCalculatingUnit::s_min_x = 0.0L;
long double StaticCalculatingUnit::s_max_x = 0.0L;
long double StaticCalculatingUnit::s_min_y = 0.0L;
long double StaticCalculatingUnit::s_max_y = 0.0L;
bool StaticCalculatingUnit::s_average_colouring = false;
boost::mutex StaticCalculatingUnit::s_plot_mutex;
int StaticCalculatingUnit::s_rotation;
boost::mutex StaticCalculatingUnit::s_max_iteration_mutex;
int StaticCalculatingUnit::s_max_iteration;
int StaticCalculatingUnit::s_precision = BASE_PRECISION;
bool StaticCalculatingUnit::s_multi_precision = false;
long double StaticCalculatingUnit::s_density_factor = 1.0L;
QRgb StaticCalculatingUnit::s_background = qRgb(0, 0, 0);
bool StaticCalculatingUnit::s_has_background;
VectorLongComplex StaticCalculatingUnit::s_critical_point;
unsigned int StaticCalculatingUnit::s_number_of_critical_points;
INNER_TYPE StaticCalculatingUnit::s_inner_type = IT_OVERLAP;
COMPOUND_IMAGE StaticCalculatingUnit::s_compound_image = CI_AVERAGE;


//==============================================================================
// Static methods common to all calculating units
//==============================================================================

void StaticCalculatingUnit::setFractal(const FractalConfig &config)
{
    s_config = config;
    s_bailout_type = s_config.getBailout();
    s_limit = s_config.getLimit();
    // Set transform sequence
    s_type = s_config.GetFractalType();
    firstRow();
    int w = s_config.getWidth();
    int h = s_config.getHeight();
    auto c = s_config.getCentre();
    s_precision = s_config.getPrecision();
    if (s_precision == 0)
    {
        s_precision = determinePrecision(c, s_config.getImageWidth(), w, h);
    }
    s_multi_precision = (s_precision > BASE_PRECISION);
    setDimensions(w, h);
    s_rotation = s_config.getRotation();
    s_is_rotated = (s_rotation != 0);
    setRotationMatrix();
    MpReal c_r(c.r, s_precision);
    MpReal c_i(c.i, s_precision);
    MpReal width(s_config.getWidth(), s_precision);
    MpReal height(s_config.getHeight(), s_precision);
    MpReal image_width(s_config.getImageWidth(), s_precision);
    s_pitch = image_width/width;
    s_density_factor = sqrt(static_cast<long double>(s_config.getDensity()));
    MpReal two("2", s_precision);
    s_topleft_r = c_r - (s_pitch*width)/two;
    s_topleft_i = c_i + (s_pitch*height)/two;
    s_ld_topleft_r = s_topleft_r.toLDouble();
    s_ld_topleft_i = s_topleft_i.toLDouble();
    if (s_config.isOrbitPlotting())
    {
        //
        // Orbit fractals have a larger calculating area than that displayed
        //
        s_ld_pitch = s_pitch.toLDouble();
        long double factor = std::sqrt(static_cast<long double>(s_config.getArea()));
        s_calculating_width = static_cast<int>(w*factor);
        s_calculating_height = static_cast<int>(h*factor);
        long double calculation_width = factor*width.toLDouble(); // in pixels
        long double calc_width = calculation_width*s_ld_pitch;
        s_orbit_topleft_r = c_r.toLDouble() - calc_width/2.0L;
        s_orbit_topleft_i = c_i.toLDouble() + (calc_width*h)/(2.0L*w);
        // Determine display bounds
        s_min_x = s_topleft_r.toLDouble() - s_ld_pitch/2.0L;
        s_max_x = s_min_x + s_ld_pitch*width.toLDouble() + s_ld_pitch/2.0L;
        s_max_y = s_topleft_i.toLDouble() + s_ld_pitch/2.0L;
        s_min_y = s_max_y - s_ld_pitch*height.toLDouble() - s_ld_pitch/2.0L;
        s_interval = 0;
        s_red = 0;
        s_green = 0;
        s_blue = 0;
        // due to change
        setOrbitColourMap();
        s_omit = s_config.getStartOrbitIteration();
        s_orbit_plot_type = s_config.getOrbitPlot();
        s_colouring_type = OC_ACCUMULATION;
        char ch = s_config.getOrbitColourType();
        if ((ch >= 'a') && (ch <= 'd'))
        {
            s_colouring_type = static_cast<ORBIT_COLOUR>(ch - 'a');
        }
        s_average_colouring = (s_colouring_type == OC_AVERAGE);
        if (s_average_colouring)
        {
            s_colour_change_interval = s_config.getOrbitColourInterval();
            s_colour_change_interval *= s_config.getArea();
        }
    }

    s_image = QImage(s_width, s_height, QImage::Format_ARGB32);
    s_image.fill(Qt::black);

    setColourFunctions();

    s_compound_image = s_config.compoundImage();
    s_inner_type = s_config.innerType();
}

void StaticCalculatingUnit::setConfig(const FractalConfig &config)
{
    s_config = config;
}

void StaticCalculatingUnit::stop()
{
    s_stop = true;
}

void StaticCalculatingUnit::initialiseResultsSet()
{
    int size = s_height*s_width;
    for (int i = 0; i < size; i++)
    {
        s_results[i].done = false;
    }
}

void StaticCalculatingUnit::initialiseResultsSet(ResultsSet& rs, long double z_r, long double z_i, bool zero_min)
{
    static const long double max = std::numeric_limits<long double>::max();
    static const LongComplex cmax(max, max);
    static const LongComplex czero(0.0L, 0.0L);
    static const long double pi = 2.0L*acos(0.0L);
    rs.escaped = 0;
    LongComplex z(z_r, z_i);
    // Gaussian Integer data
    for (int i = 0; i < 2; i++)
    {
        Statistics& s = rs.set[i];
        s.escape = 0L;
        s.z_r = 0.0L;
        s.z_i = 0.0L;
        s.last = (s_angle[i] ? arg(z) : ld::abs(z));
        // only angle can be -ve here, adjust if necessary
        s.last = (s.last < 0.0L ? s.last + 2.0L*pi : s.last);
        s.es2_last = 0.0L;
        s.min_dist = (zero_min ? LongComplex() : cmax);
        s.max_dist = czero;
        s.minimum = (zero_min ? 0.0L : max);
        s.maximum = 0.0L;
        s.angle_total = 0.0L;
        s.square_total = 0.0L;
        s.total = 0.0L;
        s.iteration_at_min = 0;
        s.iteration_at_max = 0;
        s.es_sum1 = 0.0L;
        s.es_sum2 = 0.0L;
    }
}

void StaticCalculatingUnit::setColourFunctions()
{
    boost::mutex::scoped_lock lock(s_colour_mutex);
    if (s_config.isOrbitPlotting())
    {
        s_colouring_type = OC_ACCUMULATION;
        char ch = s_config.getOrbitColourType();
        if ((ch >= 'a') && (ch <= 'd'))
        {
            s_colouring_type = static_cast<ORBIT_COLOUR>(ch - 'a');
        }
        setOrbitColourMap();
        s_background = s_config.getOrbitBackground().rgba();
        s_has_background = s_config.hasOrbitBackground();
    }
    else
    {
        long double pi = mpfr::const_pi(128).toLDouble();
        // Set colouring functions
        s_colouring_method[false] = s_config.getMethod(false);
        s_colouring_method[true] = s_config.getMethod(true);
        numberOfResultsSetsRequired();
        bool two_sets = (s_number_of_results_sets == 2);
        for (int i = 0; i <= 1; i++)
        {
            bool inner = (i == 1);
            auto method = s_config.getMethod(inner);
            char type = method[0];
            Colours &object = s_colours[inner];
            if (type != 'f')
            {
                COMPONENT_ORDER order = s_config.getComponentOrder(inner);
                int r = s_config.getRedOffset(inner);
                int g = s_config.getGreenOffset(inner);
                int b = s_config.getBlueOffset(inner);
                object.setOffsets(r, g, b);
                auto map = s_config.getColourMap(inner);
                object.setColours(order, map);
                object.setSmooth(s_config.isSmooth(inner));
                object.setAbsLog(s_config.isAbsLog(inner));
                long double scale = (s_config.getScale(inner));
                long double angle_scale = scale/(2.0L*pi);
                long double magnitude_scale = scale/2.0L;
                if (type == 'a')
                {
                    object.setFinalAngle(angle_scale, inner);
                }
                else if ((type == 'A') || (type == 'M') || (type == 'r'))
                {
                    setStatistics(inner, (inner ? two_sets : false), (type == 'A' ? angle_scale : magnitude_scale));
                }
                else if (type == 'm')
                {
                    object.setFinalMagnitude(method[1], magnitude_scale, inner);
                }
                else if (type == 'g')
                {
                    setGaussian(inner, (inner ? two_sets : false), magnitude_scale, angle_scale);
                }
                else if (type == 't')
                {
                    setOrbitTrap(inner, inner, magnitude_scale);
                }
                else
                {
                    object.setIteration(s_config.getLimit());
                }
            }
            else
            {
                object.setFixed(stringToColour(method.substr(2)).rgb());
            }
        }
    }
}

void StaticCalculatingUnit::numberOfResultsSetsRequired()
{
    s_number_of_results_sets = 0;
    // Two sets of multiple data are required if inner and outer are
    // of types that save different results data.
    //
    // Differing Gaussian Integer
    bool two_multiple = false;
    bool same_type = s_colouring_method[false][0] == s_colouring_method[true][0];
    std::string& inner = s_colouring_method[true];
    std::string& outer = s_colouring_method[false];
    bool inner_multiple = (inner[0] == 'g') || (inner[0] == 'M') ||
                          (inner[0] == 'A') || (inner[0] == 't') ||
                          (inner[0] == 'r');
    bool outer_multiple = (outer[0] == 'g') || (outer[0] == 'M') ||
                          (outer[0] == 'A') || (outer[0] == 't') ||
                          (outer[0] == 'r');
    if (inner_multiple || outer_multiple)
    {
        if (same_type)
        {
            s_number_of_results_sets = 1;
            if (outer[0] == 'g')
            {
                two_multiple = (outer[1] != inner[1]);
            }
            else if ((outer[0] == 'A') || (outer[0] == 'M'))
            {
                two_multiple = ((outer[1] < 'a') != (inner[1] < 'a'));
            }
            else if (outer[0] == 't')
            {
                two_multiple = (outer != inner);
            }
            s_number_of_results_sets += static_cast<int>(two_multiple);
        }
        else
        {
            s_number_of_results_sets += static_cast<int>(outer_multiple);
            s_number_of_results_sets += static_cast<int>(inner_multiple);
        }
    }
}

void StaticCalculatingUnit::setStatistics(bool inner, bool second, long double scale)
{
    Colours &obj = s_colours[inner];
    std::string& method = s_colouring_method[inner];
    s_magnitude_type[second] = method[0];
    if (method[0] == 'r')
    {
        s_quotient[second] = method[1];
        s_divisor[second] = method[2];
    }
    char statistic = (method[0] == 'r' ? method[3] : method[1]);
    obj.setStatistics(statistic, scale, inner);
    s_results_set[second] = (method[0] == 'A' ? RS_ANGLE : RS_MAGNITUDE);
    deltaResults(inner, second);
}

void StaticCalculatingUnit::setGaussian(bool inner, bool second, long double scale, long double angle_scale)
{
    Colours& obj = s_colours[inner];
    std::string& method = s_colouring_method[inner];
    obj.setGaussian(method[2], scale, angle_scale, inner);
    switch (method[1])
    {
    case 'c':
        s_gaussian_type[second] = GI_CEIL;
        break;
    case 'f':
        s_gaussian_type[second] = GI_FLOOR;
        break;
    default:
    case 'r':
        s_gaussian_type[second] = GI_ROUND;
        break;
    case 't':
        s_gaussian_type[second] = GI_TRUNC;
        break;
    }       
    s_results_set[second] = RS_GAUSSIAN;
}

void StaticCalculatingUnit::setOrbitTrap(bool inner, bool second, long double scale)
{
    Colours &obj = s_colours[inner];
    std::string& method = s_colouring_method[inner];
    assignTrap(inner, second);
    obj.setStatistics(method[2], scale, inner);
    s_results_set[second] = RS_ORBIT_TRAP;
    deltaResults(inner, second);
}

void StaticCalculatingUnit::deltaResults(bool inner, bool second)
{
    std::string& method = s_colouring_method[inner];
    s_angle[second] = (method[0] == 'A');
    s_change[second] = (((method[0] == 'M') && (method[1] < 'a')) ||
                       ((method[0] == 't') && (method[2] < 'a')) ||
                       ((method[0] == 'r') && (method[3] < 'a'))) ||
                       ((method[0] == 'A') && (method[1] < 'a'));
}

void StaticCalculatingUnit::assignTrap(bool inner, bool second)
{
    KeyString keys(s_colouring_method[inner].substr(3));
    OrbitTrap::TRAP_TYPE type = static_cast<OrbitTrap::TRAP_TYPE>(s_colouring_method[inner][1] - 'a');
    s_trap[second] = OrbitTrap::getTrap(type, second);
    long double centre_r = stringToLongDouble(keys.GetKey("r", "0.0"));
    long double centre_i = stringToLongDouble(keys.GetKey("i", "0.0"));
    s_trap[second]->SetPosition(LongComplex(centre_r, centre_i));
    s_trap[second]->EnableCentralPoint(stringToInt(keys.GetKey("cp", "0")) == 1);
    s_trap[second]->SetRotation(stringToUint(keys.GetKey("rot", "0")));
    s_trap[second]->SetSize(stringToLongDouble(keys.GetKey("width", "0.0")));
    if (type == OrbitTrap::STEINER_CHAIN)
    {
        auto trap = SteinerChain::instance(second);
        trap->setChainLength(stringToUint(keys.GetKey("n", "5")));
        trap->setInnerCircle(stringToInt(keys.GetKey("inner", "1")) != 0);
        trap->setOuterCircle(stringToInt(keys.GetKey("outer", "1")) != 0);
        trap->setInversionDistance(stringToLongDouble(keys.GetKey("invd", "0.0")));
        trap->setTriangles(stringToInt(keys.GetKey("triangles", "0")) != 0);
        trap->setCircleCentrePoint(stringToInt(keys.GetKey("cccp", "0")) != 0);
        trap->calculateCircles();
    }
}

void StaticCalculatingUnit::setDimensions(int width, int height)
{
    s_width = width;
    s_height = height;
    auto c = s_config.getImageCentre();
    auto w = s_config.getImageWidth();
    int size = width*height;
    if (s_config.isOrbitPlotting())
    {
        s_results.clear();
        s_hits.resize(size);
        s_colour_totals.resize(size);
        for (int i = 0; i < size; i++)
        {
            s_hits[i] = 0;
            s_colour_totals[i].red = 0;
            s_colour_totals[i].green = 0;
            s_colour_totals[i].blue = 0;
        };
        long double factor = std::sqrt(static_cast<long double>(s_config.getArea()));
        s_calculating_width = static_cast<int>(s_width*factor);
        s_calculating_height = static_cast<int>(s_height*factor);
    }
    else
    {
#ifndef BATCH
        s_results.resize(size);
#endif
    }
    s_size = size;
}

void StaticCalculatingUnit::updateOrbitImage()
{
    int* hits = s_hits.data();
    ColourAccumulator* total = s_colour_totals.data();
    long double scale = static_cast<long double>(s_config.getOrbitScale());
    QRgb colour;
    for (int row = 0; row < s_height; row++)
    {
        for (int column = 0; column < s_width; column++, hits++)
        {
            colour = qRgb(0, 0, 0);
            if (s_colouring_type == OC_AVERAGE)
            {
                if (*hits != 0)
                {
                    colour = qRgb(total->red/(*hits), total->green/(*hits), total->blue/(*hits));
                }
                else if (s_has_background)
                {
                    colour = s_background;
                }
                total++;
            }
            else if (s_has_background && (*hits == 0))
            {
                colour = s_background;
            }
            else if ((s_colouring_type == OC_ACCUMULATION) && (*hits > 0))
            {
                int value = *hits - 1;
                colour = GetColour(value, value, value);
            }
            else if ((s_colouring_type == OC_SQRT) || (s_colouring_type == OC_LOG))
            {
                double value = static_cast<double>(s_colouring_type == OC_LOG ? *hits + 1 : *hits);
                value = (s_colouring_type == OC_SQRT ? std::sqrt(value) : std::log(value))*scale;
                int index1 = static_cast<int>(ceil(value)); // Index of colour above
                int index2 = static_cast<int>(trunc(value)); // Index of colour below
                long double weighting = value - static_cast<double>(index2);
                auto colour1 = GetColour(index1, index1, index1);
                auto colour2 = GetColour(index2, index2, index2);
                colour = qRgb(static_cast<int>((static_cast<long double>(qRed(colour1))*weighting + static_cast<long double>(qRed(colour2))*(1.0L - weighting))/2.0L),
                              static_cast<int>((static_cast<long double>(qGreen(colour1))*weighting + static_cast<long double>(qGreen(colour2))*(1.0L - weighting))/2.0L),
                              static_cast<int>((static_cast<long double>(qBlue(colour1))*weighting + static_cast<long double>(qBlue(colour2))*(1.0L - weighting))/2.0L));
            }
            s_image.setPixel(column, row, colour);
        }
    }
}

void StaticCalculatingUnit::setOrbitColourMap()
{
    VectorInt reds;
    VectorInt greens;
    VectorInt blues;
    COMPONENT_ORDER order = s_config.getComponentOrder(false);
    // Output vectors always have at least one element
    s_config.getColourMap(false).getColours(order, reds, greens, blues);
    unsigned int r_size = reds.size();
    unsigned int g_size = greens.size();
    unsigned int b_size = blues.size();
    s_number_of_orbit_reds = r_size;
    s_number_of_orbit_greens = g_size;
    s_number_of_orbit_blues = b_size;
    s_orbit_reds.resize(s_number_of_orbit_reds);
    s_orbit_greens.resize(s_number_of_orbit_greens);
    s_orbit_blues.resize(s_number_of_orbit_blues);
    unsigned int red_offset = s_config.getOrbitRedOffset();
    unsigned int green_offset = s_config.getOrbitGreenOffset();
    unsigned int blue_offset = s_config.getOrbitBlueOffset();
    for (unsigned int i = 0; i < r_size; i++)
    {
        s_orbit_reds[i] = reds[(i + red_offset) % r_size];
    }
    for (unsigned int i = 0; i < g_size; i++)
    {
        s_orbit_greens[i] = greens[(i + green_offset) % g_size];
    }
    for (unsigned int i = 0; i < b_size; i++)
    {
        s_orbit_blues[i] = blues[(i + blue_offset) % b_size];
    }
}

void StaticCalculatingUnit::setRotationMatrix()
{
    MpReal zero("0", s_precision);
    MpReal one("1", s_precision);
    MpReal two("2", s_precision);
    MpReal threeSixty("360", s_precision);
    MpReal rot(s_rotation, s_precision);
    MpReal rotation = (two*mpfr::const_pi(s_precision)*rot)/threeSixty;
    if (s_is_rotated)
    {
        MpReal costheta = mpfr::cos(rotation);
        MpReal sintheta = mpfr::sin(rotation);
        s_r11 = costheta;
        s_r12 = sintheta;
        s_r21 = -sintheta;
        s_r22 = costheta;
        auto c = s_config.getCentre();
        MpReal c_r(c.r, s_precision);
        MpReal c_i(c.i, s_precision);
        s_tx = c_r*(one - costheta) + c_i*sintheta;
        s_ty = c_i*(one - costheta) - c_r*sintheta;
        //
        s_ld_r11 = s_r11.toLDouble();
        s_ld_r12 = s_r12.toLDouble();
        s_ld_r21 = s_r21.toLDouble();
        s_ld_r22 = s_r22.toLDouble();
        s_ld_tx = s_tx.toLDouble();
        s_ld_ty = s_ty.toLDouble();
    }
}

int StaticCalculatingUnit::nextRow()
{
    boost::mutex::scoped_lock lock(s_row_mutex);
    s_current_row++;
    return s_current_row;
}

void StaticCalculatingUnit::firstRow()
{
    boost::mutex::scoped_lock lock(s_row_mutex);
    s_current_row = -1; // ensures that nextRow returns zero the first time it is called.
}

void StaticCalculatingUnit::paintImage(QPainter& painter)
{
    boost::mutex::scoped_lock lock(s_colour_mutex);
    painter.drawPixmap(0, 0, QPixmap::fromImage(s_image));
}

