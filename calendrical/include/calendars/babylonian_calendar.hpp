#pragma once

#include "datetypes/simple_lunar_date.hpp"
#include "location.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    class babylonian_calendar :
        public typed_calendar_if<simple_lunar_date, babylonian_calendar>,
        public year_aware_calendar_if,
        public leap_month_aware_calendar_if<int32_t>,
        public day_aware_calendar_if,
        public is_leap_year_aware_calendar_if {

        static constexpr fixed_date_t ms_epoch{ -113502 };
        static constexpr location babylon{ 32.4794, 44.4328, 26, moment_t::hms(3.5) };

        static bool babylonian_criterion(const fixed_date_t& date);

        fixed_date_t new_month_on_or_before(const fixed_date_t& date) const;

    public:
        babylonian_calendar() = default;
        virtual ~babylonian_calendar() = default;

        virtual bool is_leap_year(int32_t year) const override;

        virtual fixed_date_t get_epoch() const override { return ms_epoch; }
        virtual calendar_type get_calendar_type() const override { return calendar_type::LUNAR; }

        virtual fixed_date_t to_fixed(const simple_lunar_date& date) const override;
        virtual simple_lunar_date to_date(const fixed_date_t& date) const override;

        virtual int32_t get_year(const fixed_date_t& date) const override;
        virtual int32_t get_month(const fixed_date_t& date) const override;
        virtual bool is_leap_month(const fixed_date_t& date) const override;
        virtual int32_t get_day(const fixed_date_t& date) const override;
    };
}