#include "calendars/chinese_based_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"
#include "time_and_astronomy.hpp"

namespace calendrical::calendars {

    using namespace calendrical::time_and_astronomy;

    chinese_based_calendar::chinese_based_calendar()
    {
        m_epoch = gregorian_calendar::standard.to_fixed({ -2636, month::february, 15 });
    }
    chinese_date chinese_based_calendar::to_date(const fixed_date_t& date) const
    {
        auto s1 = winter_solstice_on_or_before(date);
        auto s2 = winter_solstice_on_or_before(s1.add(370));
        auto m12 = new_moon_on_or_after(s1.add(1));
        auto next_m11 = new_moon_before(s2.add(1));
        auto m = new_moon_before(date.add(1));
        auto leap_year = std::round((next_m11 - m12) / mean_synodic_month) == 12;
        auto month = mod((int64_t)std::round((m - m12) / mean_synodic_month) - (leap_year && is_prior_leap_month(m12, m) ? 1 : 0), 12);
        auto leap_month = leap_year && no_major_solar_term(m) && !is_prior_leap_month(m12, new_moon_before(m));
        auto elapsed_years = (int64_t)std::floor(1.5 - month / 12.0 + (date - m_epoch) / mean_tropical_year);
        auto cycle = floor_div(elapsed_years - 1, 60) + 1;
        auto year = adjusted_remainder(elapsed_years, 60);
        auto day = date - m + 1;
        return {
            static_cast<int32_t>(cycle),
            static_cast<int32_t>(year),
            static_cast<int32_t>(month),
            leap_month,
            static_cast<int32_t>(day)
        };
    }
    fixed_date_t chinese_based_calendar::to_fixed(const chinese_date& date) const
    {
        auto [cycle, year, month, leap_month, day] = date.tie();
        auto mid_year = std::floor(m_epoch + mean_tropical_year * ((cycle - 1) * 60 + year - 1 + 0.5));
        auto new_year = new_year_on_or_before(mid_year);
        auto p = new_moon_on_or_after(new_year + (month - 1) * 29);
        auto d = to_date(p);
        auto prior_new_moon = (month == d.month && leap_month == d.leap_month) ? p : new_moon_on_or_after(p.add(1));
        return prior_new_moon + day - 1;
    }
    int32_t chinese_based_calendar::get_cycle(const fixed_date_t& date) const
    {
        return to_date(date).cycle;
    }
    int32_t chinese_based_calendar::get_year(const fixed_date_t& date) const
    {
        return to_date(date).year;
    }
    int32_t chinese_based_calendar::get_month(const fixed_date_t& date) const
    {
        return to_date(date).month;
    }
    bool chinese_based_calendar::is_leap_month(const fixed_date_t& date) const
    {
        return to_date(date).leap_month;;
    }
    int32_t chinese_based_calendar::get_day(const fixed_date_t& date) const
    {
        return to_date(date).day;
    }
    sexagesimal_name chinese_based_calendar::year_name(int32_t n)
    {
        return sexagesimal_name::from(n);
    }
    sexagesimal_name chinese_based_calendar::month_name(int32_t month, int32_t year)
    {
        auto elapsed_months = 12 * (year - 1) + month - 1;
        return sexagesimal_name::from(elapsed_months - month_name_epoch);
    }
    sexagesimal_name chinese_based_calendar::day_name(const fixed_date_t& date)
    {
        return sexagesimal_name::from(date - day_name_epoch);
    }

    fixed_date_t chinese_based_calendar::day_name_on_or_before(const sexagesimal_name& name, const fixed_date_t& date)
    {
        return interval_modulus(day_name_zero.difference(name), date, date.subtract(60));
    }

    const sexagesimal_name chinese_based_calendar::day_name_zero{ chinese_based_calendar::day_name(0) };

}