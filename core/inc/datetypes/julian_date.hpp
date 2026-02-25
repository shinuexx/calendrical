#pragma once

#include "month.hpp"

namespace calendrical::datetypes {
    /**
     * @brief Julian calendar date.
     */
    struct julian_date {
        /** @brief Julian year. */
        int32_t year;
        /** @brief Julian month enum. */
        calendrical::month month;
        /** @brief Day of month. */
        int32_t day;

        /** @brief Default-initialized date. */
        constexpr julian_date() = default;
        /** @brief Constructs from Julian year/month/day components. */
        constexpr julian_date(int32_t year, calendrical::month month, int32_t day): year(year), month(month), day(day) {}

        /** @brief Equality comparison on all components. */
        constexpr bool operator==(const julian_date &other) const = default;

        /** @brief Returns a debug-friendly string representation. */
        [[nodiscard]] std::string to_string() const;

        virtual ~julian_date() = default;
    };
}
