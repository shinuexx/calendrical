#pragma once

#include "types.hpp"
#include "hebrew_month.hpp"

namespace calendrical::datetypes {
    /**
     * @brief Hebrew calendar date.
     */
    struct hebrew_date{
        /** @brief Hebrew year. */
        int32_t year;
        /** @brief Hebrew month enum. */
        hebrew_month month;
        /** @brief Day of month. */
        int32_t day;

        /** @brief Default-initialized date. */
        constexpr hebrew_date() = default;
        /** @brief Constructs from Hebrew year/month/day components. */
        constexpr hebrew_date(int32_t year, hebrew_month month, int32_t day) : year(year), month(month), day(day) {}

        /** @brief Equality comparison on all components. */
        constexpr bool operator==(const hebrew_date &other) const = default;

        /** @brief Returns a debug-friendly string representation. */
        [[nodiscard]] std::string to_string() const;

        virtual ~hebrew_date() = default;
    };
}
