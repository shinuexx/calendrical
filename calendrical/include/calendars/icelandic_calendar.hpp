#pragma once

#include "../datetypes/icelandic_date.hpp"

namespace calendrical::calendars {

    using namespace calendrical::datetypes;

    class icelandic_calendar :
        public typed_calendar_if<icelandic_date, icelandic_calendar>,
        public year_aware_calendar_if,
        public month_aware_calendar_if<int32_t>,
        public week_aware_calendar_if,
        public new_year_aware_calendar_if,
        public is_leap_year_aware_calendar_if {

        fixed_date_t m_epoch;

        static constexpr std::array<int64_t, 4> a{ 97,24,1,0 };

        season get_season(const fixed_date_t& date, int32_t year) const;

        int32_t get_week(const fixed_date_t& date, int32_t year, season s) const;
        int32_t get_month(const icelandic_date& odate, const fixed_date_t& date) const;

    public:
        icelandic_calendar();

        virtual ~icelandic_calendar() = default;

        fixed_date_t summer(int32_t iyear) const;
        fixed_date_t winter(int32_t iyear) const;

        virtual fixed_date_t get_new_year(int32_t year) const override;
        virtual int32_t get_year(const fixed_date_t& date) const override;

        virtual fixed_date_t get_epoch() const override { return m_epoch; }
        virtual calendar_type get_calendar_type() const override { return calendar_type::SOLAR; }

        virtual icelandic_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const icelandic_date& date) const override;

        virtual int32_t get_month(const fixed_date_t& date) const override;
        int32_t get_month(const icelandic_date& date) const;

        virtual bool is_leap_year(int32_t year) const override;

        season get_season(const fixed_date_t& date) const;
        virtual int32_t get_week(const fixed_date_t& date) const;
    };
}