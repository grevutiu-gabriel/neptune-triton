// 
// File:   Utilities.cc
// Author: M.R.Eggleston
//
// Created on 29 January 2014, 14:10
//

#include <boost/lexical_cast.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <QDir>
#include "KeyString.h"
#include "Utilities.h"

std::string intToString(int value)
{
    return boost::lexical_cast<std::string>(value);
}

std::string intToString(bool value)
{
    return boost::lexical_cast<std::string>(value);
}

std::string uintToString(unsigned int value)
{
    return boost::lexical_cast<std::string>(value);
}

std::string ulongToString(unsigned long value)
{
    return boost::lexical_cast<std::string>(value);
}

std::string longdoubleToString(long double value)
{
    std::stringstream str;
    str << std::setprecision(std::numeric_limits<long double>::digits10) << value;
    return std::string(str.str());
}

QString longdoubleToQString(long double value)
{
    return QString::fromStdString(longdoubleToString(value));
}

std::string longdoubleToString(long double value, bool fixed, int precision)
{
    std::stringstream str;
    if (fixed)
    {
        str << std::fixed;
    }
    str << std::setprecision(precision) << value;
    return str.str();
}

StringComplex longComplexToStringComplex(const LongComplex& value)
{
    return StringComplex(longdoubleToString(value.real()),longdoubleToString(value.imag()));
}

QString longComplexToString(const LongComplex &z)
{
    bool neg_i = (z.imag() < 0.0L);
    QString str = longdoubleToQString(z.real()) + (neg_i ? " - " : " + ");
    str += longdoubleToQString(std::abs(z.imag())) + "i";
    return str;
}

QString complexNumberToString(const StringComplex &z)
{
    bool neg_i = (z.i[0] == '-');
    QString str = QString::fromStdString(z.r) + (neg_i ? " - " : " + ");
    str += QString::fromStdString((neg_i ? z.i.substr(1) : z.i)) + "i";
    return str;
}

QColor stringToColour(const std::string& str)
{
    QColor c;
    try
    {
        std::istringstream input(str);
        int value;
        char junk;
        input >> value;
        int red = value;
        input >> junk;
        input >> value;
        int green = value;
        input >> junk;
        input >> value;
        int blue = value;
        c = QColor(red, green, blue);
    }
    catch (std::exception&)
    {
        c = QColor(0, 0, 0);
    }
    return c;
}

unsigned int stringToUint(const std::string& str)
{
    unsigned int value = 0;
    try
    {
        value = boost::lexical_cast<unsigned int>(str);
    }
    catch (...)
    {
        // ignore
    }
    return value;
}

int stringToInt(const std::string& str)
{
    int value = 0;
    try
    {
        value = boost::lexical_cast<int>(str);
    }
    catch (...)
    {
        // ignore
    }
    return value;
}

long double stringToLongDouble(const std::string& str)
{
    long double value = 0.0L;
    try
    {
        value = boost::lexical_cast<long double>(str);
    }
    catch (...)
    {
        // ignore
    }
    return value;
}

LongComplex stdStringToLongComplex(const std::string& s1, const std::string& s2)
{
    long double r = stringToLongDouble(s1);
    long double i = stringToLongDouble(s2);
    return LongComplex(r, i);
}

LongComplex stringComplexToLongComplex(const StringComplex& z)
{
    return LongComplex(stringToLongDouble(z.r), stringToLongDouble(z.i));
}

MpComplex stringComplexToMpComplex(const StringComplex &z, int precision)
{
    return MpComplex(MpReal(z.r, precision), MpReal(z.i, precision));
}

long double trunc(long double value)
{
    // not std::trunc as it is not supported by MS C++
    return (value < 0.0L ? ceil(value) : floor(value));
}

