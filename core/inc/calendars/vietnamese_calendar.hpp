#pragma once

#include "chinese_based_calendar.hpp"

namespace calendrical::calendars {
    class vietnamese_calendar :
        public chinese_based_calendar {

    public:
        vietnamese_calendar() = default;
        ~vietnamese_calendar() override = default;

        [[nodiscard]] location get_location(const moment_t& t) const override;
    };
}