#pragma once

#include "interfaces.hpp"

namespace calendrical::datetypes {
    struct lunar_date : public has_year_if, public has_leap_month_if<int32_t>, public has_leap_day_if, public date_if
    {
        constexpr lunar_date(int32_t year, int32_t month, bool leap_month, int32_t day, bool leap_day) : has_year_if{ year },
            has_leap_month_if{ month, leap_month },
            has_leap_day_if{ day, leap_day },
            date_if{} {}

        constexpr bool operator==(const lunar_date& other) const
        {
            return (year == other.year) && (month == other.month) && (leap_month == other.leap_month) && (day == other.day) && (leap_day == other.leap_day);
        }

        virtual std::string to_string() const override;
        virtual ~lunar_date() = default;

        constexpr auto tie() const { return std::tie(year, month, leap_month, day, leap_day); }
        constexpr auto tie() { return std::tie(year, month, leap_month, day, leap_day); }
        constexpr auto tuple() const { return std::make_tuple(year, month, leap_month, day, leap_day); }
    };
}