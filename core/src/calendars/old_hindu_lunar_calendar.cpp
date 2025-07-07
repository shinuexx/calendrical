#include "calendars/old_hindu_lunar_calendar.hpp"
#include <cmath>

namespace calendrical::calendars {
    static constexpr auto quarter{0.25};

    double old_hindu_lunar_calendar::get_sun(const fixed_date_t &date) const { return day_count(date) + quarter; }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    double
    old_hindu_lunar_calendar::get_new_moon(const double &sun) const // NOLINT(*-convert-member-functions-to-static)
    {
        return sun - mod(sun, arya_lunar_month);
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t old_hindu_lunar_calendar::get_month(const double& new_moon) const // NOLINT(*-convert-member-functions-to-static)
    {
        const auto m = mod(ceil(new_moon / arya_solar_month), 12) + 1;
        return static_cast<int32_t>(m);
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t old_hindu_lunar_calendar::get_day(const double& sun) const // NOLINT(*-convert-member-functions-to-static)
    {
        const auto d = mod(floor_div(sun, arya_lunar_day), 30) + 1;
        return static_cast<int32_t>(d);
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t old_hindu_lunar_calendar::get_year(const double& new_moon) const // NOLINT(*-convert-member-functions-to-static)
    {
        const auto y = ceil((new_moon + arya_solar_month) / arya_solar_year) - 1;
        return static_cast<int32_t>(y);
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    bool old_hindu_lunar_calendar::get_leap(const double& new_moon) const // NOLINT(*-convert-member-functions-to-static)
    {
        constexpr auto r = range_right_closed_between<double>(0, arya_solar_lunar_month_difference);
        return r(mod(new_moon, arya_solar_month));
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    fixed_date_t old_hindu_lunar_calendar::get_new_year(int32_t year) const // NOLINT(*-convert-member-functions-to-static)
    {
        // return to_fixed({ year, 1, false, 1 });
        const auto mina = arya_solar_year * (12 * year - 1);
        const auto lunar_new_year = arya_lunar_month * (1 + floor_div(mina, arya_lunar_month));
        return static_cast<int64_t>(std::ceil(static_cast<int64_t>(ms_epoch) + lunar_new_year - quarter));
    }
    int32_t old_hindu_lunar_calendar::get_year(const fixed_date_t& date) const
    {
        return get_year(get_new_moon(get_sun(date)));
    }
    simple_lunar_date old_hindu_lunar_calendar::to_date(const fixed_date_t& date) const
    {
        const auto sun = get_sun(date);
        const auto new_moon = get_new_moon(sun);
        auto leap = get_leap(new_moon);
        auto month = get_month(new_moon);
        auto day = get_day(sun);
        auto year = get_year(new_moon);
        return { year, month, leap, day };
    }
    fixed_date_t old_hindu_lunar_calendar::to_fixed(const simple_lunar_date& date) const
    {
        auto [year, month, leap, day] = date;
        const auto mina = arya_solar_month * (12 * year - 1);
        const auto lunar_new_year = arya_lunar_month * (1 + floor_div(mina, arya_lunar_month));
        const auto m = !leap && std::ceil((lunar_new_year - mina) / arya_solar_lunar_month_difference) <= month
                               ? month
                               : month - 1;
        const auto a = static_cast<int64_t>(ms_epoch)
            + lunar_new_year
            + m * arya_lunar_month
            + (day - 1) * arya_lunar_day
            - quarter;
        return static_cast<int64_t>(ceil(a));
    }
    int32_t old_hindu_lunar_calendar::get_month(const fixed_date_t& date) const
    {
        return get_month(get_new_moon(get_sun(date)));
    }
    int32_t old_hindu_lunar_calendar::get_day(const fixed_date_t& date) const
    {
        return get_day(get_sun(date));
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    bool old_hindu_lunar_calendar::is_leap_year(int32_t year) const // NOLINT(*-convert-member-functions-to-static)
    {
        return mod(year * arya_solar_year - arya_solar_month, arya_lunar_month) >= lunar_leap_year_indicator;
    }
}