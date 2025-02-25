#pragma once

#include "datetypes/simple_date.hpp"
#include "modern_hindu_base.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    class modern_hindu_solar_calendar :
        public typed_calendar_if<simple_date>,
        public modern_hindu_base
    {

    public:
        modern_hindu_solar_calendar() = default;
        modern_hindu_solar_calendar(const rational_location& hindu_location) : modern_hindu_base{ hindu_location }{}
        ~modern_hindu_solar_calendar() override = default;

        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;

        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
    };
}