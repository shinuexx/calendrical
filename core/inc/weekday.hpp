#pragma once

#include <cstdint>
#include <string>

#include "helpers.hpp"

namespace calendrical {
    class fixed_date_t;

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
        constexpr weekday() : m_v{0} {}
        // ReSharper disable once CppNonExplicitConvertingConstructor
        constexpr weekday(int v) : m_v{ v } {} // NOLINT(*-explicit-constructor)

        [[nodiscard]] constexpr bool valid() const { return 0 <= m_v && m_v <= 6; }

        constexpr explicit operator int() const { return m_v; }

        constexpr bool operator==(const weekday& other) const { return m_v == other.m_v; }
        constexpr auto operator<=>(const weekday& other) const { return m_v <=> other.m_v; }

        [[nodiscard]] fixed_date_t on_or_before(const fixed_date_t& date) const;
        [[nodiscard]] fixed_date_t on_or_after(const fixed_date_t& date) const;
        [[nodiscard]] fixed_date_t nearest(const fixed_date_t& date) const;
        [[nodiscard]] fixed_date_t before(const fixed_date_t& date) const;
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
    decltype(auto) shift_days(Range& days, int64_t D) {
        return days | std::views::transform([D](const weekday& wd)
                               { return static_cast<weekday>(static_cast<int32_t>(mod(static_cast<int32_t>(wd) + D, 7))); });
    }
}

std::ostream& operator<<(std::ostream& out, const calendrical::weekday& wd);
