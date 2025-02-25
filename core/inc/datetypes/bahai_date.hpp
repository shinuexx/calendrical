#pragma once

#include "types.hpp"

namespace calendrical::datetypes {
    struct bahai_date {
        int32_t major;
        int32_t cycle;
        int32_t year;
        int32_t month;
        int32_t day;

        constexpr bahai_date() = default;
        constexpr bahai_date(int32_t major, int32_t cycle, int32_t year, int32_t month, int32_t day): major(major), cycle(cycle), year(year), month(month), day(day) {}

        [[nodiscard]] constexpr bool operator==(const bahai_date &other) const = default;

        [[nodiscard]] std::string to_string() const;

        virtual ~bahai_date() = default;
    };
}