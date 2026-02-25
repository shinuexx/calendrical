#pragma once

#include "chinese_based_calendar.hpp"

namespace calendrical::calendars {
    /**
     * @brief Chinese-based lunisolar calendar specialization for Vietnam.
     */
    class vietnamese_calendar :
        public chinese_based_calendar {

    public:
        /** @brief Constructs a Vietnamese calendar instance. */
        vietnamese_calendar() = default;
        ~vietnamese_calendar() override = default;

        /** @brief Returns the observation location used for calculations at moment @p t. */
        [[nodiscard]] location get_location(const moment_t& t) const override;
    };
}
