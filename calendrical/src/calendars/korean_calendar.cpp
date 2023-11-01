#include "calendars/korean_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::calendars {
    location korean_calendar::get_location(const moment_t& t) const
    {
        constexpr auto lat = angle_t::dms(37, 34);
        constexpr auto lon = angle_t::dms(126, 58);
        
        static const auto t1 = gregorian_calendar::standard.to_fixed({ 1908, month::april, 1 });
        static const auto t2 = gregorian_calendar::standard.to_fixed({ 1912, month::january, 1 });
        static const auto t3 = gregorian_calendar::standard.to_fixed({ 1954, month::march, 21 });
        static const auto t4 = gregorian_calendar::standard.to_fixed({ 1961, month::august, 10 });

        double z;
        if (t < t1)
            z = 3809 / 4.5e2;
        else if (t < t2)
            z = 8.5;
        else if (t < t3)
            z = 9;
        else if (t < t4)
            z = 8.5;
        else
            z = 9;
        return { lat, lon, 0, moment_t::hms(z) };
    }
    int32_t korean_calendar::korean_year(int32_t cycle, int32_t year)
    {
        return 60 * cycle + year - 364;
    }
}