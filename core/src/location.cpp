#include "location.hpp"

namespace calendrical {
    angle_t location::direction(const location& other) const
    {
        auto &[phi, psi, e,z] = *this;
        auto &[phi1, psi1, e1, z1] = other;
        const auto y = sin(phi1 - phi);
        const auto x = cos(psi) * tan(psi1) - sin(psi) * cos(phi - phi1);
        if (x == 0 && y == 0 || psi1 == 90_deg)
            return 0_deg;
        if (psi1 == -90_deg)
            return angle_t::half_circle;
        return arctan(y, x);
    }
    rational_location::operator location() const
    {
        return {
            static_cast<angle_t>(latitude),
            static_cast<angle_t>(longitude),
            elevation.convert_to<double>(),
            static_cast<moment_t>(zone),
        };
    }
}