#pragma once

#include "types.hpp"

namespace calendrical::datetypes {

    /**
     * @brief Simple Y-M-D date using integer month numbering.
     */
    struct simple_date {
        /** @brief Year component. */
        int32_t year;
        /** @brief Month component (typically 1..12 depending on calendar usage). */
        int32_t month;
        /** @brief Day-of-month component. */
        int32_t day;

        /** @brief Default-initialized date. */
        constexpr simple_date() = default;
        /** @brief Constructs from year, month and day components. */
        constexpr simple_date(int32_t year, int32_t month, int32_t day) : year(year), month(month), day(day) {}

        /** @brief Equality comparison on all components. */
        constexpr bool operator==(const simple_date& other) const = default;
        /** @brief Returns a debug-friendly string representation. */
        std::string to_string() const;

        virtual ~simple_date() = default;
    };


}
