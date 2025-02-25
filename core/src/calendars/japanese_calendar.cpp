#include "calendars/japanese_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::calendars {
    location japanese_calendar::get_location(const moment_t& t) const
    {
        if (const auto year = gregorian_calendar::standard.get_year(t.to_fixed_date()); year < 1888)
            return {35.7, angle_t::dms(139, 46), 24, moment_t::hms(9 + 143 / 4.5e2)};
        return {35, 135, 0, moment_t::hms(9)};
    }
}