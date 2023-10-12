#pragma once

#include <cstdint>
#include <compare>
#include <vector>
#include <string>
#include <iostream>

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
        constexpr weekday() : m_v{ 0 } {}
        constexpr weekday(int v) : m_v{ v } {}

        constexpr bool valid() const { return (0 <= m_v) && (m_v <= 6); }

        constexpr explicit operator int() const { return m_v; }

        constexpr bool operator==(const weekday& other) const { return m_v == other.m_v; }
        constexpr auto operator<=>(const weekday& other) const { return m_v <=> other.m_v; }

        fixed_date_t on_or_before(const fixed_date_t& date) const;
        fixed_date_t on_or_after(const fixed_date_t& date) const;
        fixed_date_t nearest(const fixed_date_t& date) const;
        fixed_date_t before(const fixed_date_t& date) const;
        fixed_date_t after(const fixed_date_t& date) const;

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
            auto copy = *this;
            operator++();
            return copy;
        }

        constexpr weekday operator--(int)
        {
            auto copy = *this;
            operator--();
            return copy;
        }

        constexpr int operator-(const weekday& rhs) const { return m_v - rhs.m_v; }

        std::string to_string() const;
	};

    inline constexpr weekday weekday::sunday{ 0 };
    inline constexpr weekday weekday::monday{ 1 };
    inline constexpr weekday weekday::tuesday{ 2 };
    inline constexpr weekday weekday::wednesday{ 3 };
    inline constexpr weekday weekday::thursday{ 4 };
    inline constexpr weekday weekday::friday{ 5 };
    inline constexpr weekday weekday::saturday{ 6 };

    void shift_days(std::vector<weekday>& l, int64_t D);
    std::vector<weekday> shift_days(const std::vector<weekday>& l, int64_t D);

}

std::ostream& operator<<(std::ostream& out, const calendrical::weekday& wd);