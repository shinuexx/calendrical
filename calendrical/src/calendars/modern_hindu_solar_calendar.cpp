#include "calendars/modern_hindu_solar_calendar.hpp"

namespace calendrical::calendars {
    simple_date modern_hindu_solar_calendar::to_date(const fixed_date_t& date) const
    {
        auto critical = hindu_sunrise(date.add(1));
        auto month = hindu_zodiac(critical);
        auto year = hindu_calendar_year(critical) - hindu_solar_era;
        auto approx = date - 3 - mod((int64_t)floor(hindu_solar_longitude(critical)), 30);
        auto start = least_passing([=](int64_t i) { return month == hindu_zodiac(hindu_sunrise(i + 1)); }, approx);
        auto day = date - start + 1;
        return { year, month, (int32_t)day };
    }
    fixed_date_t modern_hindu_solar_calendar::to_fixed(const simple_date& date) const
    {
        auto [year, month, day] = date.tie();
        auto start = (int64_t)floor(hindu_sidereal_year * (year + hindu_solar_era + rational{ month - 1, 12 })) + m_epoch;
        return day - 1 + least_passing([=](int64_t d) { return hindu_zodiac(hindu_sunrise(d + 1)) == month; }, start - 3);
    }
    int32_t modern_hindu_solar_calendar::get_year(const fixed_date_t& date) const
    {
        auto critical = hindu_sunrise(date.add(1));
        return hindu_calendar_year(critical) - hindu_solar_era;
    }
    int32_t modern_hindu_solar_calendar::get_month(const fixed_date_t& date) const
    {
        auto critical = hindu_sunrise(date.add(1));
        return hindu_zodiac(critical);
    }
    int32_t modern_hindu_solar_calendar::get_day(const fixed_date_t& date) const
    {
        return to_date(date).day;
    }
}