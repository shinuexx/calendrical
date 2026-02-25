#pragma once

#include "chinese_based_calendar.hpp"

namespace calendrical::calendars{
    /**
     * @brief Chinese-based lunisolar calendar specialization for Japan.
     */
    class japanese_calendar : public chinese_based_calendar{
    public:
        /** @brief Constructs a Japanese calendar instance. */
        japanese_calendar() = default;
        ~japanese_calendar() override = default;

        /** @brief Returns the observation location used for calculations at moment @p t. */
        [[nodiscard]] location get_location(const moment_t& t) const override;
    };
}
