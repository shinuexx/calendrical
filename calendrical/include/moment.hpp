#pragma once

#include "helpers.hpp"
#include "fixed_date.hpp"
#include <utility>
#include <iostream>

namespace calendrical {
    class moment_t {
        double m_v;

    public:

        constexpr moment_t() : m_v{ 0.0 } {}
        constexpr moment_t(double v) : m_v{ v } {}
        constexpr moment_t(const fixed_date_t& rd) : m_v{ static_cast<double>(rd.operator int64_t()) } {}
        constexpr operator double() const { return m_v; }
        explicit operator fixed_date_t() const { return to_fixed_date(); }

        constexpr bool operator==(const moment_t& other) const { return m_v == other.m_v; }
        constexpr auto operator<=>(const moment_t& other) const { return m_v <=> other.m_v; }

        double time() const;

        fixed_date_t to_fixed_date() const;

        friend std::string to_string(const moment_t& m);

        static constexpr moment_t hms(double h, double m = 0, double s = 0) {
            return (h + (m + s / 60) / 60) / 24;
        }
    };

    class rational_moment_t
    {
        rational m_v;

    public:
        rational_moment_t() : m_v{} {}
        rational_moment_t(const rational& r) : m_v{ r } {}
        rational_moment_t(rational&& r) : m_v{ std::move(r) } {}
        rational_moment_t(double v) : m_v{ v } {}
        rational_moment_t(int64_t v) : m_v{ v } {}
        rational_moment_t(const bigint& v) : m_v{ v } {}
        rational_moment_t(bigint&& v) : m_v{ std::move(v) } {}
        rational_moment_t(const moment_t& m) : m_v{ m.operator double() } {}

        operator rational() const;
        explicit operator moment_t() const;
        explicit operator fixed_date_t() const { return to_fixed_date(); }

        constexpr bool operator==(const rational_moment_t& other) const { return m_v == other.m_v; }
        constexpr auto operator<=>(const rational_moment_t& other) const { return m_v.compare(other.m_v) <=> 0; }

        friend std::string to_string(const rational_moment_t& m);

        rational time() const;

        fixed_date_t to_fixed_date() const;

        static rational_moment_t hms(const rational& h, const rational& m, const rational& s);
        static rational_moment_t hms(const rational& h, const rational& m);
        static rational_moment_t hms(const rational& h);
    };

    std::string to_string(const moment_t& m);
    std::string to_string(const rational_moment_t& m);
}

std::ostream& operator<<(std::ostream& out, const calendrical::moment_t& m);
std::ostream& operator<<(std::ostream& out, const calendrical::rational_moment_t& m);

constexpr calendrical::moment_t operator"" _m(unsigned long long x) {
    return calendrical::moment_t{ static_cast<double>(x) };
}
constexpr calendrical::moment_t operator"" _m(long double x) {
    return calendrical::moment_t{ static_cast<double>(x) };
}