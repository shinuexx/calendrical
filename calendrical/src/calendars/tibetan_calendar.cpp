#include "calendars/tibetan_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::calendars {
    const fixed_date_t tibetan_calendar::ms_epoch = gregorian_calendar::standard.to_fixed({ -127, month::december, 7 });

    rational_angle_t tibetan_calendar::tibetan_sun_equation(const rational_angle_t& a)
    {
        static const std::array<rational, 4> t{ 
            0, 
            rational_angle_t::dms(0, 6), 
            rational_angle_t::dms(0, 10),
            rational_angle_t::dms(0, 1)
        };
        if (a > 6)
            return -tibetan_sun_equation(a - 6);
        else if (a > 3)
            return tibetan_sun_equation(rational{ 6 } - a);
        else if (is_integer(a))
            return t[(int32_t)(rational)a];
        else
            return mod(a, 1) * tibetan_sun_equation(rational_angle_t{ ceil(a) }) + mod(-a, 1) * tibetan_sun_equation(rational_angle_t{ floor(a) });
    }
    rational_angle_t tibetan_calendar::tibetan_moon_equation(const rational_angle_t& a)
    {
        static const std::array<rational, 8> t{
            0,
            rational_angle_t::dms(0,5),
            rational_angle_t::dms(0,10),
            rational_angle_t::dms(0,15),
            rational_angle_t::dms(0,19),
            rational_angle_t::dms(0,22),
            rational_angle_t::dms(0,24),
            rational_angle_t::dms(0,25)
        };
        if (a > 14)
            return -tibetan_moon_equation(a - 14);
        else if (a > 7)
            return tibetan_moon_equation(rational{ 14 } - a);
        else if (is_integer(a))
            return t[(int32_t)(rational)a];
        else
            return mod(a, 1) * tibetan_moon_equation((rational)ceil(a)) + mod(-a, 1) * tibetan_moon_equation((rational)floor(a));
    }
    lunar_date tibetan_calendar::to_date(const fixed_date_t& date) const
    {
        static const rational Y = 365 + rational{ 4975,18382 };
        auto years = (int64_t)ceil((date - ms_epoch) / Y);
        auto year0 = (int32_t)last_passing([=](int64_t y) { return date >= to_fixed({ (int32_t)y, 1, false, 1, false }); }, years);
        auto month0 = (int32_t)last_passing([=](int64_t m) { return date >= to_fixed({ year0, (int32_t)m, false, 1, false }); }, 1);
        auto est = date - to_fixed({ year0, month0, false, 1, false });
        auto day0 = last_passing([=](int64_t d) { return date >= to_fixed({ year0, month0, false, (int32_t)d, false }); }, est - 2);
        auto leap_month = day0 > 30;
        auto day = (int32_t)adjusted_remainder(day0, 30);
        auto month = (int32_t)adjusted_remainder((day > day0) ? (month0 - 1LL) : (leap_month ? month0 + 1LL : month0), 12);
        auto year = (day > day0 && month0 == 1) ? year0 - 1 : (leap_month && month0 == 12 ? year0 + 1 : year0);
        auto leap_day = date == to_fixed({ year, month, leap_month, day, true });
        return { year, month, leap_month, day, leap_day };

    }
    fixed_date_t tibetan_calendar::to_fixed(const lunar_date& date) const
    {
        auto [year, month, leap_month, day, leap_day] = date.tie();
        auto months = (int64_t)floor(rational{ 804,65 } *(year - 1) + rational{ 67,65 } *month + (leap_month ? -1 : 0) + rational{ 64,65 });
        auto days = 30 * months + day;
        auto mean = days * rational{ 11135, 11312 } - 30 + (leap_day ? 0 : -1) + rational{ 1071,1616 };
        auto solar_anomaly = mod(days * rational{ 13,4824 } + rational{ 2117,4824 }, 1);
        auto lunar_anomaly = mod(days * rational{ 3781, 105840 } + rational{ 2837,15120 }, 1);
        auto sun = -tibetan_sun_equation((rational)12 * solar_anomaly);
        auto moon = tibetan_moon_equation((rational)28 * lunar_anomaly);
        return (int64_t)floor((int64_t)ms_epoch + mean + sun + moon);
    }

    int32_t tibetan_calendar::get_month(const fixed_date_t& date) const
    {
        return to_date(date).month;
    }

    bool tibetan_calendar::is_leap_month(const fixed_date_t& date) const
    {
        return to_date(date).leap_month;
    }

    int32_t tibetan_calendar::get_day(const fixed_date_t& date) const
    {
        return to_date(date).day;
    }

    bool tibetan_calendar::is_leap_day(const fixed_date_t& date) const
    {
        return to_date(date).leap_day;
    }

    int32_t tibetan_calendar::get_year(const fixed_date_t& date) const
    {
        return to_date(date).year;
    }

    bool tibetan_calendar::is_leap_month(int32_t year, int32_t month) const
    {
        return month == to_date(to_fixed({ year, month, true, 2, false })).month;
    }

    bool tibetan_calendar::is_leap_day(int32_t year, int32_t month, int32_t day) const
    {
        return day == to_date(to_fixed({ year, month, false, day, true })).day ||
            day == to_date(to_fixed({ year, month, is_leap_month(year, month), day, true })).day;
    }
    fixed_date_t tibetan_calendar::losar(int32_t tyear) const
    {
        auto tleap = is_leap_month(tyear, 1);
        return to_fixed({ tyear, 1, tleap, 1, false });
    }
    std::list<fixed_date_t> tibetan_calendar::new_year(int32_t gyear) const
    {
        auto range = gregorian_calendar::standard.get_year_range(gyear);
        auto tyear = get_year(range.max.subtract(1));
        std::list<fixed_date_t> dates{
            losar(tyear - 1),losar(tyear)
        };
        filter(dates, range);
        return dates;
    }
}