//
// Based on the GNU std::complex template library and turned into a specific
// class for handling long double complex numbers.
//
// Extra code has been added to following routines to handle values not
// correctly handled by the non-specialised code.
//
// long double abs(complex)
// long double norm(complex)
// complex pow(pmplex, mpreal)
// complex pow(complex, complex)
// complex *=(complex)
// complex /= (complex)
// complex sqrt(complex)
//
// The reason this class exists is because as of clang++ 3.2 the long double
// specialisations implemented by compiler's built in routines or using C99
// complex data types produce incorrect values for the abs and norm of
// (inf, nan)), division by complex zero, complex zero to complex zero etc.
//

#pragma once

#include <algorithm>
#include <cmath>
#include <limits>

namespace ld
{
    // Forward declarations.
    class complex;

    long double abs(const complex&);
    long double arg(const complex&);
    long double norm(const complex&);

    complex conj(const complex&);
    complex polar(const long double&, const long double&);

    // Transcendentals:
    complex cos(const complex&);
    complex cosh(const complex&);
    complex exp(const complex&);
    complex log(const complex&);
    complex log10(const complex&);
    complex pow(const complex&, int);
    complex pow(const complex&, const long double&);
    complex pow(const complex&, const complex&);
    complex pow(const long double&, const complex&);
    complex sin(const complex&);
    complex sinh(const complex&);
    complex sqrt(const complex&);
    complex tan(const complex&);
    complex tanh(const complex&);
    
    
    class complex
    {
    public:
        complex(const long double& r= 0.0L, const long double& i = 0.0L)
        : m_real(r), m_imag(i) { }

        complex(const complex& z)
        : m_real(z.real()), m_imag(z.imag()) { }

        long double& real() { return m_real; }
        const long double& real() const { return m_real; }
        void real(long double value) { m_real = value; }

        ///  Return imaginary part of complex number.
        long double& imag() { return m_imag; }
        const long double& imag() const { return m_imag; }
        void imag(long double value) { m_imag = value; }

        /// Assign this complex number to scalar @a t.
        complex& operator=(const long double&);

        complex&
        operator+=(const long double& t)
        {
            m_real += t;
            return *this;
        }

        complex&
        operator-=(const long double& t)
        {
            m_real -= t;
            return *this;
        }

        complex& operator*=(const long double&);
        complex& operator/=(const long double&);

        // Lets the compiler synthesize the
        // copy and assignment operator
        // complex<_Tp>& operator= (const complex<_Tp>&);
        /// Assign this complex number to complex @a z.
        complex& operator=(const complex&);
        /// Add @a z to this complex number.
        complex& operator+=(const complex&);
        /// Subtract @a z from this complex number.
        complex& operator-=(const complex&);
        /// Multiply this complex number by @a z.
        complex& operator*=(const complex&);
        /// Divide this complex number by @a z.
        complex& operator/=(const complex&);

    private:
        long double m_real;
        long double m_imag;
    };

    
    inline complex operator+(const complex& x, const complex& y)
    {
        complex r = x;
        r += y;
        return r;
    }

    inline complex operator+(const complex& x, const long double& y)
    {
        complex r = x;
        r += y;
        return r;
    }

    inline complex operator+(const long double& x, const complex& y)
    {
        complex r = y;
        r += x;
        return r;
    }

    inline complex operator-(const complex& x, const complex& y)
    {
        complex r = x;
        r -= y;
        return r;
    }
    
    inline complex operator-(const complex& x, const long double& y)
    {
        complex r = x;
        r -= y;
        return r;
    }

    inline complex operator-(const long double& x, const complex& y)
    {
        complex r(x, - y.imag());
        r -= y.real();
        return r;
    }

    inline complex operator*(const complex& x, const complex& y)
    {
        complex r = x;
        r *= y;
        return r;
    }

    inline complex operator*(const complex& x, const long double& y)
    {
        complex r = x;
        r *= y;
        return r;
    }

