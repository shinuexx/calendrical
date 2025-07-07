#include "moment.hpp"
#include <cmath>

namespace calendrical {
    fixed_date_t moment_t::to_fixed_date() const {
        return static_cast<int64_t>(std::floor(m_v));
    }
    double moment_t::time() const {
        return mod(m_v, 1.0);
    }

    fixed_date_t rational_moment_t::to_fixed_date() const {
        return static_cast<int64_t>(floor(m_v));
    }
    rational_moment_t::operator rational() const
    {
        return m_v;
    }
    rational_moment_t::operator moment_t() const
    {
        return static_cast<double>(m_v);
    }

    rational rational_moment_t::time() const
    {
        static const rational one{ 1 };
        return mod(m_v, one);
    }

    rational_moment_t rational_moment_t::hms(const rational& h, const rational& m, const rational& s)
    {
        return rational((h + (m + s / 60) / 60) / 24);
    }
    rational_moment_t rational_moment_t::hms(const rational& h, const rational& m)
    {
        return rational((h + m / 60) / 24);
    }
    rational_moment_t rational_moment_t::hms(const rational& h)
    {
        return rational(h / 24);
    }

    std::string to_string(const moment_t& m)
    {
        return std::to_string(m.m_v);
    }
    std::string to_string(const rational_moment_t& m)
    {
        return m.m_v.str();
    }

    std::chrono::hh_mm_ss<std::chrono::milliseconds> moment_t::to_hh_mm_ss(const moment_t& input) {
        const auto time = static_cast<int64_t>(input.time() * (24 * 60 * 60 * 1000));
        const std::chrono::milliseconds ms{time};
        return std::chrono::hh_mm_ss{ms};
    }
}

std::ostream& operator<<(std::ostream& out, const calendrical::moment_t& m)
{
    return out << to_string(m);
}
std::ostream& operator<<(std::ostream& out, const calendrical::rational_moment_t& m)
{
    return out << to_string(m);
}