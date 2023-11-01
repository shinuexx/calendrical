#pragma once

#include "chinese_based_calendar.hpp"

namespace calendrical::calendars {
    class korean_calendar :
        public chinese_based_calendar {

    public:
        korean_calendar() = default;
        virtual ~korean_calendar() = default;

        virtual location get_location(const moment_t& t) const override;

        static int32_t korean_year(int32_t cycle, int32_t year);
    };
}