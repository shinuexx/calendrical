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
        /** @brief Rata Die moment in days; the integer part is the fixed date, the fraction the time of day. */
        double m_v;

    public:

        /** @brief Constructs zero moment. */
        constexpr moment_t() : m_v{ 0.0 } {}

        /** @brief Constructs from raw day-based arithmetic value. */
        template <typename T>
            requires std::is_arithmetic_v<T>
        constexpr explicit moment_t(T v) : m_v{ static_cast<double>(v) } {}

        /** @brief Constructs from a fixed date at local midnight. */
        constexpr explicit moment_t(const fixed_date_t& rd) : m_v{ static_cast<double>(rd.operator int64_t()) } {}

        // ReSharper disable once CppNonExplicitConversionOperator
        /** @brief Converts to the raw floating-point day value. */
        constexpr operator double() const { return m_v; } // NOLINT(*-explicit-constructor)
        /** @brief Converts to a fixed date using the library rounding rule. */
        explicit operator fixed_date_t() const { return to_fixed_date(); }

        /** @brief Equality comparison on the raw day value. */
        constexpr bool operator==(const moment_t& other) const = default;
        /** @brief Chronological three-way comparison (partial ordering: NaN is unordered). */
        constexpr auto operator<=>(const moment_t& other) const = default;

        /** @brief Unary negation. */
        [[nodiscard]] constexpr moment_t operator-() const { return moment_t{ -m_v }; }

        /** @brief Sum of two moments in days. */
        [[nodiscard]] constexpr moment_t operator+(const moment_t& other) const { return moment_t{ m_v + other.m_v }; }

        /** @brief Returns the moment @p days (possibly fractional) days later. */
        template <typename T>
            requires std::is_arithmetic_v<T>
        [[nodiscard]] constexpr moment_t operator+(T days) const { return moment_t{ m_v + static_cast<double>(days) }; }

        /** @brief Returns the moment @p days days after @p m (commutative form). */
        template <typename T>
            requires std::is_arithmetic_v<T>
        [[nodiscard]] friend constexpr moment_t operator+(T days, const moment_t& m) { return m + days; }

        /** @brief Difference of two moments in days. */
        [[nodiscard]] constexpr moment_t operator-(const moment_t& other) const { return moment_t{ m_v - other.m_v }; }

        /** @brief Returns the moment @p days (possibly fractional) days earlier. */
        template <typename T>
            requires std::is_arithmetic_v<T>
        [[nodiscard]] constexpr moment_t operator-(T days) const { return moment_t{ m_v - static_cast<double>(days) }; }

        /** @brief Returns @p days minus @p m, in days. */
        template <typename T>
            requires std::is_arithmetic_v<T>
        [[nodiscard]] friend constexpr moment_t operator-(T days, const moment_t& m) { return moment_t{ static_cast<double>(days) - m.m_v }; }

        /** @brief Scales the day value by @p factor. */
        template <typename T>
            requires std::is_arithmetic_v<T>
        [[nodiscard]] constexpr moment_t operator*(T factor) const { return moment_t{ m_v * static_cast<double>(factor) }; }

        /** @brief Scales the day value of @p m by @p factor (commutative form). */
        template <typename T>
            requires std::is_arithmetic_v<T>
        [[nodiscard]] friend constexpr moment_t operator*(T factor, const moment_t& m) { return m * factor; }

        /** @brief Divides the day value by @p factor. */
        template <typename T>
            requires std::is_arithmetic_v<T>
        [[nodiscard]] constexpr moment_t operator/(T factor) const { return moment_t{ m_v / static_cast<double>(factor) }; }

        /** @brief Ratio of two day values. */
        [[nodiscard]] constexpr double operator/(const moment_t& other) const { return m_v / other.m_v; }

        /** @brief Adds the day value of @p other to this moment. */
        constexpr moment_t& operator+=(const moment_t& other) { m_v += other.m_v; return *this; }

        /** @brief Moves this moment @p days days forward. */
        template <typename T>
            requires std::is_arithmetic_v<T>
        constexpr moment_t& operator+=(T days) { m_v += static_cast<double>(days); return *this; }

        /** @brief Subtracts the day value of @p other from this moment. */
        constexpr moment_t& operator-=(const moment_t& other) { m_v -= other.m_v; return *this; }

        /** @brief Moves this moment @p days days backward. */
        template <typename T>
            requires std::is_arithmetic_v<T>
        constexpr moment_t& operator-=(T days) { m_v -= static_cast<double>(days); return *this; }

        /** @brief Scales this moment's day value by @p factor. */
        template <typename T>
            requires std::is_arithmetic_v<T>
        constexpr moment_t& operator*=(T factor) { m_v *= static_cast<double>(factor); return *this; }

        /** @brief Divides this moment's day value by @p factor. */
        template <typename T>
            requires std::is_arithmetic_v<T>
        constexpr moment_t& operator/=(T factor) { m_v /= static_cast<double>(factor); return *this; }

        /** @brief Returns a new moment shifted forward by @p x days. */
        [[nodiscard]] constexpr moment_t add(double x) const { return moment_t{ m_v + x }; }
        /** @brief Returns a new moment shifted backward by @p x days. */
        [[nodiscard]] constexpr moment_t subtract(double x) const { return moment_t{ m_v - x }; }

        /** @brief Fractional time-of-day component. */
        [[nodiscard]] double time() const;

        /** @brief Converts to the corresponding fixed date. */
        [[nodiscard]] fixed_date_t to_fixed_date() const;

        /** @brief Returns a debug-friendly string representation of @p m. */
        friend std::string to_string(const moment_t& m);

        /** @brief Creates a moment from hours/minutes/seconds within one day. */
        static constexpr moment_t hms(double h, double m = 0, double s = 0) {
            return moment_t{ (h + (m + s / 60) / 60) / 24 };
        }

        /** @brief Sentinel representing an effectively unbounded upper moment. */
        static const moment_t end_of_time;

        /** @brief Converts a moment into `hh:mm:ss.mmm` components. */
        static std::chrono::hh_mm_ss<std::chrono::milliseconds> to_hh_mm_ss(const moment_t& input);
    };

    inline constexpr moment_t moment_t::end_of_time{ std::numeric_limits<double>::infinity() };

    /** @brief Returns a debug-friendly string representation of @p m. */
    std::string to_string(const moment_t& m);

    /** @brief Stream output for @ref moment_t. */
    std::ostream& operator<<(std::ostream& out, const moment_t& m);
}

/** @brief User-defined literal for moment_t from integer day value. */
constexpr calendrical::moment_t operator""_m(unsigned long long x) {
    return calendrical::moment_t{ static_cast<double>(x) };
}
/** @brief User-defined literal for moment_t from floating-point day value. */
constexpr calendrical::moment_t operator""_m(long double x) {
    return calendrical::moment_t{ static_cast<double>(x) };
}
