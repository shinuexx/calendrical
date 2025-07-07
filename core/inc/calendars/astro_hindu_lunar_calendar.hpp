#pragma once

#include "datetypes/lunar_date.hpp"
#include "modern_hindu_base.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    class astro_hindu_lunar_calendar final :
        public typed_calendar_if<lunar_date>,
        public modern_hindu_base
    {

    public:
        astro_hindu_lunar_calendar() = default;
        ~astro_hindu_lunar_calendar() override = default;

        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        [[nodiscard]] lunar_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const lunar_date& date) const override;

        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;

        [[nodiscard]] bool is_leap_month(const fixed_date_t& date) const;
        [[nodiscard]] bool is_leap_day(const fixed_date_t& date) const;
    };
}