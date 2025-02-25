#pragma once

#include "month.hpp"
#include "roman_event.hpp"

namespace calendrical::datetypes {
    struct roman_date {
        int32_t year;
        calendrical::month month;
        roman_event event;
        int32_t count;
        bool leap;

        constexpr roman_date() = default;
        constexpr roman_date(int32_t year, calendrical::month month, roman_event event, int32_t count, bool leap) : year(year), month(month), event(event), count(count), leap(leap) {}

        constexpr bool operator==(const roman_date& other) const = default;
        [[nodiscard]] std::string to_string() const;

        virtual ~roman_date() = default;
    };

    struct olympiad {
        int32_t cycle, year;
        bool operator==(const olympiad & output) const = default;
    };

    std::string to_string(const olympiad& odate);
}

std::ostream& operator<<(std::ostream& out, const calendrical::datetypes::olympiad& odate);