#include "calendars/iso_calendar.hpp"
#include <array>

namespace calendrical::calendars{
    iso_calendar::iso_calendar() {
        m_gregorian = std::addressof(gregorian_calendar::standard);
        m_epoch = iso_calendar::to_fixed({1, 1, 1});
    }

    int32_t iso_calendar::get_week(const fixed_date_t &date, int32_t year) const {
        return static_cast<int32_t>(floor_div(date - get_new_year(year), 7)) + 1;
    }

    fixed_date_t iso_calendar::epoch() const {
        return m_epoch;
    }

    fixed_date_t iso_calendar::get_new_year(int32_t year) const {
        return to_fixed({year, 1, 1});
    }

    iso_date iso_calendar::to_date(const fixed_date_t &date) const {
        const auto year = get_year(date);
        const auto week = get_week(date, year);
        const auto day = get_day(date);
        return {year, week, day};
    }

    fixed_date_t iso_calendar::to_fixed(const iso_date &date) const {
        return m_gregorian->nth_weekday(date.week, weekday::sunday, {date.year - 1, month::december, 28}) + date.day;
    }

    int32_t iso_calendar::get_year(const fixed_date_t &date) const {
        const auto approx = m_gregorian->get_year(date.subtract(3));
        const auto year = date >= get_new_year(approx + 1) ? approx + 1 : approx;
        return year;
    }

    bool iso_calendar::is_leap_year(int32_t year) const {
        return is_long_year(year);
    }

    bool iso_calendar::is_long_year(int32_t year) const {
        const auto jan1 = m_gregorian->get_new_year(year).get_weekday();
        const auto dec31 = m_gregorian->get_year_end(year).get_weekday();
        return in(std::array{jan1, dec31}, weekday::thursday);
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t iso_calendar::get_day(const fixed_date_t &date) const {
        return static_cast<int32_t>(adjusted_remainder(date.value(), 7));
    }

    int32_t iso_calendar::get_week(const fixed_date_t &date) const {
        return get_week(date, get_year(date));
    }
}