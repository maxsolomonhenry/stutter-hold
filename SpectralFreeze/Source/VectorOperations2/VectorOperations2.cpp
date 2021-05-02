/*
  ==============================================================================

    VectorOperations2.cpp
    Created: 28 Mar 2021 6:06:23pm
    Author:  Julian Vanasse

  ==============================================================================
*/

#include "VectorOperations2.h"

template <typename T>
boost::numeric::ublas::vector<T> jv_bst::add(boost::numeric::ublas::vector<T> x, T a)
{
    boost::numeric::ublas::vector<T> y (x.size());
    for (int n = 0; n < x.size(); n++)
    {
        y(n) = x(n) + a;
    }
    return y;
}

template <typename T>
boost::numeric::ublas::vector<T> jv_bst::max(boost::numeric::ublas::vector<T> x, T a)
{
    boost::numeric::ublas::vector<T> y (x.size());
    for (int n = 0; n < x.size(); n++)
    {
        if (x(n) > a)
            y(n) = x(n);
        else
            x(n) = a;
    }
    return y;
}

template <typename T>
boost::numeric::ublas::vector<T> jv_bst::max(boost::numeric::ublas::vector<T> x, boost::numeric::ublas::vector<T> y)
{
    boost::numeric::ublas::vector<T> w (x.size());
    for (int n = 0; n < x.size(); n++)
    {
        if (x(n) > y(n))
            w(n) = x(n);
        else
            w(n) = y(n);
    }
    return y;
}

template <typename T>
boost::numeric::ublas::vector<T> jv_bst::min(boost::numeric::ublas::vector<T> x, T a)
{
    boost::numeric::ublas::vector<T> y (x.size());
    for (int n = 0; n < x.size(); n++)
    {
        if (x(n) < a)
            y(n) = x(n);
        else
            x(n) = a;
    }
    return y;
}

template <typename T>
boost::numeric::ublas::vector<T> jv_bst::min(boost::numeric::ublas::vector<T> x, boost::numeric::ublas::vector<T> y)
{
    boost::numeric::ublas::vector<T> w (x.size());
    for (int n = 0; n < x.size(); n++)
    {
        if (x(n) < y(n))
            w(n) = x(n);
        else
            w(n) = y(n);
    }
    return y;
}

template <typename T>
T jv_bst::mean(boost::numeric::ublas::vector<T> x)
{
    T mu = T();
    for (int n = 0; n < x.size(); n++)
    {
        mu += x(n);
    }
    mu /= static_cast<T>(x.size());
    return mu;
}

template <typename T>
boost::numeric::ublas::vector<T> jv_bst::real(boost::numeric::ublas::vector<std::complex<T> > z)
{
    boost::numeric::ublas::vector<T> r (z.size());
    for (int n = 0; n < z.size(); n++)
    {
        r(n) = z(n).real();
    }
    return r;
}

template <typename T>
boost::numeric::ublas::vector<T> jv_bst::imag(boost::numeric::ublas::vector<std::complex<T> > z)
{
    boost::numeric::ublas::vector<T> i (z.size());
    for (int n = 0; n < z.size(); n++)
    {
        i(n) = z(n).imag();
    }
    return i;
}

template <typename T>
boost::numeric::ublas::vector<T> jv_bst::abs(boost::numeric::ublas::vector<std::complex<T> > z)
{
    boost::numeric::ublas::vector<T> v (z.size());
    for (int n = 0; n < z.size(); n++)
    {
        v(n) = abs(z(n));
    }
    return v;
}

template <typename T>
boost::numeric::ublas::vector<T> jv_bst::abs(boost::numeric::ublas::vector<T> x)
{
    boost::numeric::ublas::vector<T> y (x.size());
    for (int n = 0; n < x.size(); n++)
    {
        y(n) = std::abs(x(n));
    }
    return x;
}

template <typename T>
boost::numeric::ublas::vector<T> jv_bst::angle(boost::numeric::ublas::vector<std::complex<T> > z)
{
    boost::numeric::ublas::vector<T> v (z.size());
    for (int n = 0; n < z.size(); n++)
    {
        v(n) = std::arg(z(n));
    }
    return v;
}

template <typename T>
boost::numeric::ublas::vector<T> jv_bst::pow(boost::numeric::ublas::vector<T> x, T p)
{
    boost::numeric::ublas::vector<T> y (x.size());
    for (int n = 0; n < x.size(); n++)
    {
        y(n) = std::pow(x(n), p);
    }
    return y;
}

