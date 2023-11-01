#pragma once

#include "chinese_based_calendar.hpp"

namespace calendrical::calendars {
    class japanese_calendar :
        public chinese_based_calendar {

    public:
        japanese_calendar() = default;
        virtual ~japanese_calendar() = default;

        virtual location get_location(const moment_t& t) const override;
    };
}