#pragma once

#include "islamic_based_calendar.hpp"

namespace calendrical::calendars{
    class islamic_calendar : public islamic_based_calendar{
        [[nodiscard]] int32_t get_month(const fixed_date_t& date, int32_t year) const;

    public:
        islamic_calendar() = default;
        ~islamic_calendar() override = default;

        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const override;
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const override;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const override;

        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;

        [[nodiscard]] bool is_leap_year(int32_t year) const;
    };
}