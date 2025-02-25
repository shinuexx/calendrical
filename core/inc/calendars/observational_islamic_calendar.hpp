#pragma once

#include "islamic_based_calendar.hpp"
#include "location.hpp"

namespace calendrical::calendars {
    class observational_islamic_calendar : public islamic_based_calendar {

        [[nodiscard]] fixed_date_t get_crescent(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_elapsed_months(const fixed_date_t& crescent) const;

        [[nodiscard]] int32_t get_year(int32_t elapsed_months) const;
        [[nodiscard]] int32_t get_month(int32_t elapsed_months) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date, const fixed_date_t& crescent) const;

    public:
        static constexpr location islamic_location{ 30.1, 31.3, 200, moment_t::hms(2) };

        observational_islamic_calendar() = default;
        ~observational_islamic_calendar() override = default;

        [[nodiscard]] int32_t get_year(const fixed_date_t &date) const override;

        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;

        [[nodiscard]] int32_t get_month(const fixed_date_t &date) const override;

        [[nodiscard]] int32_t get_day(const fixed_date_t &date) const override;

        [[nodiscard]] bool is_leap_year(int32_t year) const;
    };
}