#pragma once

#include "hebrew_based_calendar.hpp"
#include "moment.hpp"

namespace calendrical::calendars {

    using namespace calendrical::datetypes;

    class hebrew_calendar : public hebrew_based_calendar {

        hebrew_month get_month(const fixed_date_t& date, int32_t year) const;

        int64_t year_length_correction(int32_t year) const;
        int64_t elapsed_days(int32_t year) const;

        static const rational molad_part;
        static const rational molad_month_length;

        static const rational molad_1_tishri;

        const fixed_date_t m_possible_days_begin;

    public:

        hebrew_calendar();
        virtual ~hebrew_calendar() = default;

        virtual bool is_leap_year(int32_t year) const override;
        bool is_sabbatical_year(int32_t year) const;

        virtual fixed_date_t get_new_year(int32_t year) const override;
        virtual int32_t get_year(const fixed_date_t& date) const override;

        virtual hebrew_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const hebrew_date& date) const override;

        virtual hebrew_month get_month(const fixed_date_t& date) const override;

        virtual int32_t get_day(const fixed_date_t& date) const override;

        static rational_moment_t molad(int32_t year, hebrew_month month);

        static fixed_date_t fixed_from_molad(const rational_moment_t& moon);

        std::vector<weekday> possible_days(hebrew_month hmonth, int32_t hday) const;
    };
}