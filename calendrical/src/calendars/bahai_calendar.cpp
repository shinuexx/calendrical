#include "calendars/bahai_calendar.hpp"

namespace calendrical::calendars {
    int32_t bahai_calendar::get_years(const fixed_date_t& date) const
    {
        auto gyear = m_gregorian->get_year(date);
        return gyear - ms_gregorian_year_bahai_epoch - (date <= m_gregorian->to_fixed({ gyear, month::march, 20 }) ? 1 : 0);
    }
    bahai_date bahai_calendar::to_date(const fixed_date_t& date) const
    {
        auto years = get_years(date);
        auto major = get_major(years);
        auto cycle = get_cycle(years);
        auto year = get_year(years);
        auto month = get_month(date, major, cycle, year);
        auto day = date + 1 - to_fixed({ major, cycle, year, month, 1 });
        return { major, cycle, year, month, static_cast<int32_t>(day)};
    }
    fixed_date_t bahai_calendar::to_fixed(const bahai_date& date) const
    {
        auto [major, cycle, year, month, day] = date.tie();
        auto gyear = 361 * (major - 1) + 19 * (cycle - 1) + year - 1 + ms_gregorian_year_bahai_epoch;
        int32_t m;
        if (month == ayyam_i_ha)
            m = 342;
        else if (month == 19)
            m = m_gregorian->is_leap_year(gyear + 1) ? 347 : 346;
        else
            m = 19 * (month - 1);
        return m_gregorian->to_fixed({ gyear, month::march, 20 }).add(static_cast<int64_t>(m) + day);
    }
}