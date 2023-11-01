#pragma once

#include "islamic_based_calendar.hpp"
#include "location.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    class saudi_islamic_calendar :
        public islamic_based_calendar,
        public is_leap_year_aware_calendar_if {

        static bool saudi_criterion(const fixed_date_t& date);

        static fixed_date_t new_month_on_or_before(const fixed_date_t& date);

        fixed_date_t get_crescent(const fixed_date_t& date) const;
        int32_t get_elapsed_months(const fixed_date_t& crescent) const;

        int32_t get_year(int32_t elapsed_months) const;
        int32_t get_month(int32_t elapsed_months) const;
        int32_t get_day(const fixed_date_t& date, const fixed_date_t& crescent) const;

    public:
        static constexpr location islamic_location{ 30.1, 31.3, 200, moment_t::hms(2) };

        saudi_islamic_calendar() = default;
        virtual ~saudi_islamic_calendar() = default;

        virtual int32_t get_year(const fixed_date_t& date) const override;

        virtual simple_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const simple_date& date) const override;

        virtual int32_t get_month(const fixed_date_t& date) const override;

        virtual int32_t get_day(const fixed_date_t& date) const override;

        virtual bool is_leap_year(int32_t year) const override;
    };
}