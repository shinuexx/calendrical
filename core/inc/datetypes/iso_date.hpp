#pragma once

#include "types.hpp"

namespace calendrical::datetypes {
    struct iso_date {
        int32_t year;
        int32_t week;
        int32_t day;

        constexpr iso_date() = default;
        constexpr iso_date(int32_t year, int32_t week, int32_t day): year(year), week(week), day(day) {}

        constexpr bool operator==(const iso_date& rhs) const = default;

        [[nodiscard]] std::string to_string() const;

        virtual ~iso_date() = default;
    };
}