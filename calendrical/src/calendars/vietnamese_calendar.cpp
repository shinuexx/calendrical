#include "calendars/vietnamese_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::calendars {
    location vietnamese_calendar::get_location(const moment_t& t) const
    {
        static const auto t1 = gregorian_calendar::standard.get_new_year(1968);
        double z = t < t1 ? 8 : 7;
        return {
            angle_t::dms(21,2),
            angle_t::dms(105,51),
            12,
            moment_t::hms(z)
        };
    }
}