int determinePrecision(const StringComplex& c, const std::string& w, int hpixels, int vpixels)
{
    // c is the position of the centre of the image and width is added to it
    // a point horizontally relative to it. The number of pixels from x to y
    // should be the same as the number of pixels specified. The height is
    // determined and the number of pixels is a gain determined.
    //
    // A tolerance of +/- 0.05 pixels is deemed acceptable.
    //
    // This won't work if if x is close to or at zero, so add 2.0L just in case
    //
    // First try long doubles
    //
    mp_prec_t precision = BASE_PRECISION;
    // check horizontal
    long double error = 0.05L;
    long double ld_pixels = static_cast<long double>(hpixels);
    long double pos1 = std::abs(stringToLongDouble(c.r)) + 10.0L;
    std::string base_x = longdoubleToString(pos1);
    long double length = stringToLongDouble(w);
    long double pitch = length/ld_pixels;
    long double pos2 = pos1 + length;
    long double check = (pos2 - pos1)/pitch;
    bool acceptable = (check > (ld_pixels - error)) && (check < (ld_pixels + error));
    if (acceptable)
    {
        // Check vertical
        long double ld_pixels = static_cast<long double>(vpixels);
        long double pos1 = std::abs(stringToLongDouble(c.i)) + 10.0L;
        long double length = stringToLongDouble(w);
        length = (length*static_cast<long double>(vpixels))/static_cast<long double>(hpixels);
        long double pitch = length/ld_pixels;
        long double pos2 = pos1 + length;
        long double check = (pos2 - pos1)/pitch;
        acceptable = (check > (ld_pixels - error)) && (check < (ld_pixels + error));
    }
    
    while (!acceptable)
    {
        // do it again increasing precision until it is acceptable
        precision += 16;
        MpReal pos1 = MpReal(c.r) + MpReal(10, precision);
        MpReal length(w, precision);
        MpReal mp_pixels(hpixels, precision); // width in pixels
        MpReal pitch = length/mp_pixels;
        MpReal pos2 = pos1 + length;
        MpReal mp_check = (pos2 - pos1)/pitch;
        long double check = mp_check.toLDouble();
        acceptable = (check > (ld_pixels - error)) && (check < (ld_pixels + error));
        if (acceptable)
        {
            // Check vertical
            MpReal mp_pixels(vpixels, precision);
            MpReal pos1 = MpReal(c.i) + MpReal(10, precision);
            MpReal length = MpReal(w, precision);
            length = (length*MpReal(vpixels, precision))/MpReal(hpixels, precision);
            MpReal pitch = length/ld_pixels;
            MpReal pos2 = pos1 + length;
            MpReal mp_check = (pos2 - pos1)/pitch;
            long double check = mp_check.toLDouble();
            acceptable = (check > (ld_pixels - error)) && (check < (ld_pixels + error));
        }
    }
    return precision;
}

std::string abbreviate(const std::string& s)
{
    std::string str(s);
    if (s.length() > 10)
    {
        str = s.substr(0, 10);
        str += "...";
    }
    return str;
}

