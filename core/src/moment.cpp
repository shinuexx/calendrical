#include "moment.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

namespace calendrical {
    namespace {
        /**
         * Rounds a day value to a fixed date; +infinity is the end-of-time sentinel.
         * Throws std::domain_error for NaN, -infinity and results outside [-2^63, 2^63).
         */
        template <typename Rounding>
        fixed_date_t rounded_fixed_date(double days, Rounding rounding, const char* name) {
            if (days == std::numeric_limits<double>::infinity())
                return fixed_date_t::end_of_time;
            const double rounded = rounding(days);
            // double(INT64_MAX) is 2^63 itself, so the upper bound has to be exclusive.
            constexpr double lower = -0x1p63;
            constexpr double upper = 0x1p63;
            if (!(rounded >= lower && rounded < upper))
                throw std::domain_error(std::string{ name } + ": moment " + std::to_string(days) +
                                        " has no fixed_date_t representation");
            return fixed_date_t{ static_cast<int64_t>(rounded) };
        }
    }

    fixed_date_t moment_t::to_fixed_date() const {
        return rounded_fixed_date(m_v, [](double x) { return std::floor(x); }, "moment_t::to_fixed_date");
    }
    fixed_date_t floor(const moment_t& m) {
        return m.to_fixed_date();
    }
    fixed_date_t ceil(const moment_t& m) {
        return rounded_fixed_date(m.days(), [](double x) { return std::ceil(x); }, "ceil(moment_t)");
    }
    fixed_date_t round(const moment_t& m) {
        return rounded_fixed_date(m.days(), [](double x) { return std::round(x); }, "round(moment_t)");
    }
    double moment_t::time() const {
        return mod(m_v, 1.0);
    }

    std::string to_string(const moment_t& m) {
        return std::to_string(m.m_v);
    }
    std::chrono::hh_mm_ss<std::chrono::milliseconds> moment_t::to_hh_mm_ss(const moment_t& input) {
        const auto time = static_cast<int64_t>(input.time() * (24 * 60 * 60 * 1000));
        const std::chrono::milliseconds ms{time};
        return std::chrono::hh_mm_ss{ms};
    }

    std::ostream& operator<<(std::ostream& out, const moment_t& m) {
        return out << to_string(m);
    }
}
