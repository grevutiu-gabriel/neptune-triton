//
// File:   StaticCalculatingUnit.h
// Author: M.R.Eggleston
//
// Created on 02 July 2012, 08:41
//

#pragma once

#ifdef NOT_MOC
#include <boost/thread/mutex.hpp>
#endif
#include <QPainter>
#include <QPixmap>
#include "mpcomplex.h"
#include "Colours.h"
#include "CriticalPointFinder.h"
#include "FractalConfig.h"
#include "OrbitTraps.h"
#include "TypesAndConstants.h"

class StaticCalculatingUnit
{
public:
    static void setFractal(const FractalConfig &config);
    static void setConfig(const FractalConfig &config); // only updates the configuration object
    static void stop();
    static bool finished();
    static const QRgb *getColours();
    
    static int nextRow();
    static void firstRow();
    static void initialiseResultsSet();
    static void initialiseResultsSet(ResultsSet& rs, long double z_r, long double z_i, bool zero_min = false);
    static void setC(MpComplex& c);
    static void setC(LongComplex& c);
    static void setDimensions(int width, int height);
    static void updateResults(ResultsSet& rs1, ResultsSet& rs2);
    static void processResults(ResultsSet& rs);
    static QRgb getColourForLocation(ResultsSet* rs);
    static void updateImage(ResultsSet* rs, int col, int row);
    static void updateImage();
    static void updateOrbitImage();
    static bool is_outer(ResultsSet* rs);
    static void setColourFunctions();
    static void setStatistics(bool inner, bool second, long double scale);
    static void setGaussian(bool inner, bool second, long double scale, long double angle_scale);
    static void setOrbitTrap(bool inner, bool second, long double scale);
    static void deltaResults(bool inner, bool second);
    static void numberOfResultsSetsRequired();
    
    static bool lt_value(long double value);
    static bool gt_value(long double value);
    static bool lt_abs_value(long double value);
    static bool gt_abs_value(long double value);

    static void assignTrap(bool inner, bool second);
    static QRgb GetColour(unsigned int r, unsigned int g, unsigned int b);
    static void allocateColours(unsigned int size);
    static void setOrbitColourMap();
    static void plot(VectorLongComplex& orbit, int plot_length, bool escaped);
    static bool plot(long double x, long double y);
    
    static void setRotationMatrix();
    static void rotate(long double a, long double b, long double& ar, long double& br);
    static void rotate(const MpReal& a, const MpReal& b, MpReal& ar, MpReal& br);
    static void updateMaxIteration(int iterations);
    static void paintImage(QPainter& painter);
    
//    static void determineCriticalPoints();
//    static LongComplex getCriticalPointAsLongComplex(int index);
//    static MpComplex getCriticalPointAsMpComplex(int index);

    static FRACTAL_TYPE s_type;
    static FractalConfig s_config;
    static VectorResultsSet s_results;
    static long double s_limit;
    static BAILOUT s_bailout_type;
    static int s_size;
    static int s_width;
    static int s_height;
    static int s_calculating_width;
    static int s_calculating_height;
    static int s_number_of_results_sets;
    enum RESULTS_SET
    {
        RS_ANGLE,
        RS_MAGNITUDE,
        RS_GAUSSIAN,
        RS_ORBIT_TRAP
    };
    static RESULTS_SET s_results_set[2];
    static bool s_change[2];
    static bool s_angle[2];
    static boost::mutex s_row_mutex;
    static int s_current_row;
    static bool s_stop;
    static int s_rotation;
    static MpReal s_topleft_r;
    static MpReal s_topleft_i;
    static MpReal s_pitch;
    static MpReal s_r11;
    static MpReal s_r21;
    static MpReal s_tx;
    static MpReal s_r12;
    static MpReal s_r22;
    static MpReal s_ty;
    static long double s_ld_topleft_r;
    static long double s_ld_topleft_i;
    static long double s_orbit_topleft_r;
    static long double s_orbit_topleft_i;
    static long double s_ld_r11;
    static long double s_ld_r21;
    static long double s_ld_tx;
    static long double s_ld_r12;
    static long double s_ld_r22;
    static long double s_ld_ty;
    static bool s_is_rotated;
    static bool s_is_biomorph_selection;
    static QImage s_image;
    static unsigned int s_n_channels;
    static unsigned int s_rowstride;
    static Colours s_colours[2];
    static std::string s_colouring_method[2];
    static char s_magnitude_type[2];
    static char s_quotient[2];
    static char s_divisor[2];
    static long double s_select_limit;
    enum GAUSSIAN_INTEGER_TYPE
    {
        GI_CEIL,
        GI_FLOOR,
        GI_ROUND,
        GI_TRUNC
    };
    
