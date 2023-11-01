#pragma once

#include "../datetypes/simple_date.hpp"
#include "julian_calendar.hpp"
#include "../holiday_providers/julian_calendar_holiday_provider.hpp"
#include "location.hpp"

namespace calendrical::calendars {

    using namespace calendrical::datetypes;

    class samaritan_calendar :
        public typed_calendar_if<simple_date, samaritan_calendar>,
        public year_aware_calendar_if,
        public month_aware_calendar_if<int32_t>,
        public day_aware_calendar_if {

        static moment_t noon(const fixed_date_t& date);
        static fixed_date_t new_moon_after(const moment_t& t);
        static fixed_date_t new_moon_at_or_before(const moment_t& t);

        fixed_date_t new_year_on_or_before(const fixed_date_t& date) const;

        std::shared_ptr<const julian_calendar> m_julian;
        holiday_providers::julian_calendar_holiday_provider m_julian_provider;

    public:
        static constexpr location samaritan_location{ 32.1994, 35.2728, 881, moment_t::hms(2) };
        static constexpr fixed_date_t ms_epoch{-598573};

        samaritan_calendar() : m_julian{ new julian_calendar() }, m_julian_provider{} {}
        virtual ~samaritan_calendar() = default;

        virtual int32_t get_year(const fixed_date_t& date) const override;

        virtual fixed_date_t get_epoch() const override { return ms_epoch; }
        virtual calendar_type get_calendar_type() const override { return calendar_type::LUNAR; }

        virtual simple_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const simple_date& date) const override;

        virtual int32_t get_month(const fixed_date_t& date) const override;
        virtual int32_t get_day(const fixed_date_t& date) const override;
    };
}