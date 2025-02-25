#pragma once

#include "types.hpp"
#include "month.hpp"

namespace calendrical::datetypes{
    struct gregorian_date{
        int32_t year;
        calendrical::month month;
        int32_t day;

        constexpr gregorian_date() = default;
        constexpr gregorian_date(int32_t year, calendrical::month month, int32_t day): year(year), month(month), day(day){}

        constexpr bool operator==(const gregorian_date &other) const = default;

        std::string to_string() const;

        virtual ~gregorian_date() = default;
    };
}