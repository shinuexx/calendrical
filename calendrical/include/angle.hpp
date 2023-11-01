#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>
#include "helpers.hpp"

namespace calendrical
{
    class rational_angle_t;
    class angle_t
    {
        friend class rational_angle_t;
        double m_v;
 
    public:
        constexpr angle_t() : m_v{0} {}
        constexpr angle_t(double v) : m_v{v} {}

        template <typename EnumType, std::enable_if_t<std::is_enum_v<EnumType>, int> = 0>
        constexpr angle_t(const EnumType &v) : m_v{static_cast<double>(static_cast<std::underlying_type_t<EnumType>>(v))} {}

        constexpr operator double() const { return m_v; }

        constexpr bool operator==(const angle_t &other) const { return m_v == other.m_v; }
        constexpr auto operator<=>(const angle_t &other) const { return m_v <=> other.m_v; }

        constexpr angle_t operator-() const { return -m_v; }

        constexpr angle_t operator+(const angle_t &right) const { return m_v + right.m_v; }
        constexpr angle_t operator-(const angle_t &right) const { return m_v - right.m_v; }
        constexpr angle_t operator*(double right) const { return m_v + right; }
        constexpr double operator/(const angle_t &right) const { return m_v / right.m_v; }

        static constexpr angle_t dms(double d, double m = 0, double s = 0) { return d + (m + s / 60) / 60; }

        static const angle_t full_circle;
        static const angle_t half_circle;

        friend std::string to_string(const angle_t &angle);

        friend double sin(const angle_t &angle);
        friend double cos(const angle_t &angle);
        friend double tan(const angle_t &angle);

        friend angle_t asin(double value);
        friend angle_t acos(double value);
        friend angle_t atan(double value);
        friend angle_t arctan(double y, double x);
    };

    class rational_angle_t {
        rational m_v;

    public:
        rational_angle_t() : m_v{ 0 } {}
        rational_angle_t(double v) : m_v{ v } {}
        rational_angle_t(const rational& v) : m_v{ v } {}
        rational_angle_t(rational&& v) : m_v{ std::move(v)} {}
        rational_angle_t(const angle_t& angle) : m_v{ angle.m_v } {}

        template <typename EnumType, std::enable_if_t<std::is_enum_v<EnumType>, int> = 0>
        rational_angle_t(const EnumType& v) : m_v{ static_cast<std::underlying_type_t<EnumType>>(v) } {}

        explicit operator double() const { return m_v.convert_to<double>(); }
        operator rational() const { return m_v; }

        constexpr bool operator==(const rational_angle_t& other) const { return m_v == other.m_v; }
        constexpr auto operator<=>(const rational_angle_t& other) const { return m_v.compare(other.m_v) <=> 0; }

        rational_angle_t operator-() const { return rational{ -m_v }; }

        rational_angle_t operator+(const rational_angle_t& right) const { return rational{ m_v + right.m_v }; }
        friend rational_angle_t operator+(const rational& left, const rational_angle_t& right) { return rational{ left + right.m_v }; }
        rational_angle_t operator-(const rational_angle_t& right) const { return rational{ m_v - right.m_v }; }
        rational_angle_t operator*(const rational& right) const { return rational{ m_v + right }; }
        friend rational_angle_t operator*(const rational& left, const rational_angle_t& right) { return rational(left * right.m_v); }
        rational operator/(const rational_angle_t& right) const { return m_v / right.m_v; }

        static rational_angle_t dms(const rational& d, const rational& m, const rational& s) { return rational{ d + (m + s / 60) / 60 }; }
        static rational_angle_t dms(const rational& d, const rational& m) { return rational{ d + m / 60 }; }

        friend std::string to_string(const rational_angle_t& angle);

        explicit operator angle_t() const { return m_v.convert_to<double>(); }

        static const rational_angle_t full_circle;
        static const rational_angle_t half_circle;
    };

    inline constexpr angle_t angle_t::full_circle{ 360 };
    inline constexpr angle_t angle_t::half_circle{ 180 };

    std::string to_string(const angle_t &angle);
    std::string to_string(const rational_angle_t &angle);

    double sin(const angle_t &angle);
    double cos(const angle_t &angle);
    double tan(const angle_t &angle);

    angle_t asin(double value);
    angle_t acos(double value);
    angle_t atan(double value);
    angle_t arctan(double y, double x);
}

constexpr calendrical::angle_t operator"" _deg(long double x)
{
    return static_cast<double>(x);
}
constexpr calendrical::angle_t operator"" _deg(uint64_t x)
{
    return static_cast<double>(x);
}

calendrical::rational_angle_t operator"" _rdeg(long double x);
calendrical::rational_angle_t operator"" _rdeg(uint64_t x);

std::ostream &operator<<(std::ostream &out, const calendrical::angle_t &angle);
std::ostream &operator<<(std::ostream &out, const calendrical::rational_angle_t &angle);