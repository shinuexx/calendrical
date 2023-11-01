#include "helpers.hpp"

namespace calendrical {
    static const rational half{ 1, 2 };

    bigint floor(const rational& r)
    {
        auto s = r.sign() < 0;
        bigint v = boost::multiprecision::abs(boost::multiprecision::numerator(r)) / boost::multiprecision::denominator(r);
        if (s)
            return -(++v);
        else
            return v;
    }
    bigint ceil(const rational& r)
    {
        auto s = r.sign() < 0;
        bigint v = boost::multiprecision::abs(boost::multiprecision::numerator(r)) / boost::multiprecision::denominator(r);
        if (s)
            return -v;
        else
            return ++v;
    }
    bigint round(const rational& r)
    {
        return floor(r + half);
    }
    bigint trunc(const rational& r)
    {
        return boost::multiprecision::numerator(r) / boost::multiprecision::denominator(r);
    }
    bool is_integer(double x)
    {
        return std::isfinite(x) && (std::fmod(x, 1) == 0);
    }
    bool is_integer(const rational& x)
    {
        return boost::multiprecision::denominator(x) == 1;
    }

    int sign(const rational& y)
    {
        return y.sign();
    }

    int sign(const bigint& y)
    {
        return y.sign();
    }

    double floor_div(double x, double y)
    {
        return std::floor(x / y);
    }

    rational floor_div(const rational& x, const rational& y)
    {
        return floor(x / y);
    }

    bigint floor_div(const bigint& x, const bigint& y)
    {
        return floor(rational{ x,y });
    }

    double mod(double x, double y)
    {
        return x - y * floor_div(x, y);
    }
    bigint mod(const bigint& x, const bigint& y)
    {
        return x - y * floor_div(x, y);
    }
    rational mod(const rational& x, const rational& y)
    {
        return x - y * floor_div(x, y);
    }

    double interval_modulus(double x, double a, double b)
    {
        if (a == b)
            return x;
        else
            return a + mod(x - a, b - a);
    }
    bigint interval_modulus(const bigint& x, const bigint& a, const bigint& b)
    {
        if (a == b)
            return x;
        else
            return a + mod(bigint{ x - a }, b - a);
    }
    rational interval_modulus(const rational& x, const rational& a, const rational& b)
    {
        if (a == b)
            return x;
        else
            return a + mod(x - a, b - a);
    }

    bigint adjusted_remainder(const bigint& x, const bigint& b)
    {
        auto m = mod(x, b);
        return m.is_zero() ? b : m;
    }

    int64_t sigma(const predicate_t<int64_t>& p, const value_function_t<int64_t>& f, int64_t k)
    {
        int64_t s = 0;
        while (p(k))
        {
            s += f(k);
            ++k;
        }
        return s;
    }
    int64_t pi(const predicate_t<int64_t>& p, const value_function_t<int64_t>& f, int64_t k)
    {
        int64_t s = 1;
        while (p(k))
        {
            s *= f(k);
            ++k;
        }
        return s;
    }

    int64_t least_passing(const predicate_t<int64_t>& phi, int64_t d0)
    {
        while (!phi(d0))
            ++d0;
        return d0;
    }
    int64_t last_passing(const predicate_t<int64_t>& phi, int64_t d0)
    {
        while (phi(d0))
            ++d0;
        return d0 - 1;
    }

    double binary_search(const predicate_t<double>& phi, const double_predicate_t<double>& psi, double u, double v)
    {
        auto x = 0.5 * (u + v);
        if (psi(u, v))
            return x;
        else if (phi(x))
            return binary_search(phi, psi, u, x);
        else
            return binary_search(phi, psi, x, v);
    }
    rational binary_search(const predicate_t<rational>& phi, const double_predicate_t<rational>& psi, const rational& u, const rational& v)
    {
        auto x = (u + v) / 2;
        if (psi(u, v))
            return x;
        else if (phi(x))
            return binary_search(phi, psi, u, x);
        else
            return binary_search(phi, psi, x, v);
    }

    double inverse_angular(const value_function_t<double>& f, double y, double a, double b)
    {
        auto psi = [](const double& l, const double& u)
        { return (u - l) < 1e-5; };
        auto phi = [&f, &y](const double& x)
        { return mod(f(x) - y, 360) < 180; };
        return binary_search(phi, psi, a, b);
    }
    rational inverse_angular(const value_function_t<rational>& f, const rational& y, const rational& a, const rational& b)
    {
        auto psi = [](const rational& l, const rational& u)
        {
            static const rational epsilon{ 1, 10000 };
            return (u - l) < epsilon;
        };
        auto phi = [&f, &y](const rational& x)
        {
            static const rational full_circle{ 360 };
            return mod(f(x) - y, full_circle) < 180;
        };
        return binary_search(phi, psi, a, b);
    }
    rational hms(const rational& h, const rational& m, const rational& s)
    {
        return (h + (m + s / 60) / 60) / 24;
    }
}