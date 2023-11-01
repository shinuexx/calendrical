#pragma once

#include "chinese_base.hpp"
#include "datetypes/chinese_date.hpp"
#include "datetypes/sexagesimal_name.hpp"

namespace calendrical::calendars {

    using namespace calendrical::datetypes;

    class chinese_based_calendar :
        public typed_calendar_if<chinese_date, chinese_based_calendar>,
        public cycle_aware_calendar_if,
        public year_aware_calendar_if,
        public leap_month_aware_calendar_if<int32_t>,
        public day_aware_calendar_if,
        public chinese_base
    {
        fixed_date_t m_epoch;        
    public:
        chinese_based_calendar(const fixed_date_t& epoch) : m_epoch{ epoch } {}
        chinese_based_calendar();

        virtual ~chinese_based_calendar() = 0;

        virtual fixed_date_t get_epoch() const override { return m_epoch; }
        virtual calendar_type get_calendar_type() const override { return calendar_type::SOLAR; }

        virtual location get_location(const moment_t& t) const override = 0;
       
        virtual chinese_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const chinese_date& date) const override;

        virtual int32_t get_cycle(const fixed_date_t& date) const override;
        virtual int32_t get_year(const fixed_date_t& date) const override;
        virtual int32_t get_month(const fixed_date_t& date) const override;
        virtual bool is_leap_month(const fixed_date_t& date) const override;
        virtual int32_t get_day(const fixed_date_t& date) const override;

        static sexagesimal_name year_name(int32_t n);

        static constexpr int32_t month_name_epoch = 57;

        static sexagesimal_name month_name(int32_t month, int32_t year);

        static constexpr fixed_date_t day_name_epoch{ 45 };

        static sexagesimal_name day_name(const fixed_date_t& date);

        static const sexagesimal_name day_name_zero;
        
        static fixed_date_t day_name_on_or_before(const sexagesimal_name& name, const fixed_date_t& date);
    };
}