template <typename T> boost::numeric::ublas::vector<T> jv_bst::sin(boost::numeric::ublas::vector<T> t)
{
    boost::numeric::ublas::vector<T> y (t.size());
    for (int n = 0; n < t.size(); n++)
    {
        y(n) = std::sin(t(n));
    }
    return y;
}

template <typename T> boost::numeric::ublas::vector<T> jv_bst::cos(boost::numeric::ublas::vector<T> t)
{
    boost::numeric::ublas::vector<T> y (t.size());
    for (int n = 0; n < t.size(); n++)
    {
        y(n) = std::cos(t(n));
    }
    return y;
}

template <typename T> boost::numeric::ublas::vector<T> jv_bst::wrap_to_pi(boost::numeric::ublas::vector<T> t)
{
    boost::numeric::ublas::vector<T> y (t.size());
    for (int n = 0; n < t.size(); n++)
    {
        y(n) = t(n) - (2.0 * M_PI * std::floor((t(n) + M_PI) / (2.0 * M_PI)));
    }
    return y;
}

template <typename T>
boost::numeric::ublas::vector<T> jv_bst::zp(boost::numeric::ublas::vector<T> x, int len)
{
    /* zero-pad */
    boost::numeric::ublas::vector<T> y (x.size() + len, 0.0);
    for (int n = 0; n < x.size(); n++)
    {
        y(n) = x(n);
    }
    return y;
}

template <typename T> boost::numeric::ublas::vector<std::complex<T> > jv_bst::zp(boost::numeric::ublas::vector<std::complex<T> > z, int len)
{
    /* zero-pad */
    boost::numeric::ublas::vector<std::complex<T> > y (z.size() + len, 0.0);
    for (int n = 0; n < z.size(); n++)
    {
        y(n) = z(n);
    }
    return y;
}

template <typename T>
boost::numeric::ublas::vector<std::complex<T> > jv_bst::complex(boost::numeric::ublas::vector<T> r)
{
    /* takes real vector and outputs complex vector */
    boost::numeric::ublas::vector<std::complex<T> > z (r.size());
    for (int n = 0; n < z.size(); n++)
    {
        z(n) = r(n);
    }
    return z;
}

template <typename T>
boost::numeric::ublas::vector<std::complex<T> > jv_bst::complex(boost::numeric::ublas::vector<T> r, boost::numeric::ublas::vector<T> i)
{
    /* takes real vector and outputs complex vector */
    boost::numeric::ublas::vector<std::complex<T> > z (r.size());
    std::complex<T> u;
    for (int n = 0; n < z.size(); n++)
    {
        u = std::complex<T>(r(n), i(n));
        z(n) = u;
    }
    return z;
}

template <typename T>
boost::numeric::ublas::vector<std::complex<T> > jv_bst::pol2cart(boost::numeric::ublas::vector<T> r, boost::numeric::ublas::vector<T> p)
{
    /* convert magnitude and phase vectors to cartesian complex vector */
    
    if (r.size() != p.size())
        throw std::length_error("r and p must be same size");
    
    boost::numeric::ublas::vector<std::complex<T> > z (r.size(), T());
    for (int n = 0; n < z.size(); n++)
    {
        T re = r(n) * std::cos(p(n));
        T im = r(n) * std::sin(p(n));
        z(n) = std::complex<T> (re, im);
    }
    return z;
}

template <typename T>
boost::numeric::ublas::vector<std::complex<T> > jv_bst::fft(boost::numeric::ublas::vector<T> r, kiss_fft_cfg dir)
{
    /* wrapper for kiss_fft */
    
    if (dir->nfft < r.size())
        throw std::length_error("vector size must be <= n_fft");
    
    // result
    boost::numeric::ublas::vector<std::complex<T> > result (dir->nfft);
    // use kiss_fft_cpx complex type
    boost::numeric::ublas::vector<kiss_fft_cpx> buffer (dir->nfft);
    // copy r into real position of buffer
    for (int n = 0; n < r.size(); n++)
    {
        buffer(n).r = r(n);
        buffer(n).i = 0.0;
    }
    for (int n = r.size(); n < buffer.size(); n++)
    {
        buffer(n).r = 0.0;
        buffer(n).i = 0.0;
    }
    
    // transform
    kiss_fft(dir, buffer.data().begin(), buffer.data().begin());
    
    // copy to result
    for (int n = 0; n < buffer.size(); n++)
    {
        result(n) = std::complex<T>(static_cast<T>(buffer(n).r), static_cast<T>(buffer(n).i));
    }
    
    // rescale if inverse
    if (dir->inverse)
    {
        result *= (1.0 / static_cast<T>(dir->nfft));
    }
    
    return result;
}

