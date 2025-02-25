#pragma once

#include "types.hpp"
#include "hebrew_month.hpp"

namespace calendrical::datetypes {
    struct hebrew_date{
        int32_t year;
        hebrew_month month;
        int32_t day;

        constexpr hebrew_date() = default;
        constexpr hebrew_date(int32_t year, hebrew_month month, int32_t day) : year(year), month(month), day(day) {}

        constexpr bool operator==(const hebrew_date &other) const = default;

        [[nodiscard]] std::string to_string() const;

        virtual ~hebrew_date() = default;
    };
}