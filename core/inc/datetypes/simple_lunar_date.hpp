#pragma once

#include "types.hpp"

namespace calendrical::datetypes {

    /**
     * @brief Lunar date with leap-month flag and no leap-day flag.
     */
    struct simple_lunar_date {
        /** @brief Year component. */
        int32_t year;
        /** @brief Month number. */
        int32_t month;
        /** @brief True if the month is an intercalary (leap) month. */
        bool leap_month;
        /** @brief Day of month. */
        int32_t day;

        /** @brief Default-initialized date. */
        constexpr simple_lunar_date() = default;
        /** @brief Constructs from year/month/leap-month/day components. */
        constexpr simple_lunar_date(int32_t year, int32_t month, bool leap_month, int32_t day) : year(year), month(month), leap_month(leap_month), day(day) {}

        /** @brief Equality comparison on all components. */
        constexpr bool operator==(const simple_lunar_date& other) const = default;
        /** @brief Returns a debug-friendly string representation. */
        std::string to_string() const;

        virtual ~simple_lunar_date() = default;
    };


}
