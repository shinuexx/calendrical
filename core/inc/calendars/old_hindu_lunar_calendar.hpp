#pragma once

#include "types.hpp"
#include "hindu_base.hpp"
#include "datetypes/simple_lunar_date.hpp"

namespace calendrical::calendars {
    using simple_lunar_date = datetypes::simple_lunar_date;

    class old_hindu_lunar_calendar :
        public typed_calendar_if<simple_lunar_date>,
        public hindu_base {

        [[nodiscard]] double get_sun(const fixed_date_t& date) const;
        [[nodiscard]] double get_new_moon(const double& sun) const;
        [[nodiscard]] int32_t get_month(const double& new_moon) const;
        [[nodiscard]] int32_t get_day(const double& sun) const;
        [[nodiscard]] int32_t get_year(const double& new_moon) const;
        [[nodiscard]] bool get_leap(const double& new_moon) const;

    public:
        old_hindu_lunar_calendar() = default;
        ~old_hindu_lunar_calendar() override = default;

        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;

        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        [[nodiscard]] simple_lunar_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const simple_lunar_date& date) const override;

        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;

        [[nodiscard]] bool is_leap_year(int32_t year) const;
    };
}