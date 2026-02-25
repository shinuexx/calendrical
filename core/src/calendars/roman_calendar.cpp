#include "calendars/roman_calendar.hpp"
#include "helpers.hpp"

namespace calendrical::calendars{
    roman_calendar::roman_calendar() = default;

    int32_t roman_calendar::get_year(const fixed_date_t& date) const {
        return to_date(date).year;
    }

    bool roman_calendar::is_leap_year(int32_t year) const {
        return m_julian.is_leap_year(year);
    }

    roman_event roman_calendar::get_event(const fixed_date_t& date) const {
        return to_date(date).event;
    }

    int32_t roman_calendar::get_count(const fixed_date_t& date) const {
        return to_date(date).count;
    }

    bool roman_calendar::get_leap(const fixed_date_t& date) const {
        return to_date(date).leap;
    }

    roman_date roman_calendar::to_date(const fixed_date_t& date) const {
        auto [year, month, day] = m_julian.to_date(date);
        auto month1 = static_cast<calendrical::month>(adjusted_remainder(static_cast<std::underlying_type_t<calendrical::month>>(month) + 1, 12));
        auto year1 = month1 != month::january ? year : year != -1 ? year + 1 : 1;
        const auto kalends1 = to_fixed({ year1, month1, roman_event::kalends, 1, false });
        if (day == 1)
            return {year, month, roman_event::kalends, 1, false};
        if (day <= nones_of_month(month))
            return {year, month, roman_event::nones, nones_of_month(month) - day + 1, false};
        if (day <= ides_of_month(month))
            return {year, month, roman_event::ides, ides_of_month(month) - day + 1, false};
        if (month != month::february || !is_leap_year(year))
            return {year1, month1, roman_event::kalends, static_cast<int32_t>(kalends1 - date + 1), false};
        if (day < 25)
            return {year, month::march, roman_event::kalends, 30 - day, false};
        return {year, month::march, roman_event::kalends, 31 - day, day == 25};
    }
    fixed_date_t roman_calendar::to_fixed(const roman_date& date) const {
        int64_t a{}, b{}, c{};
        if (date.event == roman_event::kalends)
            a = m_julian.to_fixed({ date.year, date.month, 1 });
        else if (date.event == roman_event::nones)
            a = m_julian.to_fixed({ date.year, date.month, nones_of_month(date.month) });
        else if (date.event == roman_event::ides)
            a = m_julian.to_fixed({ date.year, date.month, ides_of_month(date.month) });
        b = is_leap_year(date.year) && date.month == month::march && date.event == roman_event::kalends && in(range_closed_between(6, 16), date.count) ? 0 : 1;
        c = date.leap ? 1 : 0;
        return a - date.count + b + c;
    }

    month roman_calendar::get_month(const fixed_date_t &date) const { return to_date(date).month; }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t roman_calendar::ides_of_month(month m) const { // NOLINT(*-convert-member-functions-to-static)
        return in(std::array{ month::march, month::may, month::july, month::october }, m) ? 15 : 13;
    }
    int32_t roman_calendar::nones_of_month(month m) const {
        return ides_of_month(m) - 8;
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t roman_calendar::julian_year_from_auc(int32_t year) const { // NOLINT(*-convert-member-functions-to-static)
        if (in(range_closed_between(1, -year_rome_founded), year)) {
            return year + year_rome_founded - 1;
        }
        return year + year_rome_founded;
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t roman_calendar::auc_year_from_julian(int32_t year) const { // NOLINT(*-convert-member-functions-to-static)
        if (in(range_closed_between(year_rome_founded, -1), year)) {
            return year - year_rome_founded + 1;
        }
        return year - year_rome_founded;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t roman_calendar::julian_year_from_olympiad(const olympiad& odate) const { // NOLINT(*-convert-member-functions-to-static)
        auto &[cycle, year] = odate;
        const auto years = olympiad_start + 4 * (cycle - 1) + year - 1;
        return years < 0 ? years : years + 1;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    olympiad roman_calendar::olympiad_from_julian_year(int32_t jyear) const { // NOLINT(*-convert-member-functions-to-static)
        const auto years = jyear - olympiad_start - (jyear < 0 ? 0 : 1);
        return {
            static_cast<int32_t>(floor_div(years, 4) + 1),
            static_cast<int32_t>(mod(years, 4) + 1)
        };
    }
}