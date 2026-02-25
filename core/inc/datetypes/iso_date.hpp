#pragma once

#include "types.hpp"

namespace calendrical::datetypes {
    /**
     * @brief ISO week date (`year-week-day`) representation.
     */
    struct iso_date {
        /** @brief ISO week-numbering year. */
        int32_t year;
        /** @brief ISO week number. */
        int32_t week;
        /** @brief ISO weekday number (calendar-specific convention in implementation). */
        int32_t day;

        /** @brief Default-initialized date. */
        constexpr iso_date() = default;
        /** @brief Constructs from ISO week-date components. */
        constexpr iso_date(int32_t year, int32_t week, int32_t day): year(year), week(week), day(day) {}

        /** @brief Equality comparison on all components. */
        constexpr bool operator==(const iso_date& rhs) const = default;

        /** @brief Returns a debug-friendly string representation. */
        [[nodiscard]] std::string to_string() const;

        virtual ~iso_date() = default;
    };
}