QString fractalTypeToString(FRACTAL_TYPE ft)
{
    QString name("Unknown");
    switch (ft)
    {
    default: break;
    case F_MC_4_3_2_1: name = "MC 4.3.2.1"; break;
    case F_MC_4_3: name = "MC 4.3"; break;
    case F_MC_4_2: name = "MC 4.2"; break;
    case F_MC_5_4_3_2_1: name = "MC 5.4.3.2.1"; break;
    case F_MC_SIN: name = "MC SIN"; break;
    case F_MC_3_2_1: name = "MC 3.2.1"; break;
    case F_MC_4_1: name = "MC 4.1"; break;
    case F_MC_3_1: name = "MC 3.1"; break;
    case F_MC_3_2: name = "MC 3.2"; break;
    case F_MC_4_3_2: name = "MC 4.3.2"; break;
    case F_MC_5_1: name = "MC 5.1"; break;
    case F_MC_5_2: name = "MC 5.2"; break;
    case F_MC_5_3: name = "MC 5.3"; break;
    case F_MC_5_4: name = "MC 5.4"; break;
    case F_MC_5_4_3: name = "MC 5.4.3"; break;
    case F_MC_6_1: name = "MC 6.1"; break;
    case F_MC_6_2: name = "MC 6.2"; break;
    case F_MC_6_3: name = "MC 6.3"; break;
    case F_MC_6_4: name = "MC 6.4"; break;
    case F_MC_6_5: name = "MC 6.5"; break;
    case F_MC_6_5_4: name = "MC 6.5.4"; break;
    case F_MC_6_5_4_3_2_1: name = "MC 6.5.4.3.2.1"; break;
    case F_MC_7_1: name = "MC 7.1"; break;
    case F_MC_7_2: name = "MC 7.2"; break;
    case F_MC_7_3: name = "MC 7.3"; break;
    case F_MC_7_4: name = "MC 7.4"; break;
    case F_MC_7_5: name = "MC 7.5"; break;
    case F_MC_7_6: name = "MC 7.6"; break;
    case F_MC_7_6_5: name = "MC 7.6.5"; break;
    case F_MC_7_6_5_4_3_2_1: name = "MC 7.6.5.4.3.2.1"; break;
    case F_MC_2_M2: name = "MC 2.-2"; break;
    case F_MC_2_M3: name = "MC 2.-3"; break;
    case F_MC_2_M4: name = "MC 2.-4"; break;
    case F_MC_3_M2: name = "MC 3.-2"; break;
    case F_MC_3_M3: name = "MC 3.-3"; break;
    case F_MC_3_M4: name = "MC 3.-4"; break;
    case F_MC_4_M2: name = "MC 4.-2"; break;
    case F_MC_4_M3: name = "MC 4.-3"; break;
    case F_MC_4_M4: name = "MC 4.-4"; break;
    case F_MC_P_C_2_M2: name = "MC +c(2.-2)"; break;
    case F_MC_P_C_2_M3: name = "MC +c(2.-3)"; break;
    case F_MC_P_C_2_M4: name = "MC +c(2.-4)"; break;
    case F_MC_P_C_3_M2: name = "MC +c(3.-2)"; break;
    case F_MC_P_C_3_M3: name = "MC +c(3.-3)"; break;
    case F_MC_P_C_3_M4: name = "MC +c(3.-4)"; break;
    case F_MC_P_C_4_M2: name = "MC +c(4.-2)"; break;
    case F_MC_P_C_4_M3: name = "MC +c(4.-3)"; break;
    case F_MC_P_C_4_M4: name = "MC +c(4.-4)"; break;
    case F_MC_P_C_3_M1: name = "MC +c(3.-1)"; break;
    case F_MC_P_C_4_M1: name = "MC +c(4.-1)"; break;
    case F_MC_8_1: name = "MC 8.1"; break;
    case F_MC_8_2: name = "MC 8.2"; break;
    case F_MC_8_3: name = "MC 8.3"; break;
    case F_MC_8_4: name = "MC 8.4"; break;
    case F_MC_8_5: name = "MC 8.5"; break;
    case F_MC_8_6: name = "MC 8.6"; break;
    case F_MC_8_7: name = "MC 8.7"; break;
    case F_MC_8_7_6: name = "MC 8.7.6"; break;
    case F_MC_8_7_6_5_4_3_2_1: name = "MC 8.7.6.5.4.3.2.1"; break;
    }
    return name;
}

QString addExtension(const QString& path, const QString& extension)
{
    // This method ensures that the file has the appropriate extension
    bool add = true;
    QString str = path;
    QString home = QDir::homePath();
    auto length = path.length() - (home.length() + 1); // length of home doesn't include the separating /
    QString last = path.right(1);
    if (length > 4)
    {
        QString ext = path.right(4);
        add = (extension.toUpper() != ext.toUpper());
    }
    if (add)
    {
        if (last != ".")
        {
            str += extension;
        }
        else
        {
            str += extension.mid(1);
        }
    }
    return str;
}

QString name_only(const QString& path)
{
    int pos = path.lastIndexOf('/');
    return (pos == -1 ? path : path.mid(pos + 1));
}

int convertFromNetworkFormat32(unsigned char *data)
{
    int value = 0;
    value += (static_cast<int>(data[0]) & 0x000000ff);
    value = value << 8;
    value += (static_cast<int>(data[1]) & 0x000000ff);
    value = value << 8;
    value += (static_cast<int>(data[2]) & 0x000000ff);
    value = value << 8;
    value += (static_cast<int>(data[3]) & 0x000000ff);
    return value;
}

