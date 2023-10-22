#pragma once

#include "datetypes/simple_date.hpp"
#include "location.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;
    class persian_based_calendar :
        public typed_calendar_if<simple_date, persian_based_calendar>,
        public year_aware_calendar_if,
        public month_aware_calendar_if<int32_t>,
        public day_aware_calendar_if,
        public new_year_aware_calendar_if,
        public is_leap_year_aware_calendar_if{

    protected:
        static const fixed_date_t m_epoch;

        virtual int32_t get_month(int32_t day_of_year) const;
        virtual int32_t get_day_of_year(const fixed_date_t& date, int32_t year) const;
        virtual int32_t get_day(const fixed_date_t& date, int32_t year, int32_t month) const;

    public:
        static constexpr location tehran{ 35.68, 51.42, 1100, moment_t::hms(3.5) };

        persian_based_calendar() = default;
        virtual ~persian_based_calendar() = default;

        virtual fixed_date_t get_new_year(int32_t year) const override = 0;
        virtual int32_t get_year(const fixed_date_t& date) const override = 0;

        virtual fixed_date_t get_epoch() const override { return m_epoch; }
        virtual calendar_type get_calendar_type() const override { return calendar_type::SOLAR; }

        virtual simple_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const simple_date& date) const override = 0;

        virtual int32_t get_month(const fixed_date_t& date) const override;

        virtual int32_t get_day(const fixed_date_t& date) const override;

        virtual bool is_leap_year(int32_t year) const override;
    };
}