#include "calendars/julian_calendar.hpp"

namespace calendrical::calendars{
    month julian_calendar::get_month(const fixed_date_t &date, int32_t year) const {
        const auto prior_days = date - get_new_year(year);
        const auto correction = date < to_fixed({year, month::march, 1}) ? 0 : is_leap_year(year) ? 1 : 2;
        return static_cast<month>(floor_div(12 * (prior_days + correction) + 373, 367));
    }

    fixed_date_t julian_calendar::get_new_year(int32_t year) const { return to_fixed({year, month::january, 1}); }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t julian_calendar::get_year(const fixed_date_t &date) const { // NOLINT(*-convert-member-functions-to-static)
        const auto approx = floor_div(4 * (date - ms_epoch) + 1464,1461);
        return static_cast<int32_t>(approx <= 0 ? approx - 1 : approx);
    }

    int32_t julian_calendar::get_day(const fixed_date_t &date) const { return to_date(date).day; }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    bool julian_calendar::is_leap_year(int32_t year) const {
        return mod(year, 4) == (year > 0 ? 0 : 3);
    }

    julian_date julian_calendar::to_date(const fixed_date_t &date) const {
        const auto year = get_year(date);
        const auto month = get_month(date,year);
        const auto day = date - to_fixed({year, month, 1}) + 1;
        return {year, month, static_cast<int32_t>(day)};
    }

    fixed_date_t julian_calendar::to_fixed(const julian_date &date) const {
        const auto &[year, month, day] = date;
        const int64_t y = year < 0 ? year + 1LL : year;
        // Scoped enum month converted to int64_t index for arithmetic
        const auto m = static_cast<int64_t>(month);
        return ms_epoch - 1
            + 365 *(y - 1)
            + floor_div(y - 1, 4)
            + floor_div(367 * m - 362, 12)
            + (month <= month::february ? 0 : is_leap_year(year) ? -1 : -2)
            + day;
    }

    month julian_calendar::get_month(const fixed_date_t &date) const {
        const auto year = get_year(date);
        return get_month(date, year);
    }

    bool julian_calendar::is_valid_date(const julian_date& date) const {
        if (date.year == 0) {
            return false;
        }
        if (date.month < month::january || date.month > month::december) {
            return false;
        }
        if (date.day < 1) {
            return false;
        }
        int32_t max_days = 31;
        switch (date.month) {
            case month::february:
                max_days = is_leap_year(date.year) ? 29 : 28;
                break;
            case month::april:
            case month::june:
            case month::september:
            case month::november:
                max_days = 30;
                break;
            default:
                max_days = 31;
                break;
        }
        return date.day <= max_days;
    }
}