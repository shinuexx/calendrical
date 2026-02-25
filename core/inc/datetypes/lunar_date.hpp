#pragma once

#include "types.hpp"

namespace calendrical::datetypes {
    /**
     * @brief Lunar date with leap-month and leap-day flags.
     */
    struct lunar_date {
        /** @brief Year component. */
        int32_t year;
        /** @brief Month number. */
        int32_t month;
        /** @brief True if the month is an intercalary (leap) month. */
        bool leap_month;
        /** @brief Day of month. */
        int32_t day;
        /** @brief True if the day is a leap/intercalary day. */
        bool leap_day;

        /** @brief Default-initialized date. */
        constexpr lunar_date() = default;
        /** @brief Constructs from year/month/leap-month/day/leap-day components. */
        constexpr lunar_date(int32_t year, int32_t month, bool leap_month, int32_t day, bool leap_day): year(year), month(month), leap_month(leap_month), day(day), leap_day(leap_day) {}

        /** @brief Equality comparison on all components. */
        constexpr bool operator==(const lunar_date &other) const = default;

        /** @brief Returns a debug-friendly string representation. */
        [[nodiscard]] std::string to_string() const;

        virtual ~lunar_date() = default;
    };
}
