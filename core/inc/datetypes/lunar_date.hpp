#pragma once

#include "types.hpp"

namespace calendrical::datetypes {
    struct lunar_date {
        int32_t year;
        int32_t month;
        bool leap_month;
        int32_t day;
        bool leap_day;

        constexpr lunar_date() = default;
        constexpr lunar_date(int32_t year, int32_t month, bool leap_month, int32_t day, bool leap_day): year(year), month(month), leap_month(leap_month), day(day), leap_day(leap_day) {}

        constexpr bool operator==(const lunar_date &other) const = default;

        [[nodiscard]] std::string to_string() const;

        virtual ~lunar_date() = default;
    };
}
