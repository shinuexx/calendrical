#include "calendars/old_hindu_lunar_calendar.hpp"

namespace calendrical::calendars {
    static const rational quarter{ 1,4 };
    rational old_hindu_lunar_calendar::get_sun(const fixed_date_t& date) const
    {
        return day_count(date) + quarter;
    }
    rational old_hindu_lunar_calendar::get_new_moon(const rational& sun) const
    {
        return sun - mod(sun, arya_lunar_month);
    }
    int32_t old_hindu_lunar_calendar::get_month(const rational& new_moon) const
    {
        auto m = mod(ceil(new_moon / arya_solar_month), 12) + 1;
        return (int32_t)m;
    }
    int32_t old_hindu_lunar_calendar::get_day(const rational& sun) const
    {
        auto d = mod((bigint)floor_div(sun, arya_lunar_day), 30) + 1;
        return (int32_t)d;
    }
    int32_t old_hindu_lunar_calendar::get_year(const rational& new_moon) const
    {
        auto y = ceil((new_moon + arya_solar_month) / arya_solar_year) + 1;
        return (int32_t)y;
    }
    bool old_hindu_lunar_calendar::get_leap(const rational& new_moon) const
    {
        static const auto r = range_right_closed_between<rational>(0, arya_solar_lunar_month_difference);
        return r(mod(new_moon, arya_solar_month));
    }
    fixed_date_t old_hindu_lunar_calendar::get_new_year(int32_t year) const
    {
        //return to_fixed({ year, 1, false, 1 });
        auto mina = arya_solar_year * (12 * year - 1);
        auto lunar_new_year = arya_lunar_month * (1 + floor_div(mina, arya_lunar_month));
        return (int64_t)ceil(static_cast<int64_t>(m_epoch) + lunar_new_year - quarter);
    }
    int32_t old_hindu_lunar_calendar::get_year(const fixed_date_t& date) const
    {
        return get_year(get_new_moon(get_sun(date)));
    }
    simple_lunar_date old_hindu_lunar_calendar::to_date(const fixed_date_t& date) const
    {
        auto sun = get_sun(date);
        auto new_moon = get_new_moon(sun);
        auto leap = get_leap(new_moon);
        auto month = get_month(new_moon);
        auto day = get_day(sun);
        auto year = get_year(new_moon);
        return { year, month, leap, day };
    }
    fixed_date_t old_hindu_lunar_calendar::to_fixed(const simple_lunar_date& date) const
    {
        auto [year, month, leap, day] = date.tie();
        auto mina = arya_solar_year * (12 * year - 1);
        auto lunar_new_year = arya_lunar_month * (1 + floor_div(mina, arya_lunar_month));
        auto m = !leap && (ceil((lunar_new_year - mina) / arya_solar_lunar_month_difference) <= month) ? month : month - 1;
        auto a = static_cast<int64_t>(m_epoch)
            + lunar_new_year
            + m * arya_lunar_month
            + (day - 1) * arya_lunar_day
            - quarter;
        return (int64_t)ceil(a);
    }
    int32_t old_hindu_lunar_calendar::get_month(const fixed_date_t& date) const
    {
        return get_month(get_new_moon(get_sun(date)));
    }
    int32_t old_hindu_lunar_calendar::get_day(const fixed_date_t& date) const
    {
        return get_day(get_sun(date));
    }
    bool old_hindu_lunar_calendar::is_leap_year(int32_t year) const
    {
        return mod(year * arya_solar_year - arya_solar_month, arya_lunar_month) >= lunar_leap_year_indicator;
    }
}