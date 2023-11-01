#include "calendars/modern_hindu_lunar_calendar.hpp"

namespace calendrical::calendars {
    lunar_date modern_hindu_lunar_calendar::to_date(const fixed_date_t& date) const
    {
        auto critical = hindu_sunrise(date);
        auto day = hindu_lunar_day_from_moment(critical);
        auto leap_day = day == hindu_lunar_day_from_moment(hindu_sunrise(date.add(1)));
        auto last_new_moon = hindu_new_moon_before(critical);
        auto next_new_moon = hindu_new_moon_before(floor(last_new_moon) + 35);
        auto solar_month = hindu_zodiac(last_new_moon);
        auto leap_month = solar_month == hindu_zodiac(next_new_moon);
        auto month = (int32_t)adjusted_remainder(solar_month + 1ll, 12);
        auto year = hindu_calendar_year(month <= 2 ? date.add(180) : date) - hindu_lunar_era;
        return {
            year,
            month,
            leap_month,
            day,
            leap_day
        };
    }
    fixed_date_t modern_hindu_lunar_calendar::to_fixed(const lunar_date& date) const
    {
        static const rational half{ 1,2 };
        auto [year, month, leap_month, day, leap_day] = date.tie();
        rational m{ month - 1, 12 };
        rational approx = (int64_t)m_epoch + hindu_sidereal_year * (year + hindu_lunar_era + m);
        auto s = (int64_t)floor(approx - hindu_sidereal_year * interval_modulus(hindu_solar_longitude(approx) / 360 - m, -half, half));
        auto k = hindu_lunar_day_from_moment(s + rational{ 1,4 });
        auto est = s + day;
        auto mid = to_date(s - 15);
        if (range_opened_between(3, 27).contains(k)) {
            est -= k;
        }
        else if (mid.month != month || (mid.leap_month && !leap_month)) {
            est -= interval_modulus(k, -15LL, 15);
        }
        else {
            est -= interval_modulus(k, 15LL, 45);
        }
        auto T = est - interval_modulus(hindu_lunar_day_from_moment(est + rational{ 1,4 }) - day, -15, 15LL);
        std::array<int64_t, 2> dates{ day, adjusted_remainder(day + 1, 30) };
        auto ddate = least_passing([=](int64_t d) { return in(dates, hindu_lunar_day_from_moment(hindu_sunrise(d))); }, T - 1);
        return leap_day ? ddate + 1 : ddate;
    }
    int32_t modern_hindu_lunar_calendar::get_year(const fixed_date_t& date) const
    {
        return to_date(date).year;
    }
    int32_t modern_hindu_lunar_calendar::get_month(const fixed_date_t& date) const
    {
        return to_date(date).month;
    }
    int32_t modern_hindu_lunar_calendar::get_day(const fixed_date_t& date) const
    {
        auto critical = hindu_sunrise(date);
        return hindu_lunar_day_from_moment(critical);
    }
    bool modern_hindu_lunar_calendar::is_leap_month(const fixed_date_t& date) const
    {
        return to_date(date).leap_month;
    }
    bool modern_hindu_lunar_calendar::is_leap_day(const fixed_date_t& date) const
    {
        auto critical = hindu_sunrise(date);
        auto day = hindu_lunar_day_from_moment(critical);
        auto leap_day = day == hindu_lunar_day_from_moment(hindu_sunrise(date.add(1)));
        return leap_day;
    }
    bool modern_hindu_lunar_calendar::is_hindu_expunged(int32_t lyear, int32_t lmonth) const
    {
        return lmonth != to_date(to_fixed({ lyear, lmonth, false, 15, false })).month;
    }
    lunar_date modern_hindu_lunar_calendar::to_full_moon_date(const fixed_date_t& date) const
    {
        auto ldate = to_date(date);
        if (ldate.day >= 16)
            ldate.month = to_date(date.add(20)).month;
        return ldate;
    }
    fixed_date_t modern_hindu_lunar_calendar::to_full_moon_fixed(lunar_date date) const
    {
        int32_t m;
        if (date.leap_month || (date.day <= 15))
            m = date.month;
        else if (is_hindu_expunged(date.year, (int32_t)adjusted_remainder(date.month - 1LL, 12)))
            m = (int32_t)adjusted_remainder(date.month - 2LL, 12);
        else
            m = (int32_t)adjusted_remainder(date.month - 1LL, 12);
        date.month = m;
        return to_fixed(date);
    }
    bool modern_hindu_lunar_calendar::hindu_lunar_on_or_before(const lunar_date& date1, const lunar_date& date2) const
    {
        auto [year1, month1, leap_month1, day1, leap_day1] = date1.tie();
        auto [year2, month2, leap_month2, day2, leap_day2] = date2.tie();
        return year1 < year2 ||
            (year1 == year2 &&
                (month1 < month2 ||
                    (month1 == month2 &&
                        (leap_month1 && !leap_month2) ||
                        (leap_month1 == leap_month2 &&
                            day1 < day2 ||
                            (day1 == day2 && (!leap_day1 || leap_day2))))));                        
    }
}