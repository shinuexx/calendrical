#pragma once

#include "chinese_based_calendar.hpp"
#include "chinese_marriage_augury.hpp"

namespace calendrical::calendars {
    /**
     * @brief Chinese calendar specialization with China-specific location rules.
     */
    class chinese_calendar :
        public chinese_based_calendar{
    public:
        /** @brief Constructs a Chinese calendar instance. */
        chinese_calendar() = default;
        ~chinese_calendar() override = default;

        /** @brief Returns the observation location used for calculations at moment @p t. */
        [[nodiscard]] location get_location(const moment_t& t) const override;

        /** @brief Returns the marriage augury category for a Chinese year designation. */
        [[nodiscard]] chinese_marriage_augury year_marriage_augury(int32_t cycle, int32_t year) const;
    };
}
