#pragma once

#include "month.hpp"

namespace calendrical::datetypes {
    struct julian_date {
        int32_t year;
        calendrical::month month;
        int32_t day;

        constexpr julian_date() = default;
        constexpr julian_date(int32_t year, calendrical::month month, int32_t day): year(year), month(month), day(day) {}

        constexpr bool operator==(const julian_date &other) const = default;

        [[nodiscard]] std::string to_string() const;

        virtual ~julian_date() = default;
    };
}
