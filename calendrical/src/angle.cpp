#include "angle.hpp"
#include "helpers.hpp"
#include <cmath>
#include <stdexcept>

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
        return degrees(std::asin(value));
    }
    angle_t acos(double value) {
        return degrees(std::acos(value));
    }
    angle_t atan(double value) {
        return degrees(std::atan(value));
    }
    angle_t arctan(double y, double x) {
        if ((x == 0) && (y == 0)) throw std::range_error("x == y == 0");
        else {
            double r;
            if (x == 0) r = sign(y) * 90;
            else {
                auto a = degrees(std::atan(y / x));
                r = x >= 0 ? a : a + 180;
            }
            return r;
        }
    }
}

std::ostream& operator<<(std::ostream& out, const calendrical::angle_t& angle) {
    return out << calendrical::to_string(angle);
}