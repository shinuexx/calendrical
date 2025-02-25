#pragma once

#include "types.hpp"

namespace calendrical::datetypes {

    struct simple_date {
        int32_t year;
        int32_t month;
        int32_t day;

        constexpr simple_date() = default;
        constexpr simple_date(int32_t year, int32_t month, int32_t day) : year(year), month(month), day(day) {}

        constexpr bool operator==(const simple_date& other) const = default;
        std::string to_string() const;

        virtual ~simple_date() = default;
    };


}