void quadratic(const LongComplex& a,
               const LongComplex& b,
               const LongComplex& c,
               VectorLongComplex& roots)
{
    roots.clear();
    LongComplex sqrt_part = ld::sqrt(b*b - 4.0L*a*c);
    LongComplex root1 = (-b + sqrt_part)/(2.0L*a);
    LongComplex root2 = (-b - sqrt_part)/(2.0L*a);
    roots.push_back(root1);
    roots.push_back(root2);
}

void roots(const LongComplex& z,
           int power,
           VectorLongComplex& roots)
{
    static const long double pi = 2.0L*acos(0.0L);
    roots.clear();
    if (power == 2)
    {
        LongComplex root1 = ld::sqrt(z);
        LongComplex root2 = -root1;
        roots.push_back(root1);
        roots.push_back(root2);
    }
    else
    {
        long double arg_increment = (2.0L*pi)/static_cast<long double>(power);
        long double nth_root = 1.0L/static_cast<long double>(power);
        LongComplex root = ld::pow(z, LongComplex(nth_root, 0.0L));
        roots.push_back(root);
        long double r = ld::abs(root);
        long double arg = ld::arg(root);
        for (int i = 1; i < power; i++)
        {
            arg += arg_increment;
            roots.push_back(LongComplex(r*std::cos(arg), r*std::sin(arg)));
        }
    }
}

void balance(ComplexMatrix& a, int& i_low, int& i_high)
// ------------------------------------------------------------------
//  This procedure is a translation of the EISPACK procedure Balanc.
//
//  This procedure balances a real matrix and isolates eigenvalues
//  whenever possible.
//
//  On input:
//
//   A contains the input matrix to be balanced.
//
//    i_low, i_high are the lowest and highest indices of the elements of a.
//
//  On output:
//
//    a contains the balanced matrix.
//
{
    const long double RADIX = 16.0L;

    int lb = 0;
    int ub = a.size() - 1;
    int m;
    long double c, f, g, r, s, b2;
    bool flag, found, conv;
    
    std::vector<long double> scale(a.size());

    b2 = (RADIX * RADIX);
    i_low = lb;
    i_high = ub;
 
    // Search for rows isolating an eigenvalue and push them down
    do
    {
        for (int j = i_high; j >= lb; j--)
        {
            for (int i = lb; i <= i_high; i++)
            {
                if (i == j) continue;
                LongComplex& value = a[i][j];
                flag = ((value.real() != 0.0L) || (value.imag() != 0.0L));
                if (flag)
                {
                    break;
                }
            }
            found = !flag;
            if (found)
            {
                m = i_high;
                // Row and column exchange
                scale[m] = j;
                if (j != m)
                {
                    for (int i = lb; i <= i_high; i++)
                    {
                        std::swap(a[j][i], a[m][i]);
                    }

                    for (int i = i_low; i <= ub; i++)
                    {
                        std::swap(a[i][j], a[i][m]);
                    }
                }
                i_high--;
                break;
            }
        }
    } while (found && (i_high >= lb));

    if (i_high < lb) i_high = lb;
    if (i_high == lb) return;

    // Search for columns isolating an eigenvalue and push them left
    do
    {
        for (int j = i_low; j <= i_high; j++)
        {
            for (int i = i_low; i <= i_high; i++)
            {
                if (i == j) continue;
                LongComplex& value = a[i][j];
                flag = ((value.real() != 0.0L) || (value.imag() != 0.0L));
                if (flag)
                {
                    break;
                }
            }
            found = !flag;
            if (found)
            {
                m = i_low;
                // Row and column exchange
                scale[m] = j;
                if (j != m)
                {
                    for (int i = lb; i <= i_high; i++)
                    {
                        std::swap(a[j][i], a[m][i]);
                    }

                    for (int i = i_low; i <= ub; i++)
                    {
                        std::swap(a[i][j], a[i][m]);
                    }
                }
                i_low++;
                break;
            }
        }
    } while (found);

    // Now balance the submatrix in rows i_low to i_igh
    for (int i = i_low; i <= i_high; i++) scale[i] = 1.0L; // check range of loop

    //  Iterative loop for norm reduction
    do
    {
        conv = true;
        for (int i = i_low; i <= i_high; i++)
        {
            c = 0.0L;
            r = 0.0L;

            for (int j = i_low; j <= i_high; j++)
            {
                if (j != i)
                {
                    c += std::abs(a[i][j].real()) + std::abs(a[i][j].imag());
                    r += std::abs(a[j][i].real()) + std::abs(a[j][i].imag());
                }
            }

            // Guard against zero C or R due to underflow
            if ((c != 0.0L) && (r != 0.0L))
            {
                g = r/RADIX;
                f = 1.0L;
                s = c + r;

                while (c < g)
                {
                    f *= RADIX;
                    c *= b2;
                }

                g = r*RADIX;

                while (c >= g)
                {
                    f /= RADIX;
                    c /= b2;
                }

                // Now balance
                if ((c+r)/f < (0.95*s))
                {
                    g = 1.0L/f;
                    scale[i] *= f;
                    conv = false;
                    for (int j = i_low; j < ub; j++)
                    {
                        a[j][i] *= g;
                    }
                    for (int j = lb; j < i_high; j++)
                    {
                        a[i][j] *= f;
                    }
                }
            }
        }
    } while (!conv);
}

