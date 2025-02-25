#pragma once

#include "datetypes/lunar_date.hpp"
#include "modern_hindu_base.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    class modern_hindu_lunar_calendar :
        public typed_calendar_if<lunar_date>,
        public modern_hindu_base
    {

    public:
        modern_hindu_lunar_calendar() = default;
        modern_hindu_lunar_calendar(const rational_location& hindu_location) : modern_hindu_base{ hindu_location }{}
        virtual ~modern_hindu_lunar_calendar() = default;

        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        [[nodiscard]] lunar_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const lunar_date& date) const override;

        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;

        [[nodiscard]] bool is_leap_month(const fixed_date_t& date) const;
        [[nodiscard]] bool is_leap_day(const fixed_date_t& date) const;

        [[nodiscard]] bool is_hindu_expunged(int32_t lyear, int32_t lmonth) const;

        [[nodiscard]] lunar_date to_full_moon_date(const fixed_date_t& date) const;
        [[nodiscard]] fixed_date_t to_full_moon_fixed(lunar_date date) const;

        [[nodiscard]] bool hindu_lunar_on_or_before(const lunar_date& date1, const lunar_date& date2) const;
    };
}