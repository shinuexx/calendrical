#pragma once

#include "datetypes/lunar_date.hpp"
#include "modern_hindu_base.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    class astro_hindu_lunar_calendar :
        public typed_calendar_if<lunar_date, astro_hindu_lunar_calendar>,
        public year_aware_calendar_if,
        public leap_month_aware_calendar_if<int32_t>,
        public leap_day_aware_calendar_if,
        public modern_hindu_base
    {

    public:
        astro_hindu_lunar_calendar() :
            typed_calendar_if<lunar_date, astro_hindu_lunar_calendar>{},
            year_aware_calendar_if{}, leap_month_aware_calendar_if<int32_t>{},
            leap_day_aware_calendar_if{},
            modern_hindu_base{}{}
        astro_hindu_lunar_calendar(const rational_location& hindu_location) :
            typed_calendar_if<lunar_date, astro_hindu_lunar_calendar>{},
            year_aware_calendar_if{}, leap_month_aware_calendar_if<int32_t>{},
            leap_day_aware_calendar_if{},
            modern_hindu_base{ hindu_location }{}
        virtual ~astro_hindu_lunar_calendar() = default;

        virtual fixed_date_t get_epoch() const override { return m_epoch; }
        virtual calendar_type get_calendar_type() const override { return calendar_type::LUNAR; }

        virtual lunar_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const lunar_date& date) const override;

        virtual int32_t get_year(const fixed_date_t& date) const override;
        virtual int32_t get_month(const fixed_date_t& date) const override;
        virtual int32_t get_day(const fixed_date_t& date) const override;

        virtual bool is_leap_month(const fixed_date_t& date) const override;
        virtual bool is_leap_day(const fixed_date_t& date) const override;
    };
}