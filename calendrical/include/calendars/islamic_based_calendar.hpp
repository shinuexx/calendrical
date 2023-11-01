#pragma once

#include "datetypes/simple_date.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;
    class islamic_based_calendar :
        public typed_calendar_if<simple_date, islamic_based_calendar>,
        public year_aware_calendar_if,
        public month_aware_calendar_if<int32_t>,
        public day_aware_calendar_if,
        public new_year_aware_calendar_if {

    protected:
        static constexpr fixed_date_t m_epoch{ 227015 };

    public:
        islamic_based_calendar() = default;
        virtual ~islamic_based_calendar() = default;

        virtual fixed_date_t get_new_year(int32_t year) const override { return to_fixed({ year, 1, 1 }); };
        virtual int32_t get_year(const fixed_date_t& date) const override = 0;

        virtual fixed_date_t get_epoch() const override { return m_epoch; }
        virtual calendar_type get_calendar_type() const override { return calendar_type::LUNAR; }

        virtual int32_t next_year(int32_t year) const override { return year == -1 ? 1 : year + 1; }
        virtual int32_t previous_year(int32_t year) const override { return year == 1 ? -1 : year - 1; }

        virtual simple_date to_date(const fixed_date_t& date) const override = 0;
        virtual fixed_date_t to_fixed(const simple_date& date) const override = 0;

        virtual int32_t get_month(const fixed_date_t& date) const override = 0;

        virtual int32_t get_day(const fixed_date_t& date) const override = 0;
    };
}