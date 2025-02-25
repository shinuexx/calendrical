#pragma once

#include "types.hpp"
#include "hindu_base.hpp"
#include "datetypes/simple_date.hpp"

namespace calendrical::calendars {
    using simple_date = datetypes::simple_date;

    class old_hindu_solar_calendar final :
        public typed_calendar_if<simple_date>,
        public hindu_base
    {
        [[nodiscard]] rational get_sun(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_year(const rational& sun) const;
        [[nodiscard]] int32_t get_month(const rational& sun) const;
        [[nodiscard]] int32_t get_day(const rational& sun) const;

    public:
        old_hindu_solar_calendar() = default;
        ~old_hindu_solar_calendar() override = default;

        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;

        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;

        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
    };
}