#pragma once

#include "french_based_calendar.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    class french_calendar :
        public french_based_calendar {

        using french_based_calendar::get_month;
        using french_based_calendar::get_day;
        moment_t midnight_in_paris(const fixed_date_t& date) const;
        fixed_date_t new_year_on_or_before(const fixed_date_t& date) const;
            
    public:
        french_calendar() = default;
        virtual ~french_calendar() = default;

        virtual fixed_date_t get_new_year(int32_t year) const override;
        virtual int32_t get_year(const fixed_date_t& date) const override;

        virtual simple_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const simple_date& date) const override;

        virtual int32_t get_month(const fixed_date_t& date) const override;

        virtual int32_t get_day(const fixed_date_t& date) const override;

        virtual bool is_leap_year(int32_t year) const override;
    };
}