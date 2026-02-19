#include "helpers.hpp"
#include <cmath>

namespace calendrical{
    bool is_integer(double x)
    {
        return std::isfinite(x) && std::fmod(x, 1) == 0;
    }

    double floor_div(double x, double y)
    {
        return std::floor(x / y);
    }

    double mod(double x, double y)
    {
        return x - y * floor_div(x, y);
    }

    double interval_modulus(double x, double a, double b)
    {
        if (a == b)
            return x;
        return a + mod(x - a, b - a);
    }

    int64_t sigma(const predicate_t<int64_t> &p, const value_function_t<int64_t>& f, int64_t k)
    {
        int64_t s = 0;
        while(p(k)){
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

    static constexpr int64_t passing_guard = 400; // little above the year

    int64_t least_passing(const predicate_t<int64_t>& phi, int64_t d0)
    {
        const auto begin = d0;
        while (!phi(d0) && std::abs(begin - d0) < passing_guard)
            ++d0;
        return d0;
    }
    int64_t last_passing(const predicate_t<int64_t>& phi, int64_t d0)
    {
        const auto begin = d0;
        while (phi(d0) && std::abs(begin - d0) < passing_guard)
            ++d0;
        return d0 - 1;
    }

    double binary_search(const predicate_t<double>& phi, const double_predicate_t<double>& psi, double u, double v)
    {
        while (!psi(u,v))
            if (const auto x = 0.5 * (u + v); phi(x))
                v = x;
            else
                u = x;
        return 0.5 * (u + v);
    }

    double inverse_angular(const value_function_t<double>& f, double y, double a, double b)
    {
        auto psi = [](const double& l, const double& u)
        { return u - l < 1e-5; };
        auto phi = [&f, &y](const double& x)
        { return mod(f(x) - y, 360) < 180; };
        return binary_search(phi, psi, a, b);
    }
}