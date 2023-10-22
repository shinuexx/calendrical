#include "calendars/islamic_calendar.hpp"

namespace calendrical::calendars {
    int32_t islamic_calendar::get_month(const fixed_date_t& date, int32_t year) const {
        auto prior_days = date - get_new_year(year);
        return static_cast<int32_t>(floor_div(11 * prior_days + 330, 325));
    }

    int32_t islamic_calendar::get_month(const fixed_date_t& date) const {
        return get_month(date, get_year(date));
    }

    int32_t islamic_calendar::get_year(const fixed_date_t& date) const {
        return static_cast<int32_t>(floor_div(30 * (date - m_epoch) + 10646, 10631));
    }

    simple_date islamic_calendar::to_date(const fixed_date_t& date) const {
        auto year = get_year(date);
        auto month = get_month(date, year);
        auto day = date - to_fixed({ year, month, 1 }) + 1;
        return { year, month, static_cast<int32_t>(day) };
    }

    fixed_date_t islamic_calendar::to_fixed(const simple_date& date) const {
        auto [year, month, day] = date.tie();
        return m_epoch - 1
            + 354 * (year - 1ll)
            + floor_div(3 + 11ll * year, 30)
            + 29 * (month - 1ll)
            + floor_div(month, 2ll)
            + day;
    }

    bool islamic_calendar::is_leap_year(int32_t year) const {
        return mod(14 + 11ll * year, 30) < 11;
    }
}