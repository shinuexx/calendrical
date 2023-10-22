#pragma once

#include "interfaces.hpp"

namespace calendrical::datetypes {
    struct simple_lunar_date :
        public has_year_if,
        public has_leap_month_if<int32_t>,
        public has_day_if,
        public date_if {

        constexpr simple_lunar_date(int32_t year, int32_t month, bool leap_month, int32_t day) :
            has_year_if{ year },
            has_leap_month_if{ month, leap_month },
            has_day_if{ day },
            date_if{} {

        }

        constexpr bool operator==(const simple_lunar_date& other) const {
            return (year == other.year)
                && (month == other.month)
                && (leap_month == other.leap_month)
                && (day == other.day);
        }

        virtual std::string to_string() const override;
        virtual ~simple_lunar_date() = default;

        constexpr auto tie() const { return std::tie(year, month, leap_month, day); }
        constexpr auto tie() { return std::tie(year, month, leap_month, day); }
        constexpr auto tuple() const { return std::make_tuple(year, month, leap_month, day); }
    };
}