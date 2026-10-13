#include "moment.hpp"
#include <cmath>

namespace calendrical {
    fixed_date_t moment_t::to_fixed_date() const {
        return fixed_date_t{ static_cast<int64_t>(std::floor(m_v)) };
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
