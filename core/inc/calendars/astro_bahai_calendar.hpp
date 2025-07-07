#pragma once

#include "bahai_based_calendar.hpp"
#include "location.hpp"

namespace calendrical::calendars{

    class astro_bahai_calendar final : public bahai_based_calendar{
    protected:
        [[nodiscard]] int32_t get_years(const fixed_date_t& date) const override;

    public:
        [[nodiscard]] moment_t bahai_sunset(const fixed_date_t& date) const;
        [[nodiscard]] fixed_date_t new_year_on_or_before(const fixed_date_t& date) const;

        static constexpr location bahai_location{35.696111, 51.423056, 0, moment_t::hms(3.5)};

        astro_bahai_calendar() = default;

        ~astro_bahai_calendar() override = default;

        [[nodiscard]] bahai_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const bahai_date& date) const override;
    };
}