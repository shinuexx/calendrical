#pragma once

#include "datetypes/simple_date.hpp"
#include "julian_calendar.hpp"
#include "holiday_providers/julian_calendar_holiday_provider.hpp"
#include "location.hpp"

namespace calendrical::calendars{
    using simple_date = datetypes::simple_date;
    class samaritan_calendar : public typed_calendar_if<simple_date>{\
        [[nodiscard]] static moment_t noon(const fixed_date_t& date);
        [[nodiscard]] static fixed_date_t new_moon_after(const moment_t& t);
        [[nodiscard]] static fixed_date_t new_moon_at_or_before(const moment_t& t);

        [[nodiscard]] fixed_date_t new_year_on_or_before(const fixed_date_t& date) const;

        std::unique_ptr<const julian_calendar> m_julian;
        holiday_providers::julian_calendar_holiday_provider m_julian_provider;

    public:
        static constexpr location samaritan_location{ 32.1994, 35.2728, 881, moment_t::hms(2) };
        fixed_date_t m_epoch;

        samaritan_calendar();
        ~samaritan_calendar() override = default;

        [[nodiscard]]  int32_t get_year(const fixed_date_t& date) const;

        [[nodiscard]]  fixed_date_t epoch() const override { return m_epoch; }

        [[nodiscard]]  simple_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]]  fixed_date_t to_fixed(const simple_date& date) const override;

        [[nodiscard]]  int32_t get_month(const fixed_date_t& date) const;
        [[nodiscard]]  int32_t get_day(const fixed_date_t& date) const;
    };
}