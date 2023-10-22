#pragma once

#include "persian_based_calendar.hpp"

namespace calendrical::calendars {

    using namespace calendrical::datetypes;

    class arithmetic_persian_calendar :
        public persian_based_calendar {

    public:
        virtual fixed_date_t get_new_year(int32_t year) const override;
        virtual int32_t get_year(const fixed_date_t& date) const override;

        virtual fixed_date_t to_fixed(const simple_date& date) const override;
        virtual bool is_leap_year(int32_t year) const override;
    };
}