    static GAUSSIAN_INTEGER_TYPE s_gaussian_type[2];
    // Biomorph colour selection
    static char s_variable1;
    static char s_variable2;
    static char s_comparison1;
    static char s_comparison2;
    static char s_combination;
    static boost::shared_ptr<OrbitTrap> s_trap[2];
    static boost::mutex s_colour_mutex;
    // Orbit plotting picture data
    static ORBIT_COLOUR s_colouring_type;
    static VectorInt s_hits;
    static VectorColourAccumulator s_colour_totals;
    static VectorInt s_orbit_reds;
    static VectorInt s_orbit_greens;
    static VectorInt s_orbit_blues;
    static unsigned int s_number_of_orbit_reds;
    static unsigned int s_number_of_orbit_greens;
    static unsigned int s_number_of_orbit_blues;
    static int s_omit;
    static ORBIT_PLOTS s_orbit_plot_type;
    static long s_interval;
    static long s_colour_change_interval;
    static int s_red;
    static int s_green;
    static int s_blue;
    static long double s_min_x;
    static long double s_max_x;
    static long double s_min_y;
    static long double s_max_y;
    static long double s_ld_pitch;
    static boost::mutex s_plot_mutex;
    static bool s_average_colouring;
    static int s_settling_cycles;
    static boost::mutex s_max_iteration_mutex;
    static int s_max_iteration;
    static int s_precision;
    static bool s_multi_precision;
    static long double s_density_factor;
    static QRgb s_background;
    static bool s_has_background;
    //
    static VectorLongComplex s_critical_point;
    static unsigned int s_number_of_critical_points;
    static INNER_TYPE s_inner_type;
    static COMPOUND_IMAGE s_compound_image;
};

inline void StaticCalculatingUnit::setC(MpComplex& c)
{
    if (s_is_rotated)
    {
        MpReal new_c_r = real(c)*s_r11 + imag(c)*s_r21 + s_tx;
        MpReal new_c_i = real(c)*s_r12 + imag(c)*s_r22 + s_ty;
        c = MpComplex(new_c_r, new_c_i);
    }
}

inline void StaticCalculatingUnit::setC(LongComplex& c)
{
    if (s_is_rotated)
    {
        long double new_c_r = real(c)*s_ld_r11 + imag(c)*s_ld_r21 + s_ld_tx;
        long double new_c_i = real(c)*s_ld_r12 + imag(c)*s_ld_r22 + s_ld_ty;
        c = LongComplex(new_c_r, new_c_i);
    }
}

inline bool StaticCalculatingUnit::finished()
{
    bool finished = true;
    for (int i = 0; i < s_size; i++)
    {
        if (!s_results[i].escaped)
        {
            finished = false;
        }
    }
    return finished;
}

