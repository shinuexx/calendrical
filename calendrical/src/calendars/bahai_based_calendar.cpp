#pragma once

#include "calendars/bahai_based_calendar.hpp"

namespace calendrical::calendars {
    int32_t bahai_based_calendar::get_major(int32_t years) const
    {
        return static_cast<int32_t>(floor_div(years, 361LL) + 1);
    }
    int32_t bahai_based_calendar::get_cycle(int32_t years) const
    {
        return static_cast<int32_t>(floor_div(mod(years, 361LL), 19) + 1);
    }
    int32_t bahai_based_calendar::get_year(int32_t years) const
    {
        return static_cast<int32_t>(mod(years, 19LL) + 1);
    }
    int32_t bahai_based_calendar::get_month(const fixed_date_t& date, int32_t major, int32_t cycle, int32_t year) const
    {
        auto days = get_days(date, to_fixed({major, cycle, year, 1, 1}));
        int32_t month;
        if (date >= to_fixed({ major, cycle, year, 19, 1 }))
            month = 19;
        else if (date >= to_fixed({ major, cycle, year, ayyam_i_ha, 1 }))
            month = ayyam_i_ha;
        else
            month = static_cast<int32_t>(floor_div(days, 19LL) + 1);
        return month;
    }
    int32_t bahai_based_calendar::get_days(const fixed_date_t& date, const fixed_date_t& new_year) const
    {
        return static_cast<int32_t>(date - new_year);
    }
    int32_t bahai_based_calendar::get_day(const fixed_date_t& date, int32_t major, int32_t cycle, int32_t year, int32_t month) const
    {
        return static_cast<int32_t>(date + 1 - to_fixed({ major, cycle, year, month, 1 }));
    }
    bahai_based_calendar::bahai_based_calendar()
    {
        m_gregorian = gregorian_calendar::standard.get_shared();
    }
    int32_t bahai_based_calendar::get_major(const fixed_date_t& date) const
    {
        return get_major(get_years(date));
    }
    int32_t bahai_based_calendar::get_cycle(const fixed_date_t& date) const
    {
        return get_cycle(get_years(date));
    }
    int32_t bahai_based_calendar::get_year(const fixed_date_t& date) const
    {
        return get_year(get_years(date));
    }
    int32_t bahai_based_calendar::get_month(const fixed_date_t& date) const
    {
        auto years = get_years(date);
        auto major = get_major(years);
        auto cycle = get_cycle(years);
        auto year = get_year(years);
        return get_month(date, major, cycle, year);
    }
    int32_t bahai_based_calendar::get_day(const fixed_date_t& date) const
    {
        return to_date(date).day;
    }
}