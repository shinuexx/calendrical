#pragma once

#include "types.hpp"
#include "season.hpp"
#include "weekday.hpp"

namespace calendrical::datetypes{
    struct icelandic_date {
        int32_t year;
        calendrical::season season;
        int32_t week;
        calendrical::weekday weekday;

        constexpr icelandic_date() = default;
        constexpr icelandic_date(int32_t year, calendrical::season season, int32_t week, calendrical::weekday weekday) : year(year), season(season), week(week), weekday(weekday) {}

        constexpr bool operator==(const icelandic_date &other) const = default;

        [[nodiscard]] std::string to_string() const;

        virtual ~icelandic_date() = default;
    };
}