#pragma once

#include "datetypes/hebrew_date.hpp"

namespace calendrical::calendars {
    
    using namespace calendrical::datetypes;

    class hebrew_based_calendar :
        public typed_calendar_if<hebrew_date, hebrew_based_calendar>,
        public year_aware_calendar_if,
        public month_aware_calendar_if<hebrew_month>,
        public day_aware_calendar_if,
        public new_year_aware_calendar_if,
        public is_leap_year_aware_calendar_if {

    protected:
        static constexpr fixed_date_t m_epoch{ -1373427 };

    public:

        hebrew_based_calendar() = default;
        virtual ~hebrew_based_calendar() = default;

        virtual fixed_date_t get_new_year(int32_t year) const override = 0;
        virtual int32_t get_year(const fixed_date_t& date) const override = 0;

        virtual fixed_date_t get_epoch() const override { return m_epoch; }
        virtual calendar_type get_calendar_type() const override { return calendar_type::SOLAR; }

        virtual hebrew_date to_date(const fixed_date_t& date) const override = 0;
        virtual fixed_date_t to_fixed(const hebrew_date& date) const override = 0;

        virtual hebrew_month get_month(const fixed_date_t& date) const override = 0;

        virtual int32_t get_day(const fixed_date_t& date) const override = 0;

        virtual int32_t last_day_of_month(int32_t year, hebrew_month hm) const;
        virtual bool is_short_kislev(int32_t year) const;
        virtual bool is_long_marheshvan(int32_t year) const;
        virtual int32_t days_in_year(int32_t year) const;
        
        virtual bool is_leap_year(int32_t year) const override = 0;

        virtual hebrew_month last_month_of_year(int32_t year) const;
    };
}