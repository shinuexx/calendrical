#pragma once

#include "datetypes/simple_date.hpp"
#include "modern_hindu_base.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    class modern_hindu_solar_calendar :
        public typed_calendar_if<simple_date, modern_hindu_solar_calendar>,
        public year_aware_calendar_if,
        public month_aware_calendar_if<int32_t>,
        public day_aware_calendar_if,
        public modern_hindu_base
    {

    public:
        modern_hindu_solar_calendar() :
            typed_calendar_if<simple_date, modern_hindu_solar_calendar>{},
            year_aware_calendar_if{}, month_aware_calendar_if<int32_t>{},
            day_aware_calendar_if{},
            modern_hindu_base{}{}
        modern_hindu_solar_calendar(const rational_location& hindu_location) : 
            typed_calendar_if<simple_date, modern_hindu_solar_calendar>{}, 
            year_aware_calendar_if{}, month_aware_calendar_if<int32_t>{},
            day_aware_calendar_if{},
            modern_hindu_base{ hindu_location }{}
        virtual ~modern_hindu_solar_calendar() = default;

        virtual fixed_date_t get_epoch() const override { return m_epoch; }
        virtual calendar_type get_calendar_type() const override { return calendar_type::SOLAR; }

        virtual simple_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const simple_date& date) const override;

        virtual int32_t get_year(const fixed_date_t& date) const override;
        virtual int32_t get_month(const fixed_date_t& date) const override;
        virtual int32_t get_day(const fixed_date_t& date) const override;
    };
}