#pragma once

#include "chinese_based_calendar.hpp"
#include "chinese_marriage_augury.hpp"

namespace calendrical::calendars {
    class chinese_calendar :
        public chinese_based_calendar {

    public:
        chinese_calendar() = default;
        virtual ~chinese_calendar() = default;

        virtual location get_location(const moment_t& t) const override;

        chinese_marriage_augury year_marriage_augury(int32_t cycle, int32_t year) const;
    };
}