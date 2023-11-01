#include "location.hpp"

namespace calendrical {
    angle_t location::direction(const location& other) const
    {
        auto &[psi, phi, e,z] = *this;
        auto &[psi1, phi1, e1, z1] = other;
        auto y = sin(phi1 - phi);
        auto x = cos(psi) * tan(psi1) - sin(psi) * cos(phi - phi1);
        if (((x == 0) && (y == 0)) || (psi1 == 90_deg))
            return 0_deg;
        else if (psi1 == -90_deg)
            return angle_t::half_circle;
        else
            return arctan(y, x);
    }
    rational_location::operator location() const
    {
        return {
            (angle_t)latitude,
            (angle_t)longitude,
            elevation.convert_to<double>(),
            (moment_t)zone,
        };
    }
}