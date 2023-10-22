#pragma once

#include "../interfaces.hpp"
#include "hindu_base.hpp"
#include "datetypes/simple_lunar_date.hpp"

namespace calendrical::calendars {

    using namespace calendrical::datetypes;

    class old_hindu_lunar_calendar :
        public typed_calendar_if<simple_lunar_date, old_hindu_lunar_calendar>,
        public year_aware_calendar_if,
        public month_aware_calendar_if<int32_t>,
        public day_aware_calendar_if,
        public new_year_aware_calendar_if,
        public is_leap_year_aware_calendar_if,
        public hindu_base {

        rational get_sun(const fixed_date_t& date) const;
        rational get_new_moon(const rational& sun) const;
        int32_t get_month(const rational& new_moon) const;
        int32_t get_day(const rational& sun) const;
        int32_t get_year(const rational& new_moon) const;
        bool get_leap(const rational& new_moon) const;

    public:
        old_hindu_lunar_calendar() = default;
        virtual ~old_hindu_lunar_calendar() = default;

        virtual fixed_date_t get_new_year(int32_t year) const override;
        virtual int32_t get_year(const fixed_date_t& date) const override;

        virtual fixed_date_t get_epoch() const override { return m_epoch; }
        virtual calendar_type get_calendar_type() const override { return calendar_type::LUNISOLAR; }

        virtual simple_lunar_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const simple_lunar_date& date) const override;

        virtual int32_t get_month(const fixed_date_t& date) const override;
        virtual int32_t get_day(const fixed_date_t& date) const override;

        virtual bool is_leap_year(int32_t year) const override;
    };
}