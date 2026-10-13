#include "angle.hpp"
#include "helpers.hpp"
#include <cmath>

namespace calendrical {
    std::string to_string(const angle_t& angle) {
        return std::to_string(angle.m_v) + "°";
    }

    double sin(const angle_t& angle) {
        return std::sin(radians(angle.m_v));
    }
    double cos(const angle_t& angle) {
        return std::cos(radians(angle.m_v));
    }
    double tan(const angle_t& angle) {
        return std::tan(radians(angle.m_v));
    }

    angle_t asin(double value) {
        return angle_t{degrees(std::asin(value))};
    }
    angle_t acos(double value) {
        return angle_t{degrees(std::acos(value))};
    }
    angle_t atan(double value) {
        return angle_t{degrees(std::atan(value))};
    }
    angle_t arctan(double y, double x) {
        if (x == 0 && y == 0)
            return angle_t{std::numeric_limits<double>::quiet_NaN()};
        double r;
        if (x == 0)
            r = sign(y) * 90;
        else {
            const auto a = degrees(std::atan(y / x));
            r = x >= 0 ? a : a + 180;
        }
        return angle_t{mod(r, 360)};
    }

    std::ostream& operator<<(std::ostream& out, const angle_t& angle) {
        return out << to_string(angle);
    }
}