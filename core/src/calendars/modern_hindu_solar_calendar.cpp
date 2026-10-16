#include "calendars/modern_hindu_solar_calendar.hpp"
#include <cmath>

namespace calendrical::calendars {
    simple_date modern_hindu_solar_calendar::to_date(const fixed_date_t& date) const
    {
        const auto critical = hindu_sunrise(date.add(1));
        auto month = hindu_zodiac(critical);
        auto year = hindu_calendar_year(critical) - hindu_solar_era;
        // Convert floored solar longitude degrees to integer offset for date arithmetic
        const auto approx = date - 3 - mod(static_cast<int64_t>(floor(hindu_solar_longitude(critical)).degrees()), 30);
        const auto start =
                fixed_date_t{ least_passing([&](const int64_t& i) { return month == hindu_zodiac(hindu_sunrise(fixed_date_t{ i + 1 })); }, approx.value()) };
        const auto day = date - start + 1;
        return { year, month, static_cast<int32_t>(day) };
    }
    fixed_date_t modern_hindu_solar_calendar::to_fixed(const simple_date& date) const
    {
        auto [year, month, day] = date;
        // Convert floored sidereal year estimate to integer days for fixed_date_t addition
        const auto start = ms_epoch + static_cast<int64_t>(std::floor(hindu_sidereal_year * (year + hindu_solar_era + (month - 1) / 12.0)));
        return fixed_date_t{ least_passing([&](const int64_t& d) { return hindu_zodiac(hindu_sunrise(fixed_date_t{ d + 1 })) == month; }, (start - 3).value()) + day - 1 };
    }
    int32_t modern_hindu_solar_calendar::get_year(const fixed_date_t& date) const
    {
        const auto critical = hindu_sunrise(date.add(1));
        return hindu_calendar_year(critical) - hindu_solar_era;
    }
    int32_t modern_hindu_solar_calendar::get_month(const fixed_date_t& date) const
    {
        const auto critical = hindu_sunrise(date.add(1));
        return hindu_zodiac(critical);
    }
    int32_t modern_hindu_solar_calendar::get_day(const fixed_date_t& date) const
    {
        return to_date(date).day;
    }
}