#pragma once

#include "types.hpp"
#include "month.hpp"

namespace calendrical::datetypes{
    /**
     * @brief Gregorian calendar date.
     */
    struct gregorian_date{
        /** @brief Gregorian year. */
        int32_t year;
        /** @brief Gregorian month enum. */
        calendrical::month month;
        /** @brief Day of month. */
        int32_t day;

        /** @brief Default-initialized date. */
        constexpr gregorian_date() = default;
        /** @brief Constructs from Gregorian year/month/day components. */
        constexpr gregorian_date(int32_t year, calendrical::month month, int32_t day): year(year), month(month), day(day){}

        /** @brief Equality comparison on all components. */
        constexpr bool operator==(const gregorian_date &other) const = default;

        /** @brief Returns a debug-friendly string representation. */
        std::string to_string() const;

        virtual ~gregorian_date() = default;
    };
}
