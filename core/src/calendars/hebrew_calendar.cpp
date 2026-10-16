#include "calendars/hebrew_calendar.hpp"
#include <algorithm>
#include <array>

namespace calendrical::calendars {


    hebrew_month hebrew_calendar::get_month(const fixed_date_t& date, int32_t year) const {
        auto start = date < to_fixed({ year, hebrew_month::nisan, 1 }) ? hebrew_month::tishri : hebrew_month::nisan;
        // Scoped enum hebrew_month converted to int64_t search bound for least_passing
        auto month = least_passing([&](int64_t m) {
            auto hm = static_cast<hebrew_month>(m);
            return date <= to_fixed({ year, hm, last_day_of_month(year, hm) });
            }, static_cast<int64_t>(start));
        return static_cast<hebrew_month>(month);
    }

    int64_t hebrew_calendar::year_length_correction(int32_t year) const {
        const auto ny0 = elapsed_days(year - 1);
        const auto ny1 = elapsed_days(year);
        if (const auto ny2 = elapsed_days(year + 1); ny2 - ny1 == 356)
            return 2;
        if (ny1 - ny0 == 382)
            return 1;
        return 0;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    int64_t hebrew_calendar::elapsed_days(int32_t year) const { // NOLINT(*-convert-member-functions-to-static)
        const auto months_elapsed = floor_div(235 * year - 234, 19);
        const auto parts_elapsed = 12084 + 13753 * months_elapsed;
        const auto days = 29 * months_elapsed + floor_div(parts_elapsed, 25920);
        return mod(3 * (days + 1), 7) < 3 ? days + 1 : days;
    }

    hebrew_calendar::hebrew_calendar() : m_possible_days_begin(hebrew_calendar::to_fixed({5, hebrew_month::nisan, 1})){}

    bool hebrew_calendar::is_leap_year(int32_t year) const { return mod(7 * year + 1, 19) < 7; }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    bool hebrew_calendar::is_sabbatical_year(int32_t year) const { // NOLINT(*-convert-member-functions-to-static)
        return mod(year, 7) == 0;
    }

    fixed_date_t hebrew_calendar::get_new_year(int32_t year) const {
        return ms_epoch + elapsed_days(year) + year_length_correction(year);
    }

    int32_t hebrew_calendar::get_year(const fixed_date_t& date) const {
        const auto approx = floor_div(98496 * (date - ms_epoch), 35975351) + 1;
        const auto year = last_passing([&](int64_t y){ return get_new_year(static_cast<int32_t>(y)) <= date; }, approx - 1);
        return static_cast<int32_t>(year);
    }

    hebrew_date hebrew_calendar::to_date(const fixed_date_t& date) const {
        const auto year = get_year(date);
        const auto month = get_month(date, year);
        const auto day = date - to_fixed({year, month, 1}) + 1;
        return {year, month, static_cast<int32_t>(day)};
    }

    fixed_date_t hebrew_calendar::to_fixed(const hebrew_date &date) const {
        auto& [year, month, day] = date;
        const auto lmhy = last_month_of_year(year);
        // Scoped enum hebrew_month converted to int64_t for sigma summation and integer bounds
        const auto p = [=](int64_t m) { return m <= static_cast<int64_t>(lmhy); };
        const auto p2 = [=](int64_t m) { return m < static_cast<int64_t>(month);};
        const auto f = [&](int64_t m) -> int64_t { return last_day_of_month(year, static_cast<hebrew_month>(m)); };
        int64_t a;
        if (month < hebrew_month::tishri)
            a = sigma(p, f, static_cast<int64_t>(hebrew_month::tishri)) + sigma(p2, f, static_cast<int64_t>(hebrew_month::nisan));
        else
            a = sigma(p2, f, static_cast<int64_t>(hebrew_month::tishri));
        return get_new_year(year) + day - 1 + a;
    }

    hebrew_month hebrew_calendar::get_month(const fixed_date_t& date) const {
        return get_month(date, get_year(date));
    }

    int32_t hebrew_calendar::get_day(const fixed_date_t &date) const {
        return to_date(date).day;
    }

    moment_t hebrew_calendar::molad(int32_t year, hebrew_month month) {
        const auto y = month < hebrew_month::tishri ? year + 1 : year;
        const auto months_elapsed = month - hebrew_month::tishri + floor_div(235 * y - 234, 19);
        return moment_t{ ms_epoch } - molad_part + months_elapsed * molad_month_length;
    }

    fixed_date_t hebrew_calendar::fixed_from_molad(const moment_t &moon) {
        const auto r = mod(74377 * moon.days() - 2879.0 / 2160, 7);
        const moment_t a{molad_1_tishri + r * 765433};
        return a.to_fixed_date();
    }

    std::vector<weekday> hebrew_calendar::possible_days(hebrew_month hmonth, int32_t hday) const {
        const auto begin = m_possible_days_begin;
        std::vector basic{weekday::tuesday, weekday::thursday, weekday::saturday};
        const auto hyear = hmonth > hebrew_month::elul ? 6 : 5;
        const hebrew_date hdate{hyear, hmonth, hday};
        const auto n = to_fixed(hdate) - begin;
        std::vector<weekday> extra;
        if (hmonth == hebrew_month::marheshvan && hday == 30)
            extra.clear();
        else if (hmonth == hebrew_month::kislev && hday < 30)
            extra = { weekday::monday, weekday::wednesday, weekday::friday };
        else if (hmonth == hebrew_month::kislev && hday == 30)
            extra = { weekday::monday };
        else if (in(std::array{ hebrew_month::kislev, hebrew_month::shevat }, hmonth) || (hmonth == hebrew_month::adar && hday < 30))
            extra = { weekday::sunday, weekday::monday };
        else
            extra = { weekday::sunday };
        std::ranges::copy(extra, std::back_inserter(basic));
        shift_days(basic, n);
        std::ranges::sort(basic);
        return basic;
    }

    bool hebrew_calendar::is_valid_date(const hebrew_date& date) const {
        if (date.year < 1) {
            return false;
        }
        if (date.month < hebrew_month::nisan || date.month > last_month_of_year(date.year)) {
            return false;
        }
        return date.day >= 1 && date.day <= last_day_of_month(date.year, date.month);
    }
}
