#include "calendars/hebrew_based_calendar.hpp"

namespace calendrical::calendars {
    int32_t hebrew_based_calendar::last_day_of_month(int32_t year, hebrew_month hm) const {
        if (constexpr auto months = std::array{hebrew_month::iyyar, hebrew_month::tammuz, hebrew_month::elul,
                                               hebrew_month::tevet, hebrew_month::adarii};
            in(months, hm) || (hm == hebrew_month::adar && !is_leap_year(year)) ||
            (hm == hebrew_month::marheshvan && !is_long_marheshvan(year)) ||
            (hm == hebrew_month::kislev && is_short_kislev(year)))
            return 29;
        return 30;
    }

    bool hebrew_based_calendar::is_short_kislev(int32_t year) const {
        return in(std::array{353,383}, days_in_year(year));
    }

    bool hebrew_based_calendar::is_long_marheshvan(int32_t year) const {
        return in(std::array{355, 385}, days_in_year(year));
    }

    int32_t hebrew_based_calendar::days_in_year(int32_t year) const {
        return static_cast<int32_t>(get_new_year(year + 1) - get_new_year(year));
    }

    hebrew_month hebrew_based_calendar::last_month_of_year(int32_t year) const {
        return is_leap_year(year) ? hebrew_month::adarii : hebrew_month::adar;
    }

}