long double pythag(long double a, long double b)
{
//
//     finds dsqrt(a**2+b**2) without overflow or destructive underflow
//
      long double p = std::max(std::abs(a), std::abs(b));
      if ((p != 0.0L) && !isnan(p))
      {
          long double r = std::min(std::abs(a),std::abs(b))/p;
          r *= r;
          for (;;)
          {
              long double t = r + 4.0L;
              if ((t == 4.0L) || isnan(p))
              {
                  break;
              }
              long double s = r/t;
              long double u = 1.0L + 2.0L*s;
              p *= u;
              r = (s/u)*(s/u)*r;
          } 
      }
      return p;
}

void orth(ComplexMatrix& a, int low, int high)
{
    int size = a.size();
    int order = size - 1;
    
    std::vector<LongComplex> ort;
    ort.resize(high+1);
    long double scale = 0.0L;
    
    int la = high - 1;
    int kp1 = low + 1;
    
    if (la >= kp1)
    {
        for (int m = kp1; m <= la; m++)
        {
            long double h = 0.0L;
            scale = 0.0L;
            for (int i = m; i <= high; i++)
            {
                scale += (std::abs(a[m-1][i].real()) + std::abs(a[m-1][i].imag()));
            }
            if (scale != 0.0L)
            {
                int mp = m + high;
                for (int ii = m; ii <= high; ii++)
                {
                    int i = mp - ii;
                    ort[i] = a[m-1][i]/scale;
                    h += ld::norm(ort[i]);
                }
                
                long double g = std::sqrt(h);
                long double f = pythag(ort[m].real(), ort[m].imag());
                if (f != 0.0L)
                {
                    h += f*g;
                    g /= f;
                    ort[m] *= (1.0L + g);
                }
                else
                {
                    ort[m] = LongComplex(g, ort[m].imag());
                    a[m-1][m] = LongComplex(scale, a[m-1][m].imag());
                }
                for (int j = m; j <= order; j++)
                {
                    LongComplex f;
                    for (int ii = m; ii <= high; ii++)
                    {
                        int i = mp - ii;
                        f += LongComplex(ort[i].real()*a[j][i].real() + ort[i].imag()*a[j][i].imag(),
                                         ort[i].real()*a[j][i].imag() - ort[i].imag()*a[j][i].real());
                    }
                    f /= h;
                    for (int i = m; i <= high; i++)
                    {
                        a[j][i] -= LongComplex(f.real()*ort[i].real() - f.imag()*ort[i].imag(),
                                                f.real()*ort[i].imag() + f.imag()*ort[i].real());
                    }
                }
                for (int i = 0; i <= high; i++)
                {
                    LongComplex f;
                    for (int jj = m; jj <= high; jj++)
                    {
                        int j = mp - jj;
                        f += LongComplex(ort[j].real()*a[j][i].real() - ort[j].imag()*a[j][i].imag(),
                                          ort[j].real()*a[j][i].imag() + ort[j].imag()*a[j][i].real());
                    }
                    f /= h;
                    for (int j = m; j <= high; j++)
                    {
                        a[j][i] += LongComplex(-(f.real()*ort[j].real() + f.imag()*ort[j].imag()),
                                                 f.real()*ort[j].imag() - f.imag()*ort[j].real());
                        
                    }
                }
                ort[m] *= scale;
                a[m-1][m] *= -g;
            }
        }
    }
}

