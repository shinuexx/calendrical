#include "calendars/astro_hindu_lunar_calendar.hpp"
#include "time_and_astronomy.hpp"
#include <array>
#include <cmath>

namespace calendrical::calendars {

    using namespace calendrical::time_and_astronomy;

    lunar_date astro_hindu_lunar_calendar::to_date(const fixed_date_t& date) const
    {
        const auto critical = alt_hindu_sunrise(date);
        auto day = astro_hindu_lunar_day_from_moment(critical);
        auto leap_day = day == astro_hindu_lunar_day_from_moment(alt_hindu_sunrise(date.subtract(1)));
        const auto last_new_moon = new_moon_before(critical);
        const auto next_new_moon = new_moon_on_or_after(critical);
        const auto solar_month = sidereal_zodiac(last_new_moon);
        auto leap_month = solar_month == sidereal_zodiac(next_new_moon);
        auto month = static_cast<int32_t>(adjusted_remainder(solar_month + 1LL, 12));
        auto year = astro_hindu_calendar_year(moment_t{ month <= 2 ? date.add(180) : date }) - hindu_lunar_era;
        return {
            year,
            month,
            leap_month,
            day,
            leap_day
        };
    }
    fixed_date_t astro_hindu_lunar_calendar::to_fixed(const lunar_date& date) const
    {
        auto [year, month, leap_month, day, leap_day] = date;
        const double m = (month - 1.0) / 12;
        const auto approx = ms_epoch.value() + mean_sidereal_year * (year + hindu_lunar_era + m);
        // Convert floored sidereal year estimate to integer days
        const auto s = static_cast<int64_t>(
                std::floor(approx - mean_sidereal_year *
                                       interval_modulus(sidereal_solar_longitude(moment_t{ approx }) / 360_deg - m, -0.5, 0.5)));
        const auto k = astro_hindu_lunar_day_from_moment(moment_t{ s + 0.25 });
        auto est = s + day;
        const auto mid = to_date(fixed_date_t{ s - 15 });
        if (interval_opened_between(3, 27).contains(k)) {
            est -= k;
        } else if (mid.month != month || (mid.leap_month && !leap_month)) {
            est -= interval_modulus(k, -15, 15);
        } else {
            est -= interval_modulus(k, 15, 45);
        }
        const auto T = est - interval_modulus(astro_hindu_lunar_day_from_moment(moment_t{ est + 0.25 }) - day, -15, 15);
        const std::array<int64_t, 2> dates{day, adjusted_remainder(day + 1, 30)};
        const auto ddate = fixed_date_t{ least_passing([&](const int64_t& d) { return in(dates, astro_hindu_lunar_day_from_moment(alt_hindu_sunrise(fixed_date_t{ d }))); }, T - 1) };
        return leap_day ? ddate + 1 : ddate;
    }
    int32_t astro_hindu_lunar_calendar::get_year(const fixed_date_t& date) const
    {
        return to_date(date).year;
    }
    int32_t astro_hindu_lunar_calendar::get_month(const fixed_date_t& date) const
    {
        return to_date(date).month;
    }
    int32_t astro_hindu_lunar_calendar::get_day(const fixed_date_t& date) const
    {
        const auto critical = alt_hindu_sunrise(date);
        return astro_hindu_lunar_day_from_moment(critical);
    }
    bool astro_hindu_lunar_calendar::is_leap_month(const fixed_date_t& date) const
    {
        return to_date(date).leap_month;
    }
    bool astro_hindu_lunar_calendar::is_leap_day(const fixed_date_t& date) const
    {
        const auto critical = alt_hindu_sunrise(date);
        const auto day = astro_hindu_lunar_day_from_moment(critical);
        return day == astro_hindu_lunar_day_from_moment(alt_hindu_sunrise(date.subtract(1)));
    }
}