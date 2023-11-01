#pragma once

#include <cstdint>
#include <compare>
#include <string>
#include <iostream>
#include "weekday.hpp"
#include "helpers.hpp"

namespace calendrical {
    class fixed_date_t {
        int64_t m_v;

    public:
        constexpr fixed_date_t(int64_t v) : m_v{ v } {}
        constexpr fixed_date_t() : m_v{} {}

        constexpr operator int64_t() const { return m_v; }

        constexpr bool operator==(const fixed_date_t& other) const { return m_v == other.m_v; }
        constexpr auto operator<=>(const fixed_date_t& other) const { return m_v <=> other.m_v; }

        constexpr weekday get_weekday() const { return weekday{ static_cast<int>(mod(m_v, 7)) }; }

        static constexpr fixed_date_t of(int64_t value) { return { value }; }

        constexpr fixed_date_t add(int64_t value) const { return { m_v + value }; }
        constexpr fixed_date_t subtract(int64_t value) const { return { m_v - value }; }

        friend std::string to_string(const fixed_date_t& rd);

        static const fixed_date_t end_of_time;
    };

    inline constexpr fixed_date_t fixed_date_t::end_of_time{ std::numeric_limits<int64_t>::max() };
}

std::ostream& operator<<(std::ostream& out, const calendrical::fixed_date_t& rd);

constexpr calendrical::fixed_date_t operator"" _rd(unsigned long long x)
{
    return calendrical::fixed_date_t{ static_cast<int64_t>(x) };
}