void csroot(const LongComplex& x, LongComplex& y)
{
//
//     (yr,yi) = complex dsqrt(xr,xi) 
//     branch chosen so that yr .ge. 0.0 and sign(yi) .eq. sign(xi)
//
      long double tr = x.real();
      long double ti = x.imag();
      long double yr = 0.0L;
      long double yi = 0.0L;
      long double s = std::sqrt(0.5L*(pythag(tr,ti) + std::abs(tr)));
      if (tr >= 0.0L) yr = s;
      if (ti < 0.0L) s = -s;
      if (tr <= 0.0L) yi = s;
      if (tr < 0.0L) yr = 0.5L*(ti/yi);
      if (tr > 0.0L) yi = 0.5L*(ti/yr);
      y = LongComplex(yr, yi);
}

void compqr(ComplexMatrix& h, int low, int high, VectorLongComplex& roots, int& ierr)
{
    int l = 0;
    int lp1 = 1;
    LongComplex s, t, x, y, z;
    int n = h.size();
    roots.clear();
    for (int i = 0; i<n; i++) roots.push_back(LongComplex());

    ierr = 0;
    if (low != high)
    {
        l = low + 1;
        for (int i = l; i <= high; i++)
        {
            int ll = std::min(i+1, high);
            if (h[i-1][i].imag() == 0.0L) continue;
            long double norm = pythag(h[i-1][i].real(), h[i-1][i].imag());
            LongComplex y = h[i-1][i]/norm;
            h[i-1][i] = LongComplex(norm, 0.0L);
            for (int j = i; j <= high; j++)
            {
                long double imaginary = y.real()*h[j][i].imag() - y.imag()*h[j][i].real();
                h[j][i].real(y.real()*h[j][i].real() + y.imag()*h[j][i].imag());
                h[j][i].imag(imaginary);
            }
            for (int j = low; j <= ll; j++)
            {
                long double imaginary = y.real()*h[i][j].imag() + y.imag()*h[i][j].real();
                h[i][j].real(y.real()*h[i][j].real() - y.imag()*h[i][j].imag());
                h[i][j].imag(imaginary);
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        if ((i >= low) && (i <= high)) continue;
        roots[i] = h[i][i];
    }
    int en = high;
    int iterations = 30*n;
    if (en < low)
    {
        return;
    }
    int iteration = 0;
    int enm1 = en - 1;
    for (;;)
    {
        // Look for single small sub-diagonal element
        for (int ll = low; ll <= en; ll++)
        {
            l = en + low - ll;
            if (l == low) break; // exit loop
            long double test1 = std::abs(h[l-1][l-1].real()) + std::abs(h[l-1][l-1].imag()) +
                                std::abs(h[l][l].real()) + std::abs(h[l][l].imag());
            long double test2 = test1 + std::abs(h[l-1][l].real());
            if (test1 == test2) break;
        }
        // form shift
        if (l == en)
        {
            // a root has been found
            roots[en] = h[en][en] + t;
            en = enm1;
            if (en < low)
            {
                ierr = en + 1;
                return;
            }
            iteration = 0;
            enm1 = en - 1;
        }
        else if (iterations == 0)
        {
            break;
        }
        else
        {
            if ((iteration == 10) || (iteration == 20))
            {
                long double a = std::abs(h[enm1][en].real()) + 
                                std::abs(h[en-2][enm1].real());
                s = LongComplex(a, 0.0L);
            }
            else
            {
                s = h[en][en];
                x.real(h[en][enm1].real()*h[enm1][en].real());
                x.imag(h[en][enm1].imag()*h[enm1][en].real());
                if (x != LongComplex())
                {
                    y = (h[enm1][enm1] - s)/2.0L;
                    csroot(y*y + x, z);
                    if ((y.real()*z.real() + y.imag()*z.imag()) < 0.0L)
                    {
                        z = -z;
                    }
                    x /= (y + z);
                    s -= x;
                }
            }
            for (int i = low; i <= en; i++)
            {
                h[i][i] -= s;
            }
            t += s;
            iteration++;
            iterations--;
            // reduce to triangle (rows)
            lp1 = l + 1;
            
            for (int i = lp1; i <= en; i++)
            {
                long double s = h[i-1][i].real();
                h[i-1][i].real(0.0L);
                long double norm = pythag(pythag(h[i-1][i-1].real(), h[i-1][i-1].imag()), s);
                x = h[i-1][i-1]/norm;
                roots[i-1] = x;
                h[i-1][i-1].real(norm);
                h[i-1][i-1].imag(0.0L);
                h[i-1][i].imag(s/norm);
                for (int j = i; j <= en; j++)
                {
                    y = h[j][i-1];
                    z = h[j][i];
                    h[j][i-1].real(x.real()*y.real() + x.imag()*y.imag() + h[i-1][i].imag()*z.real());
                    h[j][i-1].imag(x.real()*y.imag() - x.imag()*y.real() + h[i-1][i].imag()*z.imag());
                    h[j][i].real(x.real()*z.real() - x.imag()*z.imag() - h[i-1][i].imag()*y.real());
                    h[j][i].imag(x.real()*z.imag() + x.imag()*z.real() - h[i-1][i].imag()*y.imag());
                }
            }
            s = h[en][en];
            if (s.imag() != 0.0L)
            {
                long double norm = pythag(s.real(), s.imag());
                s /= norm;
                h[en][en] = LongComplex(norm, 0.0L);
            }
            // inverse operation (columns)
            for (int j = lp1; j <= en; j++)
            {
                x = roots[j-1];
                for (int i = l; i <= j; i++)
                {
                    y = LongComplex(h[j-1][i].real(), 0.0L);
                    z = h[j][i];
                    if (i != j)
                    {
                        y.imag(h[j-1][i].imag());
                        h[j-1][i].imag(x.real()*y.imag() + x.imag()*y.real() + h[j-1][j].imag()*z.imag());
                    }
                    h[j-1][i].real(x.real()*y.real() - x.imag()*y.imag() + h[j-1][j].imag()*z.real());
                    h[j][i].real(x.real()*z.real() + x.imag()*z.imag() - h[j-1][j].imag()*y.real());
                    h[j][i].imag(x.real()*z.imag() - x.imag()*z.real() - h[j-1][j].imag()*y.imag());
                }
            }
            if (s.imag() != 0.0L)
            {
                for (int i = l; i <= en; i++)
                {
                    h[en][i] *= s;
                }
            }
        }
    }
    ierr = en + 1;
}
                    
void polyroots(const VectorLongComplex& coefficients, VectorLongComplex& roots)
{
    // Roots of polynomial from companion matrix
    int low;
    int high;
    int status;
    
    int degree = coefficients.size();
    int size = degree - 1;
   
    ComplexMatrix matrix; //(size);

    // Set up companion matrix
    VectorLongComplex row;
    for (int i = 1; i <= size; i++)
    {
        row.push_back(-coefficients[size - i]/coefficients[size]);
    }
    matrix.push_back(row);
    for (int i = 1; i < size; i++)
    {
        row.clear();
        for (int j = 0; j < size; j++)
        {
            row.push_back(LongComplex((i-1) == j ? 1.0L : 0.0L, 0.0L));
        }
        matrix.push_back(row);
    }

    // The roots of the polynomial are the eigenvalues of the companion matrix
    balance(matrix, low, high);
    orth(matrix, low, high);
    compqr(matrix, low, high, roots, status);
}

bool isnan(long double x)
{
    return (x != x);
}