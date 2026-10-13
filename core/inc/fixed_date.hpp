#pragma once

#include <string>
#include "weekday.hpp"
#include "helpers.hpp"

namespace calendrical {
    /**
     * @brief Fixed day number (Rata Die style integer day index).
     *
     * This is the central internal date representation used by calendar
     * conversions. Most calendar implementations convert to and from this type.
     */
    class fixed_date_t {
        int64_t m_v;

    public:
        template <std::integral T>
        /** @brief Constructs from an integer day index. */
        constexpr explicit fixed_date_t(T v) : m_v{ static_cast<int64_t>(v) } {}
        /** @brief Constructs a zero-initialized fixed date. */
        constexpr fixed_date_t() : m_v{} {}

        // ReSharper disable once CppNonExplicitConversionOperator
        /** @brief Converts to the underlying integer day index. */
        constexpr operator int64_t() const { return m_v; } // NOLINT(*-explicit-constructor)

        constexpr bool operator==(const fixed_date_t& other) const = default;
        constexpr auto operator<=>(const fixed_date_t& other) const = default;

        /** @brief Returns the weekday for this fixed date. */
        [[nodiscard]] constexpr weekday get_weekday() const { return weekday{ static_cast<int>(mod(m_v, 7)) }; }

        /** @brief Named constructor for readability at call sites. */
        static constexpr fixed_date_t of(int64_t value) { return fixed_date_t{ value }; }

        /** @brief Returns a date shifted by @p value days forward. */
        [[nodiscard]] constexpr fixed_date_t add(int64_t value) const { return fixed_date_t{ m_v + value }; }
        /** @brief Returns a date shifted by @p value days backward. */
        [[nodiscard]] constexpr fixed_date_t subtract(int64_t value) const { return fixed_date_t{ m_v - value }; }

        /** @brief Unary negation (for negative R.D. dates). */
        [[nodiscard]] constexpr fixed_date_t operator-() const { return fixed_date_t{ -m_v }; }

        template <std::integral T>
        [[nodiscard]] constexpr fixed_date_t operator+(T days) const { return fixed_date_t{ m_v + static_cast<int64_t>(days) }; }

        template <std::integral T>
        [[nodiscard]] friend constexpr fixed_date_t operator+(T days, const fixed_date_t& date) { return date + days; }

        template <std::integral T>
        [[nodiscard]] constexpr fixed_date_t operator-(T days) const { return fixed_date_t{ m_v - static_cast<int64_t>(days) }; }

        [[nodiscard]] constexpr int64_t operator-(const fixed_date_t& other) const { return m_v - other.m_v; }

        template <std::integral T>
        constexpr fixed_date_t& operator+=(T days) { m_v += static_cast<int64_t>(days); return *this; }

        template <std::integral T>
        constexpr fixed_date_t& operator-=(T days) { m_v -= static_cast<int64_t>(days); return *this; }

        constexpr fixed_date_t& operator++() { ++m_v; return *this; }
        constexpr fixed_date_t operator++(int) { auto tmp = *this; ++m_v; return tmp; }
        constexpr fixed_date_t& operator--() { --m_v; return *this; }
        constexpr fixed_date_t operator--(int) { auto tmp = *this; --m_v; return tmp; }

        /** @brief Returns a debug-friendly string representation. */
        [[nodiscard]] std::string to_string() const;

        /** @brief Sentinel representing an effectively unbounded upper date. */
        static const fixed_date_t end_of_time;
    };
    inline constexpr fixed_date_t fixed_date_t::end_of_time{ std::numeric_limits<int64_t>::max() };

    /** @brief Stream output for @ref fixed_date_t. */
    std::ostream& operator<<(std::ostream& out, const fixed_date_t& rd);
}

/** @brief User-defined literal for fixed_date_t (e.g. `10_rd`). */
constexpr calendrical::fixed_date_t operator""_rd(unsigned long long x)
{
    return calendrical::fixed_date_t{ static_cast<int64_t>(x) };
}
