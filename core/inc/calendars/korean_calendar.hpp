#pragma once

#include "chinese_based_calendar.hpp"

namespace calendrical::calendars {
    /**
     * @brief Chinese-based lunisolar calendar specialization for Korea.
     */
    class korean_calendar :
        public chinese_based_calendar {

    public:
        /** @brief Constructs a Korean calendar instance. */
        korean_calendar() = default;
        ~korean_calendar() override = default;

        /** @brief Returns the observation location used for calculations at moment @p t. */
        [[nodiscard]] location get_location(const moment_t& t) const override;

        /** @brief Converts Chinese cycle/year numbering to Korean year numbering. */
        [[nodiscard]] static int32_t korean_year(int32_t cycle, int32_t year);
    };
}
