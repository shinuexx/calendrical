#pragma once

#include <cstdint>
#include <string>

#include "helpers.hpp"

namespace calendrical {
    class fixed_date_t;

    /**
     * @brief Weekday value in range [0, 6].
     *
     * The canonical mapping is:
     * 0=sunday, 1=monday, ..., 6=saturday.
     */
    struct weekday {
        static const weekday sunday;
        static const weekday monday;
        static const weekday tuesday;
        static const weekday wednesday;
        static const weekday thursday;
        static const weekday friday;
        static const weekday saturday;

    private:
        int m_v;

    public:
        /** @brief Constructs Sunday (0). */
        constexpr weekday() : m_v{0} {}
        // ReSharper disable once CppNonExplicitConvertingConstructor
        /** @brief Constructs from raw weekday index (not validated). */
        constexpr weekday(int v) : m_v{ v } {} // NOLINT(*-explicit-constructor)

        /** @brief Returns true if the value is within the valid weekday range. */
        [[nodiscard]] constexpr bool valid() const { return 0 <= m_v && m_v <= 6; }

        /** @brief Explicit conversion to the raw weekday index. */
        constexpr explicit operator int() const { return m_v; }

        constexpr bool operator==(const weekday& other) const { return m_v == other.m_v; }
        constexpr auto operator<=>(const weekday& other) const { return m_v <=> other.m_v; }

        /** @brief Previous occurrence of this weekday on or before @p date. */
        [[nodiscard]] fixed_date_t on_or_before(const fixed_date_t& date) const;
        /** @brief Next occurrence of this weekday on or after @p date. */
        [[nodiscard]] fixed_date_t on_or_after(const fixed_date_t& date) const;
        /** @brief Nearest occurrence of this weekday to @p date. */
        [[nodiscard]] fixed_date_t nearest(const fixed_date_t& date) const;
        /** @brief Previous occurrence strictly before @p date. */
        [[nodiscard]] fixed_date_t before(const fixed_date_t& date) const;
        /** @brief Next occurrence strictly after @p date. */
        [[nodiscard]] fixed_date_t after(const fixed_date_t& date) const;

        constexpr weekday& operator++()
        {
            ++m_v;
            return *this;
        }
        constexpr weekday& operator--()
        {
            --m_v;
            return *this;
        }

        constexpr weekday operator++(int)
        {
            const auto copy = *this;
            operator++();
            return copy;
        }

        constexpr weekday operator--(int)
        {
            const auto copy = *this;
            operator--();
            return copy;
        }

        constexpr int operator-(const weekday& rhs) const { return m_v - rhs.m_v; }

        /** @brief Returns lowercase weekday name or a fallback debug string. */
        [[nodiscard]] std::string to_string() const;
    };

    inline constexpr weekday weekday::sunday{ 0 };
    inline constexpr weekday weekday::monday{ 1 };
    inline constexpr weekday weekday::tuesday{ 2 };
    inline constexpr weekday weekday::wednesday{ 3 };
    inline constexpr weekday weekday::thursday{ 4 };
    inline constexpr weekday weekday::friday{ 5 };
    inline constexpr weekday weekday::saturday{ 6 };

    template<
        std::ranges::range Range
    >
    /**
     * @brief Produces a lazy view of weekdays shifted by @p D (modulo 7).
     */
    decltype(auto) shift_days(Range& days, int64_t D) {
        return days | std::views::transform([D](const weekday& wd)
                               { return static_cast<weekday>(static_cast<int32_t>(mod(static_cast<int32_t>(wd) + D, 7))); });
    }

    /** @brief Stream output for @ref weekday. */
    std::ostream& operator<<(std::ostream& out, const weekday& wd);
}