inline QRgb StaticCalculatingUnit::getColourForLocation(ResultsSet *rs)
{
    QRgb colour = qRgb(0, 0, 0);
    unsigned int& done = rs->escaped;
    if (s_inner_type == IT_OVERLAP)
    {
        if (done == 0)
        {
            // inner colouring
            colour = s_colours[1].getColour(rs);
        }
        else if (done == s_number_of_critical_points)
        {
            // outer colouring
            colour = s_colours[0].getColour(rs);
        }
        else if ((done > 0)  && (done < s_number_of_critical_points))
        {
            int not_done = static_cast<int>(s_number_of_critical_points) - done;
            auto colour1 = s_colours[0].getColour(rs);
            auto colour2 = s_colours[1].getColour(rs);
            int blue1 = qBlue(colour1)*done;
            int blue2 = qBlue(colour2)*not_done;
            int green1 = qGreen(colour1)*done;
            int green2 = qGreen(colour2)*not_done;
            int red1 = qRed(colour1)*done;
            int red2 = qRed(colour2)*not_done;
            colour = qRgb((red1 + red2)/(done + not_done),
                          (green1 + green2)/(done + not_done),
                          (blue1 + blue2)/(done + not_done));
        }
    }
    else if (s_inner_type == IT_ONE_OR_MORE)
    {
        // inner if any critical point is inner
        colour = s_colours[done < s_number_of_critical_points ? 1 : 0].getColour(rs);
    }
    else
    {
        // Inner only if all critical points are inner.
        colour = s_colours[done == 0 ? 1 : 0].getColour(rs);
    }
    return colour;
}

inline void StaticCalculatingUnit::updateImage(ResultsSet* rs, int col, int row)
{
    boost::mutex::scoped_lock lock(s_colour_mutex);
    s_image.setPixel(col, row, getColourForLocation(rs));
}

inline void StaticCalculatingUnit::updateImage()
{
    QRgb black = qRgb(0, 0, 0);
    boost::mutex::scoped_lock lock(s_colour_mutex);
    ResultsSet* rs = s_results.data();
    if (rs != NULL)
    {
        for (int row = 0; row < s_height; row++)
        {
            QRgb *pixel = reinterpret_cast<QRgb *>(s_image.scanLine(row));
            for (int col = 0; col < s_width; col++, rs++)
            {
                pixel[col] = (rs->done ? getColourForLocation(rs) : black);
            }
        }
    }
}

inline bool StaticCalculatingUnit::lt_value(long double value)
{
    return (value < s_select_limit);
}

inline bool StaticCalculatingUnit::gt_value(long double value)
{
    return (value > s_select_limit);
}

inline bool StaticCalculatingUnit::lt_abs_value(long double value)
{
    return (std::abs(value) < s_select_limit);
}

inline bool StaticCalculatingUnit::gt_abs_value(long double value)
{
    return (std::abs(value) > s_select_limit);
}

inline void StaticCalculatingUnit::plot(VectorLongComplex& orbit, int plot_length, bool escaped)
{
    boost::mutex::scoped_lock lock(s_colour_mutex);
    if ((((s_orbit_plot_type == OP_ALL) ||
         ((s_orbit_plot_type == OP_ESCAPED) && escaped) ||
         ((s_orbit_plot_type == OP_CAPTIVE) && !escaped))))
    {
        for (int orbit_index = 0; orbit_index < plot_length; orbit_index++)
        {
            long double x = real(orbit[orbit_index]);
            long double y = imag(orbit[orbit_index]);
            //
            // Need to reverse any image rotation here
            //
            if (s_average_colouring)
            {
                s_interval++;
                if (s_interval == s_colour_change_interval)
                {
                    s_interval = 0;
                    s_red = (s_red + 1) % s_number_of_orbit_reds;
                    s_green = (s_green + 1) % s_number_of_orbit_greens;
                    s_blue = (s_blue + 1) % s_number_of_orbit_blues;
                }
            }
            if ((x >= s_min_x) && (x < s_max_x) && (y >= s_min_y) && (y < s_max_y))
            {
                int i = static_cast<int>((x - s_min_x)/s_ld_pitch);
                int j = static_cast<int>((s_max_y - y)/s_ld_pitch);
                if ((i >= 0) && (i < s_width) && (j >= 0) && (j < s_height))
                {
                    int pos = i + j*s_width;
                    boost::mutex::scoped_lock lock(s_plot_mutex);
                    s_hits[pos]++;
                    if (s_average_colouring)
                    {
                        QRgb colour = GetColour(s_red, s_green, s_blue);
                        s_colour_totals[pos].red += qRed(colour);
                        s_colour_totals[pos].green += qGreen(colour);
                        s_colour_totals[pos].blue += qBlue(colour);
                    }
                }
            }
        }
    }
}

