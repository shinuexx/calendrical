#pragma once

#include "datetypes/simple_date.hpp"
#include "location.hpp"

namespace calendrical::calendars {

    using namespace calendrical::datetypes;

    class french_based_calendar :
        public typed_calendar_if<simple_date, french_based_calendar>,
        public year_aware_calendar_if,
        public month_aware_calendar_if<int32_t>,
        public day_aware_calendar_if,
        public new_year_aware_calendar_if,
        public is_leap_year_aware_calendar_if
    {

    protected:
        static constexpr fixed_date_t ms_epoch{ 654415 };
        static constexpr location paris{angle_t::dms(48,50,11), angle_t::dms(2,20,15), 27, moment_t::hms(1)};

        int32_t get_month(const fixed_date_t& date, const fixed_date_t& new_year) const;
        int32_t get_day(const fixed_date_t& date, const fixed_date_t& new_year) const;

    public:
        french_based_calendar() = default;
        virtual ~french_based_calendar() = default;

        virtual fixed_date_t get_new_year(int32_t year) const override = 0;
        virtual int32_t get_year(const fixed_date_t& date) const override = 0;

        virtual fixed_date_t get_epoch() const override { return ms_epoch; }
        virtual calendar_type get_calendar_type() const override { return calendar_type::SOLAR; }

        virtual simple_date to_date(const fixed_date_t& date) const override = 0;
        virtual fixed_date_t to_fixed(const simple_date& date) const override = 0;

        virtual int32_t get_month(const fixed_date_t& date) const override = 0;

        virtual int32_t get_day(const fixed_date_t& date) const override = 0;

        virtual bool is_leap_year(int32_t year) const override = 0;

    };
}