template <typename T>
boost::numeric::ublas::vector<std::complex<T> > jv_bst::fft(boost::numeric::ublas::vector<std::complex<T> > z, kiss_fft_cfg dir)
{
    /* wrapper for kiss_fft */
    
    if (dir->nfft < z.size())
        throw std::length_error("vector size must be <= n_fft");
    
    // result
    boost::numeric::ublas::vector<std::complex<T> > result (dir->nfft);
    // use kiss_fft_cpx complex type
    boost::numeric::ublas::vector<kiss_fft_cpx> buffer (dir->nfft);
    // copy r into real position of buffer
    for (int n = 0; n < z.size(); n++)
    {
        buffer(n).r = z(n).real();
        buffer(n).i = z(n).imag();
    }
    for (int n = z.size(); n < buffer.size(); n++)
    {
        buffer(n).r = 0.0;
        buffer(n).i = 0.0;
    }
    
    // transform
    kiss_fft(dir, buffer.data().begin(), buffer.data().begin());
    
    // copy to result
    for (int n = 0; n < buffer.size(); n++)
    {
        result(n) = std::complex<T>(static_cast<T>(buffer(n).r), static_cast<T>(buffer(n).i));
    }
    
    // rescale if inverse
    if (dir->inverse)
    {
        result *= (1.0 / static_cast<T>(dir->nfft));
    }
    
    return result;
}

template <typename T> bool jv_bst::has_nan(boost::numeric::ublas::vector<T> x)
{
    /* Return true if x contains nan value(s) */
    
    for (int n = 0; n < x.size(); n++)
    {
        if (std::isnan(x(n)))
            return true;
    }
    return false;
}

template <typename T> bool jv_bst::has_inf(boost::numeric::ublas::vector<T> x)
{
    for (int n = 0; n < x.size(); n++)
    {
        if (std::isinf(x(n)))
            return true;
    }
    return false;
}


template boost::numeric::ublas::vector<float>                   jv_bst::add(boost::numeric::ublas::vector<float> x, float a);
template boost::numeric::ublas::vector<float>                   jv_bst::max(boost::numeric::ublas::vector<float> x, float a);
template boost::numeric::ublas::vector<float>                   jv_bst::max(boost::numeric::ublas::vector<float> x, boost::numeric::ublas::vector<float> y);
template boost::numeric::ublas::vector<float>                   jv_bst::min(boost::numeric::ublas::vector<float> x, float a);
template boost::numeric::ublas::vector<float>                   jv_bst::min(boost::numeric::ublas::vector<float> x, boost::numeric::ublas::vector<float> y);
template float                                                  jv_bst::mean(boost::numeric::ublas::vector<float> x);
template boost::numeric::ublas::vector<float>                   jv_bst::real(boost::numeric::ublas::vector<std::complex<float> > z);
template boost::numeric::ublas::vector<float>                   jv_bst::imag(boost::numeric::ublas::vector<std::complex<float> > z);
template boost::numeric::ublas::vector<float>                   jv_bst::abs(boost::numeric::ublas::vector<std::complex<float> > z);
template boost::numeric::ublas::vector<float>                   jv_bst::abs(boost::numeric::ublas::vector<float> x);
template boost::numeric::ublas::vector<float>                   jv_bst::angle(boost::numeric::ublas::vector<std::complex<float> > z);
template boost::numeric::ublas::vector<float>                   jv_bst::pow(boost::numeric::ublas::vector<float> x, float p);
template boost::numeric::ublas::vector<float>                   jv_bst::sin(boost::numeric::ublas::vector<float> t);
template boost::numeric::ublas::vector<float>                   jv_bst::cos(boost::numeric::ublas::vector<float> t);
template boost::numeric::ublas::vector<float>                   jv_bst::wrap_to_pi(boost::numeric::ublas::vector<float> t);
template boost::numeric::ublas::vector<float>                   jv_bst::zp(boost::numeric::ublas::vector<float> x, int len);
template boost::numeric::ublas::vector<std::complex<float> >    jv_bst::zp(boost::numeric::ublas::vector<std::complex<float> > z, int len);
template boost::numeric::ublas::vector<std::complex<float> >    jv_bst::complex(boost::numeric::ublas::vector<float> r);
template boost::numeric::ublas::vector<std::complex<float> >    jv_bst::complex(boost::numeric::ublas::vector<float> r, boost::numeric::ublas::vector<float> i);
template boost::numeric::ublas::vector<std::complex<float> >    jv_bst::pol2cart(boost::numeric::ublas::vector<float> r, boost::numeric::ublas::vector<float> p);
template boost::numeric::ublas::vector<std::complex<float> >    jv_bst::fft(boost::numeric::ublas::vector<float> r, kiss_fft_cfg dir);
template boost::numeric::ublas::vector<std::complex<float> >    jv_bst::fft(boost::numeric::ublas::vector<std::complex<float> > z, kiss_fft_cfg dir);
template bool                                                   jv_bst::has_nan(boost::numeric::ublas::vector<float> x);
template bool                                                   jv_bst::has_inf(boost::numeric::ublas::vector<float> x);

