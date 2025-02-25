#pragma once

#include "datetypes/julian_date.hpp"
#include "types.hpp"
#include "fixed_date.hpp"

namespace calendrical::calendars{
    using julian_date = datetypes::julian_date;

    class julian_calendar final :
    public typed_calendar_if<julian_date>{

        static constexpr fixed_date_t ms_epoch{-1};

        [[nodiscard]] month get_month(const fixed_date_t& date, int32_t year) const;

    public:
        constexpr julian_calendar() = default;

        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
        [[nodiscard]] bool is_leap_year(int32_t year) const;

        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }
        [[nodiscard]] julian_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const julian_date& date) const override;

        [[nodiscard]] month get_month(const fixed_date_t& date) const;
    };
}