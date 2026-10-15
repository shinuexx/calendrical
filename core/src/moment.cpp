#include "moment.hpp"
#include <cmath>
#include <limits>

namespace calendrical {
    namespace {
        /** Rounds a day value to a fixed date; +infinity is the end-of-time sentinel. */
        template <typename Rounding>
        fixed_date_t rounded_fixed_date(double days, Rounding rounding) {
            if (days == std::numeric_limits<double>::infinity())
                return fixed_date_t::end_of_time;
            return fixed_date_t{ static_cast<int64_t>(rounding(days)) };
        }
    }

    fixed_date_t moment_t::to_fixed_date() const {
        return rounded_fixed_date(m_v, [](double x) { return std::floor(x); });
    }
    fixed_date_t floor(const moment_t& m) {
        return m.to_fixed_date();
    }
    fixed_date_t ceil(const moment_t& m) {
        return rounded_fixed_date(m.days(), [](double x) { return std::ceil(x); });
    }
    fixed_date_t round(const moment_t& m) {
        return rounded_fixed_date(m.days(), [](double x) { return std::round(x); });
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
