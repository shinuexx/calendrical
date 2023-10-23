#pragma once

#include "bahai_based_calendar.hpp"
#include "location.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    class astro_bahai_calendar :
        public bahai_based_calendar {

    protected:
        virtual int32_t get_years(const fixed_date_t& date) const override;

    public:
        virtual moment_t bahai_sunset(const fixed_date_t& date) const;
        virtual fixed_date_t new_year_on_or_before(const fixed_date_t& date) const;

    public:
        static constexpr location bahai_location{35.696111, 51.423056, 0, moment_t::hms(3.5)};

        astro_bahai_calendar() = default;

        virtual ~astro_bahai_calendar() = default;

        virtual bahai_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const bahai_date& date) const override;
    };
}