    inline complex operator*(const long double& x, const complex& y)
    {
        complex r = y;
        r *= x;
        return r;
    }

    inline complex operator/(const complex& x, const complex& y)
    {
        complex r = x;
        r /= y;
        return r;
    }
    
    inline complex operator/(const complex& x, const long double& y)
    {
        complex r = x;
        r /= y;
        return r;
    }

    inline complex operator/(const long double& x, const complex& y)
    {
        complex r = x;
        r /= y;
        return r;
    }

    inline complex operator+(const complex& x)
    { return x; }

    inline complex operator-(const complex& x)
    { return complex(-x.real(), -x.imag()); }

    inline bool operator==(const complex& x, const complex& y)
    { return x.real() == y.real() &&x.imag() == y.imag(); }

    inline bool operator==(const complex& x, const long double& y)
    { return x.real() == y && x.imag() == 0.0L; }

    inline bool operator==(const long double& x, const complex& y)
    { return (x == y.real()) && (0.0L == y.imag()); }

    inline bool operator!=(const complex& x, const complex& y)
    { return x.real() != y.real() || x.imag() != y.imag(); }

    inline bool operator!=(const complex& x, const long double& y)
    { return (x.real() != y) || (x.imag() != 0.0); }

    inline bool operator!=(const long double& x, const complex& y)
    { return (x != y.real()) || (0.0L != y.imag()); }

    // Values
    inline long double& real(complex& z) { return z.real(); }
    inline const long double& real(const complex& z) { return z.real(); }
    inline long double& imag(complex& z) { return z.imag(); }
    inline const long double& imag(const complex& z) { return z.imag(); }

    inline long double abs(const complex& z)
    {
        if ((std::abs(z.real()) == std::numeric_limits<long double>::infinity()) ||
             (std::abs(z.imag()) == std::numeric_limits<long double>::infinity()))
        {
            return std::numeric_limits<long double>::infinity();
        }
        else
        {
            long double x = z.real();
            long double y = z.imag();
            const long double s = std::max(std::abs(x), std::abs(y));
            if (s == 0.0L) return s;
            x /= s; 
            y /= s;
            return s*std::sqrt(x*x + y*y);
        }
    }

    inline long double arg(const complex& z) { return std::atan2(z.imag(), z.real()); }

    inline long double norm(const complex& z)
    {
        if ((std::abs(z.real()) == std::numeric_limits<long double>::infinity()) ||
            (std::abs(z.imag()) == std::numeric_limits<long double>::infinity()))
        {
            return std::numeric_limits<long double>::infinity();
        }
        else
        {
            const long double x = z.real();
            const long double y = z.imag();
            return x*x + y*y;
        }
    }

    inline complex polar(const long double& rho, const long double& theta)
    {
        return complex(rho*std::cos(theta), rho*std::sin(theta));
    }

    inline complex conj(const complex& z)
    {
        return complex(z.real(), -z.imag());
    }
  
    inline complex cos(const complex& z)
    {
        const long double x = z.real();
        const long double y = z.imag();
        return complex(std::cos(x)*std::cosh(y), -std::sin(x)*std::sinh(y));
    }

    inline complex cosh(const complex& z)
    {
        const long double x = z.real();
        const long double y = z.imag();
        return complex(std::cosh(x)*std::cos(y), std::sinh(x)*std::sin(y));
    }

    inline complex exp(const complex& z)
    {
        return polar(std::exp(z.real()), z.imag());
    }

    inline complex log(const complex& z)
    {
        return complex(std::log(ld::abs(z)), arg(z));
    }

    inline complex log10(const complex& z)
    { 
        return log(z) / log(10.0L);
    }

    inline complex sin(const complex& z)
    {
        const long double x = z.real();
        const long double y = z.imag();
        return complex(std::sin(x)*std::cosh(y), std::cos(x)*std::sinh(y)); 
    }