template boost::numeric::ublas::vector<double>                  jv_bst::add(boost::numeric::ublas::vector<double> x, double a);
template boost::numeric::ublas::vector<double>                  jv_bst::max(boost::numeric::ublas::vector<double> x, double a);
template boost::numeric::ublas::vector<double>                  jv_bst::max(boost::numeric::ublas::vector<double> x, boost::numeric::ublas::vector<double> y);
template boost::numeric::ublas::vector<double>                  jv_bst::min(boost::numeric::ublas::vector<double> x, double a);
template boost::numeric::ublas::vector<double>                  jv_bst::min(boost::numeric::ublas::vector<double> x, boost::numeric::ublas::vector<double> y);
template double                                                 jv_bst::mean(boost::numeric::ublas::vector<double> x);
template boost::numeric::ublas::vector<double>                  jv_bst::real(boost::numeric::ublas::vector<std::complex<double> > z);
template boost::numeric::ublas::vector<double>                  jv_bst::imag(boost::numeric::ublas::vector<std::complex<double> > z);
template boost::numeric::ublas::vector<double>                  jv_bst::abs(boost::numeric::ublas::vector<std::complex<double> > z);
template boost::numeric::ublas::vector<double>                  jv_bst::abs(boost::numeric::ublas::vector<double> x);
template boost::numeric::ublas::vector<double>                  jv_bst::angle(boost::numeric::ublas::vector<std::complex<double> > z);
template boost::numeric::ublas::vector<double>                  jv_bst::pow(boost::numeric::ublas::vector<double> x, double p);
template boost::numeric::ublas::vector<double>                  jv_bst::sin(boost::numeric::ublas::vector<double> t);
template boost::numeric::ublas::vector<double>                  jv_bst::cos(boost::numeric::ublas::vector<double> t);
template boost::numeric::ublas::vector<double>                  jv_bst::wrap_to_pi(boost::numeric::ublas::vector<double> t);
template boost::numeric::ublas::vector<double>                  jv_bst::zp(boost::numeric::ublas::vector<double> x, int len);
template boost::numeric::ublas::vector<std::complex<double> >   jv_bst::zp(boost::numeric::ublas::vector<std::complex<double> > z, int len);
template boost::numeric::ublas::vector<std::complex<double> >   jv_bst::complex(boost::numeric::ublas::vector<double> r);
template boost::numeric::ublas::vector<std::complex<double> >   jv_bst::complex(boost::numeric::ublas::vector<double> r, boost::numeric::ublas::vector<double> i);
template boost::numeric::ublas::vector<std::complex<double> >   jv_bst::pol2cart(boost::numeric::ublas::vector<double> r, boost::numeric::ublas::vector<double> p);
template boost::numeric::ublas::vector<std::complex<double> >   jv_bst::fft(boost::numeric::ublas::vector<double> r, kiss_fft_cfg dir);
template boost::numeric::ublas::vector<std::complex<double> >   jv_bst::fft(boost::numeric::ublas::vector<std::complex<double> > z, kiss_fft_cfg dir);
template bool                                                   jv_bst::has_nan(boost::numeric::ublas::vector<double> x);
template bool                                                   jv_bst::has_inf(boost::numeric::ublas::vector<double> x);
