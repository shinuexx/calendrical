#pragma once

#include "islamic_based_calendar.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    class islamic_calendar :
        public islamic_based_calendar,
        public is_leap_year_aware_calendar_if {

        int32_t get_month(const fixed_date_t& date, int32_t year) const;

    public:
        islamic_calendar() = default;
        virtual ~islamic_calendar() = default;

        virtual int32_t get_year(const fixed_date_t& date) const override;

        virtual simple_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const simple_date& date) const override;

        virtual int32_t get_month(const fixed_date_t& date) const override;

        virtual int32_t get_day(const fixed_date_t& date) const override {
            return to_date(date).day;
        }

        virtual bool is_leap_year(int32_t year) const override;
    };
}