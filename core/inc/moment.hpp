#pragma once

#include "helpers.hpp"
#include "fixed_date.hpp"
#include <utility>

namespace calendrical {
    class moment_t {
        double m_v;

    public:

        constexpr moment_t() : m_v{ 0.0 } {}
        // ReSharper disable once CppNonExplicitConvertingConstructor
        constexpr moment_t(double v) : m_v{ v } {} // NOLINT(*-explicit-constructor)
        // ReSharper disable once CppNonExplicitConvertingConstructor
        constexpr moment_t(const fixed_date_t& rd) : m_v{ static_cast<double>(rd.operator int64_t()) } {} // NOLINT(*-explicit-constructor)
        // ReSharper disable once CppNonExplicitConversionOperator
        constexpr operator double() const { return m_v; } // NOLINT(*-explicit-constructor)
        explicit operator fixed_date_t() const { return to_fixed_date(); }

        constexpr bool operator==(const moment_t& other) const { return m_v == other.m_v; }
        constexpr auto operator<=>(const moment_t& other) const { return m_v <=> other.m_v; }

        [[nodiscard]] constexpr moment_t add(double x) const { return m_v + x; }
        [[nodiscard]] constexpr moment_t subtract(double x) const { return m_v - x; }

        [[nodiscard]] double time() const;

        [[nodiscard]] fixed_date_t to_fixed_date() const;

        friend std::string to_string(const moment_t& m);

        static constexpr moment_t hms(double h, double m = 0, double s = 0) {
            return (h + (m + s / 60) / 60) / 24;
        }

        static const moment_t end_of_time;

        static std::chrono::hh_mm_ss<std::chrono::milliseconds> to_hh_mm_ss(const moment_t& input);
    };

    inline constexpr moment_t moment_t::end_of_time{ std::numeric_limits<double>::infinity() };

    std::string to_string(const moment_t& m);
}

std::ostream& operator<<(std::ostream& out, const calendrical::moment_t& m);

constexpr calendrical::moment_t operator""_m(unsigned long long x) {
    return calendrical::moment_t{ static_cast<double>(x) };
}
constexpr calendrical::moment_t operator""_m(long double x) {
    return calendrical::moment_t{ static_cast<double>(x) };
}
