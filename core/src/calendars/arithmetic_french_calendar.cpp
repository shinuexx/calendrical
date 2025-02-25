#include "calendars/arithmetic_french_calendar.hpp"
#include <array>

namespace calendrical::calendars {
    bool arithmetic_french_calendar::is_leap_year(int32_t year) const {
        constexpr std::array<int64_t,3> years{100, 200, 300};
        return mod(year, 4) == 0
            && !in(years, mod(year, 400))
            && mod(year, 4000) != 0;
    }
    fixed_date_t arithmetic_french_calendar::get_new_year(int32_t year) const
    {
        const int64_t y1 = year - 1LL;
        return ms_epoch
            + 365 * y1
            + floor_div(y1, 4)
            - floor_div(y1, 100)
            + floor_div(y1, 400)
            - floor_div(y1, 4000);
    }
    int32_t arithmetic_french_calendar::get_year(const fixed_date_t& date) const
    {
        const auto approx = static_cast<int32_t>(floor_div(4000 * (date - ms_epoch + 2), 1460969) + 1);
        return date < get_new_year(approx) ? approx - 1 : approx;
    }
    int32_t arithmetic_french_calendar::get_month(const fixed_date_t& date) const
    {
        return get_month(date, get_new_year(get_year(date)));
    }
    int32_t arithmetic_french_calendar::get_day(const fixed_date_t& date) const
    {
        return get_day(date, get_new_year(get_year(date)));
    }
    simple_date arithmetic_french_calendar::to_date(const fixed_date_t& date) const
    {
        auto year = get_year(date);
        const auto new_year = get_new_year(year);
        auto month = get_month(date, new_year);
        auto day = get_day(date, new_year);
        return { year, month, day };
    }
    fixed_date_t arithmetic_french_calendar::to_fixed(const simple_date& date) const
    {
        auto [year, month, day] = date;
        return get_new_year(year)
            + 30 * (month - 1LL)
            + day - 1;
    }
}