    inline complex sinh(const complex& z)
    {
        const long double x = z.real();
        const long double y = z.imag();
        return complex(std::sinh(x)*std::cos(y), std::cosh(x)*std::sin(y));
    }

    inline complex tan(const complex& z)
    {
        return sin(z)/cos(z);
    }

    inline complex tanh(const complex& z)
    {
        return sinh(z)/cosh(z);
    }

    inline complex pow(const complex& z, int n)
    {
        complex x = z;
        bool inverse = n < 0.0L;
        if (inverse) n = -n;
        complex y = n % 2 ? x : 1.0L;
        while (n >>= 1)
        {
            x *= x;
            if (n % 2)
            {
                y *= x;
            }
        }
        return inverse ? 1.0/y : y;
    }

    inline complex pow(const complex& x, const long double& y)
    {
        if (x == complex())
        {
            if (y == 0.0L)
            {
                return complex(std::numeric_limits<long double>::quiet_NaN(),
                               std::numeric_limits<long double>::quiet_NaN());
            }
            else
            {
                return complex();
            }
        }
        else if ((x.imag() == 0.0L) && (y < 0.0L))
        {
            return complex(std::pow(x.real(),y), 0.0L);
        }
        else if (x.imag() == 0.0L)
        {
            return complex(std::numeric_limits<long double>::infinity(),
                           -std::numeric_limits<long double>::quiet_NaN());
        }
        else if ((x.imag() == 0.0L) && (y >= 0.0L))
        {
            return complex();
        }
        else if ((x.imag() == 0.0L) && (x.real() > 0.0L))
        {
            return complex(std::pow(x.real(),y), 0.0L);
        }
        complex t = log(x);
        return polar(std::exp(y*t.real()), y*t.imag());
    }

    inline complex pow(const complex& x, const complex& y)
    {
        if (x == complex())
        {
            if (y == complex() || ((y.real() == 0.0L) && (y.imag() != 0)))
            {
                return complex(std::numeric_limits<long double>::quiet_NaN(),
                               std::numeric_limits<long double>::quiet_NaN());
            }
            else if (y.real() > 0.0L)
            {
                return complex();
            }
            else if (y.real() < 0.0L)
            {
                return complex(std::numeric_limits<long double>::infinity(),
                               std::numeric_limits<long double>::quiet_NaN());
            }
        }
        else if (y == complex())
        {
            if ((std::abs(x.real()) == std::numeric_limits<long double>::infinity()) ||
                (std::abs(x.imag()) == std::numeric_limits<long double>::infinity()))
            {
                return complex(std::numeric_limits<long double>::quiet_NaN(),
                               std::numeric_limits<long double>::quiet_NaN());
            }
            return complex(1.0L, 0.0L);
        }
        else if ((std::abs(x.real()) == std::numeric_limits<long double>::infinity()) ||
                 (std::abs(x.imag()) == std::numeric_limits<long double>::infinity()))

        {
            if (y.real() > 0.0L)
            {
                return x;
            }
            else if (y.real() < 0.0L)
            {
                return complex();
            }
            else
            {
                return complex(std::numeric_limits<long double>::quiet_NaN(),
                               std::numeric_limits<long double>::quiet_NaN());
            }
        }
        return exp(y*log(x));
    }

    inline complex pow(const long double& x, const complex& y)
    {
        return x > 0.0L ? polar(std::pow(x, y.real()), y.imag()*std::log(x))
                        : pow(complex(x), y);
    }

    inline complex& complex::operator=(const long double& t)
    {
        m_real = t;
        m_imag = 0.0L;
        return *this;
    } 

    inline complex& complex::operator*=(const long double& t)
    {
        m_real *= t;
        m_imag *= t;
        return *this;
    }

    inline complex& complex::operator/=(const long double& t)
    {
        m_real /= t;
        m_imag /= t;
        return *this;
    }

    inline complex& complex::operator=(const complex& z)
    {
        m_real = z.real();
        m_imag = z.imag();
        return *this;
    }

