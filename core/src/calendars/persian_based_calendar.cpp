#include "calendars/persian_based_calendar.hpp"
#include "calendars/julian_calendar.hpp"

namespace calendrical::calendars {
    const fixed_date_t persian_based_calendar::ms_epoch = 226896;//julian_calendar{}.to_fixed({ 622, month::march, 19 });

    simple_date persian_based_calendar::to_date(const fixed_date_t& date) const
    {
        auto year = get_year(date);
        const auto day_of_year = get_day_of_year(date, year);
        auto month = get_month(day_of_year);
        auto day = get_day(date, year, month);
        return { year, month, day };
    }

    int32_t persian_based_calendar::get_month(const fixed_date_t& date) const
    {
        return get_month(get_day_of_year(date, get_year(date)));
    }

    int32_t persian_based_calendar::get_day(const fixed_date_t& date) const
    {
        const auto year = get_year(date);
        return get_day(date, year, get_month(get_day_of_year(date, year)));
    }

    bool persian_based_calendar::is_leap_year(int32_t year) const
    {
        return get_new_year(year + 1) - get_new_year(year) == 366;
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t persian_based_calendar::get_month(int32_t day_of_year) const // NOLINT(*-convert-member-functions-to-static)
    {
        return static_cast<int32_t>(std::ceil(day_of_year <= 186 ? day_of_year / 31.0 : (day_of_year - 6) / 30.0));
    }
    int32_t persian_based_calendar::get_day_of_year(const fixed_date_t& date, int32_t year) const
    {
        return static_cast<int32_t>(1 + date - get_new_year(year));
    }
    int32_t persian_based_calendar::get_day(const fixed_date_t& date, int32_t year, int32_t month) const
    {
        return static_cast<int32_t>(date - to_fixed({ year, month, 1 }) + 1);
    }
}