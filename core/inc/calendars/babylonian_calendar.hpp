#pragma once

#include "datetypes/simple_lunar_date.hpp"
#include "location.hpp"

namespace calendrical::calendars {
    using simple_lunar_date = datetypes::simple_lunar_date;
    class babylonian_calendar final :
        public typed_calendar_if<simple_lunar_date> {

        static constexpr fixed_date_t ms_epoch { -113502 };
        static constexpr location babylon{ 32.4794, 44.4328, 26, moment_t::hms(3.5) };

        static bool babylonian_criterion(const fixed_date_t& date);

        fixed_date_t new_month_on_or_before(const fixed_date_t& date) const;

    public:
        babylonian_calendar() = default;
        ~babylonian_calendar() override = default;

        [[nodiscard]] bool is_leap_year(int32_t year) const;

        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        [[nodiscard]] fixed_date_t to_fixed(const simple_lunar_date& date) const override;
        [[nodiscard]] simple_lunar_date to_date(const fixed_date_t& date) const override;

        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        [[nodiscard]] bool is_leap_month(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
    };
}