    inline complex& complex::operator+=(const complex& z)
    {
        m_real += z.real();
        m_imag += z.imag();
        return *this;
    }

    inline complex& complex::operator-=(const complex& z)
    {
        m_real -= z.real();
        m_imag -= z.imag();
        return *this;
    }

    inline complex& complex::operator*=(const complex& z)
    {
        if ((std::abs(m_real) == std::numeric_limits<long double>::infinity()) ||
            (std::abs(m_imag) == std::numeric_limits<long double>::infinity()))
        {
            if (z == complex())
            {
                m_real = std::numeric_limits<long double>::quiet_NaN();
                m_imag = std::numeric_limits<long double>::quiet_NaN();
            }
            return *this;
        }
        else if ((std::abs(z.real()) == std::numeric_limits<long double>::infinity()) ||
                 (std::abs(z.imag()) == std::numeric_limits<long double>::infinity()))
        {
            if ((m_real == 0) && (m_imag == 0))
            {
                m_real = std::numeric_limits<long double>::quiet_NaN();
                m_imag = std::numeric_limits<long double>::quiet_NaN();
            }
            else
            {
                m_real = z.real();
                m_imag = z.imag();
            }
            return *this;
        }
        const long double r = m_real*z.real() - m_imag*z.imag();
        m_imag = m_real*z.imag() + m_imag*z.real();
        m_real = r;
        return *this;
    }

    inline complex& complex::operator/=(const complex& z)
    {
        if (z == complex())
        {
            if (m_real == 0.0L)
            {
                m_real = -std::numeric_limits<long double>::quiet_NaN();
            }
            else if (m_real > 0.0L)
            {
                m_real = std::numeric_limits<long double>::infinity();
            }
            else
            {
                m_real = -std::numeric_limits<long double>::infinity();
            }
            if (m_imag == 0.0L)
            {
                m_imag = -std::numeric_limits<long double>::quiet_NaN();
            }
            else if (m_imag > 0.0L)
            {
                m_imag = std::numeric_limits<long double>::infinity();
            }
            else
            {
                m_imag = -std::numeric_limits<long double>::infinity();
            }
        }
        else if ((std::abs(z.real()) == std::numeric_limits<long double>::infinity()) ||
                 (std::abs(z.imag()) == std::numeric_limits<long double>::infinity()))
        {
            if ((m_real == std::numeric_limits<long double>::infinity()) ||
                (m_imag == std::numeric_limits<long double>::infinity()))
            {
                m_real = std::numeric_limits<long double>::quiet_NaN();
                m_imag = std::numeric_limits<long double>::quiet_NaN();
            }
            else if ((m_real != m_real) || (m_imag != m_imag))
            {
                return *this;
            }
            else
            {
                m_real = 0.0L;
                m_imag = 0.0L;
            }
        }
        else
        {
            const long double r =  m_real*z.real() + m_imag*z.imag();
            const long double n = ld::norm(z);
            m_imag = (m_imag*z.real() - m_real*z.imag())/n;
            m_real = r/n;
        }
        return *this;
    }


    inline complex sqrt(const complex& z)
    {
        if ((std::abs(z.real()) == std::numeric_limits<long double>::infinity()) ||
            (std::abs(z.imag()) == std::numeric_limits<long double>::infinity()))
        {
            return z;
        }
        long double x = z.real();
        long double y = z.imag();

        if (x == 0.0L)
        {
            long double t = std::sqrt(std::abs(y) / 2.0L);
            return complex(t, y < 0.0L ? -t : t);
        }
        else
        {
            long double t = std::sqrt(2.0L * (ld::abs(z) + std::abs(x)));
            long double u = t / 2.0L;
            return x > 0.0L ? complex(u, y/t)
                            : complex(ld::abs(y) / t, y < 0.0L ? -u : u);
        }
    }

} // end of mpfr namespace

