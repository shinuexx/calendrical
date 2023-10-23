#pragma once
#include "gregorian_calendar.hpp"
#include "datetypes/bahai_date.hpp"

namespace calendrical::calendars {

    using namespace calendrical::datetypes;

    class bahai_based_calendar :
        public typed_calendar_if<bahai_date, bahai_based_calendar>,
        public major_aware_calendar_if,
        public cycle_aware_calendar_if,
        public year_aware_calendar_if,
        public month_aware_calendar_if<int32_t>,
        public day_aware_calendar_if
    {
    protected:
        static constexpr int32_t ms_gregorian_year_bahai_epoch = 1844;

        static constexpr fixed_date_t ms_epoch{ 673222 };

        std::shared_ptr<const gregorian_calendar> m_gregorian;

        virtual int32_t get_years(const fixed_date_t& date) const = 0;
        virtual int32_t get_major(int32_t years) const;
        virtual int32_t get_cycle(int32_t years) const;
        virtual int32_t get_year(int32_t years) const;
        virtual int32_t get_month(const fixed_date_t& date, int32_t major, int32_t cycle, int32_t year) const;
        virtual int32_t get_days(const fixed_date_t& date, const fixed_date_t& new_year) const;
        virtual int32_t get_day(const fixed_date_t& date, int32_t major, int32_t cycle, int32_t year, int32_t month) const;

    public:
        static constexpr int32_t ayyam_i_ha{ 0 };

        bahai_based_calendar();
        virtual ~bahai_based_calendar() = default;

        virtual int32_t get_major(const fixed_date_t& date) const override;
        virtual int32_t get_cycle(const fixed_date_t& date) const override;
        virtual int32_t get_year(const fixed_date_t& date) const override;
        virtual int32_t get_month(const fixed_date_t& date) const override;
        virtual int32_t get_day(const fixed_date_t& date) const override;

        virtual fixed_date_t get_epoch() const override { return ms_epoch; }
        virtual calendar_type get_calendar_type() const override { return calendar_type::SOLAR; }

        virtual bahai_date to_date(const fixed_date_t& date) const override = 0;
        virtual fixed_date_t to_fixed(const bahai_date& date) const override = 0;
    };
}