#pragma once

#include "datetypes/lunar_date.hpp"
#include "angle.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    class tibetan_calendar : 
        public typed_calendar_if<lunar_date, tibetan_calendar>, 
        public year_aware_calendar_if,
        public leap_month_aware_calendar_if<int32_t>,
        public leap_day_aware_calendar_if
    {
        static const fixed_date_t ms_epoch;

        static rational_angle_t tibetan_sun_equation(const rational_angle_t& a);
        static rational_angle_t tibetan_moon_equation(const rational_angle_t& a);

    public:

        tibetan_calendar() = default;
        virtual ~tibetan_calendar() = default;

        virtual fixed_date_t get_epoch() const override { return ms_epoch; }
        virtual calendar_type get_calendar_type() const override { return calendar_type::LUNAR; }

        virtual lunar_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const lunar_date& date) const override;

        virtual int32_t get_month(const fixed_date_t& date) const override;
        virtual bool is_leap_month(const fixed_date_t& date) const override;
        virtual int32_t get_day(const fixed_date_t& date) const override;
        virtual bool is_leap_day(const fixed_date_t& date) const override;
        virtual int32_t get_year(const fixed_date_t& date) const override;

        bool is_leap_month(int32_t year, int32_t month) const;
        bool is_leap_day(int32_t year, int32_t month, int32_t day) const;

        fixed_date_t losar(int32_t tyear) const;

        std::list<fixed_date_t> new_year(int32_t gyear) const;
    };
}