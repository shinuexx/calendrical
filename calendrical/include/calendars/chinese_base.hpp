#pragma once

#include "location.hpp"
#include "moment.hpp"

namespace calendrical::calendars {
    class chinese_base {
    public:
        using location_provider = std::function<location(const moment_t&)>;

        virtual ~chinese_base() = default;

        virtual location get_location(const moment_t& t) const = 0;

        static location chinese_location(const moment_t& t);
        
        int32_t current_major_solar_term(const fixed_date_t& date) const;
        moment_t solar_longitude_on_or_after(const angle_t& l, const moment_t& t) const;
        moment_t major_solar_term_on_or_after(const fixed_date_t& date) const;
        moment_t minor_solar_term_on_of_after(const fixed_date_t& date) const;
        moment_t midnight_in_china(const fixed_date_t& date) const;
        fixed_date_t winter_solstice_on_or_before(const fixed_date_t& date) const;

        fixed_date_t new_moon_on_or_after(const fixed_date_t& date) const;
        fixed_date_t new_moon_before(const fixed_date_t& date) const;

        bool no_major_solar_term(const fixed_date_t& date) const;
        bool is_prior_leap_month(const fixed_date_t& m1, const fixed_date_t& m) const;

        fixed_date_t new_year_in_sui(const fixed_date_t& date) const;
        fixed_date_t new_year_on_or_before(const fixed_date_t& date) const;
    };
}