inline bool StaticCalculatingUnit::plot(long double x, long double y)
{
    bool visible = false;
    boost::mutex::scoped_lock lock(s_colour_mutex);
        //
        // Need to reverse any image rotation here ????
        //
    if (s_average_colouring)
    {
        s_interval++;
        if (s_interval == s_colour_change_interval)
        {
            s_interval = 0;
            s_red = (s_red + 1) % s_number_of_orbit_reds;
            s_green = (s_green + 1) % s_number_of_orbit_greens;
            s_blue = (s_blue + 1) % s_number_of_orbit_blues;
        }
    }
    if ((x >= s_min_x) && (x < s_max_x) && (y >= s_min_y) && (y < s_max_y))
    {
        int i = static_cast<int>((x - s_min_x)/s_ld_pitch);
        int j = static_cast<int>((s_max_y - y)/s_ld_pitch);
        if ((i >= 0) && (i < s_width) && (j >= 0) && (j < s_height))
        {
            visible = true;
            int pos = i + j*s_width;
            boost::mutex::scoped_lock lock(s_plot_mutex);
            s_hits[pos]++;
            if (s_average_colouring)
            {
                QRgb colour = GetColour(s_red, s_green, s_blue);
                s_colour_totals[pos].red += qRed(colour);
                s_colour_totals[pos].green += qGreen(colour);
                s_colour_totals[pos].blue += qBlue(colour);
            }
        }
    }
    return visible;
}

inline QRgb StaticCalculatingUnit::GetColour(unsigned int r, unsigned int g, unsigned int b)
{
    return qRgb(s_orbit_reds[r % s_number_of_orbit_reds],
                s_orbit_greens[g % s_number_of_orbit_greens],
                s_orbit_blues[b % s_number_of_orbit_blues]);
}

inline void StaticCalculatingUnit::updateMaxIteration(int iteration)
{
    boost::mutex::scoped_lock lock(s_max_iteration_mutex);
    if (iteration > s_max_iteration)
    {
        s_max_iteration = iteration;
    }
}

inline void StaticCalculatingUnit::rotate(long double a, long double b, long double&ar, long double&br)
{
    if (s_is_rotated)
    {
        ar = a*s_ld_r11 + b*s_ld_r21 + s_ld_tx;
        br = a*s_ld_r12 + b*s_ld_r22 + s_ld_ty;
    }
    else
    {
        ar = a;
        br = b;
    }
}

inline void StaticCalculatingUnit::rotate(const MpReal& a, const MpReal& b, MpReal& ar, MpReal& br)
{
    if (s_is_rotated)
    {
        ar = a*s_r11 + b*s_r21 + s_tx;
        br = a*s_r12 + b*s_r22 + s_ty;
    }
    else
    {
        ar = a;
        br = b;
    }
}

