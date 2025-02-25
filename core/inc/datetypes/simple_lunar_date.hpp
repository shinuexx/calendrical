#pragma once

#include "types.hpp"

namespace calendrical::datetypes {

    struct simple_lunar_date {
        int32_t year;
        int32_t month;
        bool leap_month;
        int32_t day;

        constexpr simple_lunar_date() = default;
        constexpr simple_lunar_date(int32_t year, int32_t month, bool leap_month, int32_t day) : year(year), month(month), leap_month(leap_month), day(day) {}

        constexpr bool operator==(const simple_lunar_date& other) const = default;
        std::string to_string() const;

        virtual ~simple_lunar_date() = default;
    };


}