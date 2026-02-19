#pragma once

#include "french_based_calendar.hpp"

namespace calendrical::calendars{
    class french_calendar : public french_based_calendar {
    protected:
        using french_based_calendar::get_month;
        using french_based_calendar::get_day;

        [[nodiscard]] moment_t midnight_in_paris(const fixed_date_t& date) const;
        [[nodiscard]] fixed_date_t new_year_on_or_before(const fixed_date_t& date) const;

    public:
        french_calendar() = default;
        ~french_calendar() override = default;

        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const override;
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const override;

        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;

        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const override;

        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const override;

        [[nodiscard]] bool is_leap_year(int32_t year) const override;
    };
}