inline void StaticCalculatingUnit::updateResults(ResultsSet& rs1, ResultsSet& rs2)
{
    // needs to take into account a single set used for both inner and outer
    rs1.escaped += rs2.escaped;
    int i = (rs2.escaped ? 0 : 1); // If done 'outer' hence 0
    bool two_sets = (s_number_of_results_sets == 2);
    if ((s_compound_image == CI_AVERAGE) || i == 1)
    {
        rs1.set[i].escape += rs2.set[0].escape; 
        rs1.set[i].z_r += rs2.set[0].z_r;
        rs1.set[i].z_i += rs2.set[0].z_i;
        rs1.set[i].min_dist += rs2.set[two_sets ? i : 0].min_dist;
        rs1.set[i].max_dist += rs2.set[two_sets ? i : 0].max_dist;
        rs1.set[i].minimum += rs2.set[two_sets ? i : 0].minimum;
        rs1.set[i].maximum += rs2.set[two_sets ? i : 0].maximum;
        rs1.set[i].angle_total += rs2.set[two_sets ? i : 0].angle_total;
        rs1.set[i].square_total += rs2.set[two_sets ? i : 0].square_total;
        rs1.set[i].total += rs2.set[two_sets ? i : 0].total;
        rs1.set[i].iteration_at_min += rs2.set[two_sets ? i : 0].iteration_at_min;
        rs1.set[i].iteration_at_max += rs2.set[two_sets ? i : 0].iteration_at_max;
        rs1.set[i].es_sum1 += rs2.set[two_sets ? i : 0].es_sum1;
        rs1.set[i].es_sum2 += rs2.set[two_sets ? i : 0].es_sum2;
        rs1.set[i].last += rs2.set[two_sets ? i : 0].last;     // Last value as a magnitude or an angle, first multiple data
        rs1.set[i].es2_last += rs2.set[two_sets ? i : 0].es2_last;
    }
    else if (rs2.set[0].escape > rs1.set[0].escape)
    {
        rs1.set[0].escape = rs2.set[0].escape; 
        rs1.set[0].z_r = rs2.set[0].z_r;
        rs1.set[0].z_i = rs2.set[0].z_i;
        rs1.set[0].escape = rs2.set[0].escape;
        rs1.set[0].min_dist = rs2.set[0].min_dist;
        rs1.set[0].max_dist = rs2.set[0].max_dist;
        rs1.set[0].minimum = rs2.set[0].minimum;
        rs1.set[0].maximum = rs2.set[0].maximum;
        rs1.set[0].angle_total = rs2.set[0].angle_total;
        rs1.set[0].square_total = rs2.set[0].square_total;
        rs1.set[0].total = rs2.set[0].total;
        rs1.set[0].iteration_at_min = rs2.set[0].iteration_at_min;
        rs1.set[0].iteration_at_max = rs2.set[0].iteration_at_max;
        rs1.set[0].es_sum1 = rs2.set[0].es_sum1;
        rs1.set[0].es_sum2 = rs2.set[0].es_sum2;
        rs1.set[0].last = rs2.set[0].last;
        rs1.set[0].es2_last = rs2.set[0].es2_last;
    }
}

inline void StaticCalculatingUnit::processResults(ResultsSet& rs)
{
    rs.done = true;
    int start = (s_compound_image == CI_AVERAGE ? 0 : 1);
    for (unsigned int i = start; i < 2; i++)
    {
        unsigned int overlaps = (i == 0 ? rs.escaped : s_number_of_critical_points - rs.escaped);
        if (overlaps != 0)
        {
            auto ld_overlaps = static_cast<long double>(overlaps);
//            rs.set[i].escape /= ld_overlaps; 
            rs.set[i].z_r /= ld_overlaps; 
            rs.set[i].z_i /= ld_overlaps;
            rs.set[i].min_dist /= ld_overlaps;
            rs.set[i].max_dist /= ld_overlaps;
            rs.set[i].minimum /= ld_overlaps;
            rs.set[i].maximum /= ld_overlaps;
//            rs.set[i].angle_total /= ld_overlaps;
//            rs.set[i].square_total /= ld_overlaps;
//            rs.set[i].total /= ld_overlaps;
            rs.set[i].iteration_at_min /= ld_overlaps;
            rs.set[i].iteration_at_max /= ld_overlaps;
//            rs.set[i].es_sum1 /= ld_overlaps;
//            rs.set[i].es_sum2 /= ld_overlaps;
            rs.set[i].last /= ld_overlaps;
            rs.set[i].es2_last/= ld_overlaps;
        }
    }
}
