#include "calendars/astro_hindu_solar_calendar.hpp"
#include "time_and_astronomy.hpp"

namespace calendrical::calendars {

    using namespace calendrical::time_and_astronomy;

    simple_date astro_hindu_solar_calendar::to_date(const fixed_date_t& date) const
    {
        const auto critical = astro_hindu_sunset(date);
        auto month = sidereal_zodiac(critical);
        auto year = astro_hindu_calendar_year(critical) - hindu_solar_era;
        const auto approx = date - 3 - mod(static_cast<int64_t>(std::floor(sidereal_solar_longitude(critical))), 30);
        const auto start = least_passing(
                [&](const int64_t &i) { return sidereal_zodiac(astro_hindu_sunset(i)) == month; }, approx);
        const auto day = date - start + 1;
        return {
            year,
            month,
            static_cast<int32_t>(day)
        };
    }
    fixed_date_t astro_hindu_solar_calendar::to_fixed(const simple_date& date) const
    {
        auto [year, month, day] = date;
        const auto approx =
                ms_epoch - 3 +
                static_cast<int64_t>(std::floor(mean_sidereal_year * (year + hindu_solar_era + (month - 1) / 12.0)));
        const auto start = least_passing([&](const int64_t& i) { return sidereal_zodiac(astro_hindu_sunset(i)) == month; }, approx);
        return start + day - 1;
    }
    int32_t astro_hindu_solar_calendar::get_year(const fixed_date_t& date) const
    {
        return to_date(date).year;
    }
    int32_t astro_hindu_solar_calendar::get_month(const fixed_date_t& date) const
    {
        return to_date(date).month;
    }
    int32_t astro_hindu_solar_calendar::get_day(const fixed_date_t& date) const
    {
        return to_date(date).day;
    }
}