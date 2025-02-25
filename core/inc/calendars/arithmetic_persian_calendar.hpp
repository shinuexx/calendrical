#pragma once

#include "persian_based_calendar.hpp"

namespace calendrical::calendars {
    class arithmetic_persian_calendar final : public persian_based_calendar {
    public:
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const override;
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const override;

        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;
        [[nodiscard]] bool is_leap_year(int32_t year) const override;
    };
}