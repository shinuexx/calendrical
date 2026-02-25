#pragma once

#include "types.hpp"
#include "season.hpp"
#include "weekday.hpp"

namespace calendrical::datetypes{
    /**
     * @brief Icelandic calendar date expressed by season, week and weekday.
     */
    struct icelandic_date {
        /** @brief Icelandic calendar year. */
        int32_t year;
        /** @brief Half-year season marker. */
        calendrical::season season;
        /** @brief Week number within the season/year context. */
        int32_t week;
        /** @brief Weekday within the Icelandic calendar date. */
        calendrical::weekday weekday;

        /** @brief Default-initialized date. */
        constexpr icelandic_date() = default;
        /** @brief Constructs from Icelandic date components. */
        constexpr icelandic_date(int32_t year, calendrical::season season, int32_t week, calendrical::weekday weekday) : year(year), season(season), week(week), weekday(weekday) {}

        /** @brief Equality comparison on all components. */
        constexpr bool operator==(const icelandic_date &other) const = default;

        /** @brief Returns a debug-friendly string representation. */
        [[nodiscard]] std::string to_string() const;

        virtual ~icelandic_date() = default;
    };
}
