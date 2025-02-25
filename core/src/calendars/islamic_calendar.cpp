#include "calendars/islamic_calendar.hpp"

namespace calendrical::calendars{

    int32_t islamic_calendar::get_month(const fixed_date_t &date, int32_t year) const {
        const auto prior_days = date - get_new_year(year);
        return static_cast<int32_t>(floor_div(11 * prior_days + 330, 325));
    }

    int32_t islamic_calendar::get_month(const fixed_date_t &date) const {
        return get_month(date, get_year(date));
    }

    int32_t islamic_calendar::get_year(const fixed_date_t &date) const {
        return static_cast<int32_t>(floor_div(30 * (date - ms_epoch) + 10646, 10631));
    }

    int32_t islamic_calendar::get_day(const fixed_date_t &date) const {
        return to_date(date).day;
    }


    simple_date islamic_calendar::to_date(const fixed_date_t &date) const {
        const auto year = get_year(date);
        const auto month = get_month(date, year);
        const auto day = date - to_fixed({year, month, 1}) + 1;
        return {year, month, static_cast<int32_t>(day)};
    }

    fixed_date_t islamic_calendar::to_fixed(const simple_date &date) const {
        auto &[year, month, day] = date;
        return ms_epoch - 1 + 354 * (year - 1LL) + floor_div(3 + 11 * year, 30) + 29 * (month - 1LL) +
               floor_div(month, 2) + day;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    bool islamic_calendar::is_leap_year(int32_t year) const {
        return mod(14 + 11 * year, 30) < 11;
    }
}