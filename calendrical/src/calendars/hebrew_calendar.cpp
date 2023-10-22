#include "calendars/hebrew_calendar.hpp"

namespace calendrical::calendars {
    const rational hebrew_calendar::molad_part{ 876, 25920 };
    const rational hebrew_calendar::molad_month_length{ 765433, 25920 }; //= 29 + rational{ 1,2 } + rational{ 793,25920 };
    const rational hebrew_calendar::molad_1_tishri = static_cast<int64_t>(hebrew_calendar::m_epoch) - hebrew_calendar::molad_part;

    hebrew_month hebrew_calendar::get_month(const fixed_date_t& date, int32_t year) const
    {
        auto start = date < to_fixed({ year, hebrew_month::nisan, 1 }) ? hebrew_month::tishri : hebrew_month::nisan;
        auto month = least_passing([=](int64_t m) {
            auto hm = static_cast<hebrew_month>(m);
            return date <= to_fixed({ year, hm, last_day_of_month(year, hm) });
            }, static_cast<int64_t>(start));
        return static_cast<hebrew_month>(month);
    }
    int64_t hebrew_calendar::year_length_correction(int32_t year) const
    {
        auto ny0 = elapsed_days(year - 1);
        auto ny1 = elapsed_days(year);
        auto ny2 = elapsed_days(year + 1);
        if ((ny2 - ny1) == 356) return 2;
        else if ((ny1 - ny0) == 382) return 1;
        else return 0;
    }
    int64_t hebrew_calendar::elapsed_days(int32_t year) const
    {
        auto months_elapsed = floor_div(235ll * year - 234, 19);
        auto parts_elapsed = 12084 + 13753 * months_elapsed;
        auto days = 29 * months_elapsed + floor_div(parts_elapsed, 25920);
        return mod(3 * (days + 1), 7) < 3 ? days + 1 : days;
    }
    hebrew_calendar::hebrew_calendar() : m_possible_days_begin{ to_fixed({ 5, hebrew_month::nisan, 1 }) }
    {
    }
    bool hebrew_calendar::is_leap_year(int32_t year) const
    {
        return mod(7ll * year + 1, 19) < 7;
    }

    bool hebrew_calendar::is_sabbatical_year(int32_t year) const
    {
        return mod(year, 7ll) == 0;
    }

    fixed_date_t hebrew_calendar::get_new_year(int32_t year) const
    {
        return m_epoch + elapsed_days(year) + year_length_correction(year);
    }

    int32_t hebrew_calendar::get_year(const fixed_date_t& date) const
    {
        auto approx = floor_div(98496 * (date - m_epoch), 35975351) + 1;
        auto year = last_passing([=](int64_t y) { return get_new_year(static_cast<int32_t>(y)) <= date; }, approx - 1);
        return static_cast<int32_t>(year);
    }

    hebrew_date hebrew_calendar::to_date(const fixed_date_t& date) const
    {
        auto year = get_year(date);
        auto month = get_month(date, year);
        auto day = date - to_fixed({ year, month, 1 }) + 1;
        return { year, month, static_cast<int32_t>(day) };
    }

    fixed_date_t hebrew_calendar::to_fixed(const hebrew_date& date) const
    {
        auto [year, month, day] = date.tie();
        auto lmhy = last_month_of_year(year);
        auto p = [=](int64_t m) { return m < static_cast<int64_t>(lmhy); };
        auto p2 = [=](int64_t m) { return m < static_cast<int64_t>(month); };
        auto f = [=](int64_t m) { return static_cast<int64_t>(last_day_of_month(year, static_cast<hebrew_month>(m))); };
        int64_t a;
        if (month < hebrew_month::tishri)
            a = sigma(p, f, static_cast<int64_t>(hebrew_month::tishri))
            + sigma(p2, f, static_cast<int64_t>(hebrew_month::nisan));
        else
            a = sigma(p2, f, static_cast<int64_t>(hebrew_month::tishri));
        return get_new_year(year) + day - 1 + a;
    }

    hebrew_month hebrew_calendar::get_month(const fixed_date_t& date) const
    {
        return get_month(date, get_year(date));
    }

    int32_t hebrew_calendar::get_day(const fixed_date_t& date) const
    {
        return to_date(date).day;
    }

    rational_moment_t hebrew_calendar::molad(int32_t year, hebrew_month month)
    {
        auto y = month < hebrew_month::tishri ? year + 1 : year;
        auto months_elapsed = (month - hebrew_month::tishri) + floor_div(235ll * y - 234, 19);
        rational r = static_cast<int64_t>(m_epoch) - molad_part + months_elapsed * molad_month_length;
        return r;
    }

    fixed_date_t hebrew_calendar::fixed_from_molad(const rational_moment_t& moon)
    {
        auto r = mod(74377 * (rational)moon - rational{ 2879,2160 }, 7);
        rational_moment_t a = (rational)(molad_1_tishri + r * 765433);
        return (fixed_date_t)a;
    }

    std::vector<weekday> hebrew_calendar::possible_days(hebrew_month hmonth, int32_t hday) const
    {
        auto begin = m_possible_days_begin;
        std::vector<weekday> basic{ weekday::tuesday, weekday::thursday, weekday::saturday };
        auto hyear = hmonth > hebrew_month::elul ? 6 : 5;
        hebrew_date hdate{ hyear, hmonth, hday };
        auto n = to_fixed(hdate) - begin;
        std::vector<weekday> extra;
        if ((hmonth == hebrew_month::marheshvan) && (hday == 30))
            extra.clear();
        else if ((hmonth == hebrew_month::kislev) && (hday < 30))
            extra = { weekday::monday, weekday::wednesday, weekday::friday };
        else if ((hmonth == hebrew_month::kislev) && (hday == 30))
            extra = { weekday::monday };
        else if (in(std::array{ hebrew_month::kislev, hebrew_month::shevat }, hmonth) || ((hmonth == hebrew_month::adar) && (hday < 30)))
            extra = { weekday::sunday, weekday::monday };
        else
            extra = { weekday::sunday };
        join_in_place(basic, extra);
        shift_days(basic, n);
        std::ranges::sort(basic);
        return basic;
    }

    
}