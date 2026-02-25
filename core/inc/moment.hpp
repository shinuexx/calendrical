#pragma once

#include "helpers.hpp"
#include "fixed_date.hpp"
#include <utility>

namespace calendrical {
    /**
     * @brief Continuous time value represented in days.
     *
     * Unlike fixed_date_t (integer days), moment_t stores fractional days and is
     * used by astronomical calculations and time-of-day operations.
     */
    class moment_t {
        double m_v;

    public:

        /** @brief Constructs zero moment. */
        constexpr moment_t() : m_v{ 0.0 } {}
        // ReSharper disable once CppNonExplicitConvertingConstructor
        /** @brief Constructs from raw day-based floating-point value. */
        constexpr moment_t(double v) : m_v{ v } {} // NOLINT(*-explicit-constructor)
        // ReSharper disable once CppNonExplicitConvertingConstructor
        /** @brief Constructs from a fixed date at local midnight. */
        constexpr moment_t(const fixed_date_t& rd) : m_v{ static_cast<double>(rd.operator int64_t()) } {} // NOLINT(*-explicit-constructor)
        // ReSharper disable once CppNonExplicitConversionOperator
        /** @brief Converts to the raw floating-point day value. */
        constexpr operator double() const { return m_v; } // NOLINT(*-explicit-constructor)
        /** @brief Converts to a fixed date using the library rounding rule. */
        explicit operator fixed_date_t() const { return to_fixed_date(); }

        constexpr bool operator==(const moment_t& other) const { return m_v == other.m_v; }
        constexpr auto operator<=>(const moment_t& other) const { return m_v <=> other.m_v; }

        /** @brief Returns a new moment shifted forward by @p x days. */
        [[nodiscard]] constexpr moment_t add(double x) const { return m_v + x; }
        /** @brief Returns a new moment shifted backward by @p x days. */
        [[nodiscard]] constexpr moment_t subtract(double x) const { return m_v - x; }

        /** @brief Fractional time-of-day component. */
        [[nodiscard]] double time() const;

        /** @brief Converts to the corresponding fixed date. */
        [[nodiscard]] fixed_date_t to_fixed_date() const;

        friend std::string to_string(const moment_t& m);

        /** @brief Creates a moment from hours/minutes/seconds within one day. */
        static constexpr moment_t hms(double h, double m = 0, double s = 0) {
            return (h + (m + s / 60) / 60) / 24;
        }

        /** @brief Sentinel representing an effectively unbounded upper moment. */
        static const moment_t end_of_time;

        /** @brief Converts a moment into `hh:mm:ss.mmm` components. */
        static std::chrono::hh_mm_ss<std::chrono::milliseconds> to_hh_mm_ss(const moment_t& input);
    };

    inline constexpr moment_t moment_t::end_of_time{ std::numeric_limits<double>::infinity() };

    std::string to_string(const moment_t& m);
}

std::ostream& operator<<(std::ostream& out, const calendrical::moment_t& m);

/** @brief User-defined literal for moment_t from integer day value. */
constexpr calendrical::moment_t operator""_m(unsigned long long x) {
    return calendrical::moment_t{ static_cast<double>(x) };
}
/** @brief User-defined literal for moment_t from floating-point day value. */
constexpr calendrical::moment_t operator""_m(long double x) {
    return calendrical::moment_t{ static_cast<double>(x) };
}
