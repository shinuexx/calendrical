#include "calendars/tibetan_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"
#include <array>
#include <cmath>

namespace calendrical::calendars {
    const fixed_date_t tibetan_calendar::ms_epoch = gregorian_calendar::standard.to_fixed({ -127, month::december, 7 });

    angle_t tibetan_calendar::tibetan_sun_equation(const angle_t& a)
    {
        static constexpr std::array<angle_t, 4> t{
            0_deg,
            angle_t::dms(0, 6),
            angle_t::dms(0, 10),
            angle_t::dms(0, 11)
        };
        if (a > 6_deg)
            return -tibetan_sun_equation(a - 6_deg);
        if (a > 3_deg)
            return tibetan_sun_equation(6_deg - a);
        if (is_integer(a.degrees())) {
            const auto i = static_cast<int32_t>(a.degrees());
            return t[i];
        }
        const auto ca = ceil(a);
        const auto fa = floor(a);

        return mod(a.degrees(), 1) * tibetan_sun_equation(ca) +
            mod(-a.degrees(), 1) * tibetan_sun_equation(fa);

    }
    angle_t tibetan_calendar::tibetan_moon_equation(const angle_t& a)
    {
        static constexpr std::array<angle_t, 8> t{
            0_deg,
            angle_t::dms(0,5),
            angle_t::dms(0,10),
            angle_t::dms(0,15),
            angle_t::dms(0,19),
            angle_t::dms(0,22),
            angle_t::dms(0,24),
            angle_t::dms(0,25)
        };
        if (a > 14_deg)
            return -tibetan_moon_equation(a - 14_deg);
        if (a > 7_deg)
            return tibetan_moon_equation(14_deg - a);
        if (is_integer(a.degrees())) {
            const auto i = static_cast<int32_t>(a.degrees());
            return t[i];
        }
        const auto ca = ceil(a);
        const auto fa = floor(a);

        return mod(a.degrees(), 1) * tibetan_moon_equation(ca) +
            mod(-a.degrees(), 1) * tibetan_moon_equation(fa);
    }
    lunar_date tibetan_calendar::to_date(const fixed_date_t& date) const
    {
        constexpr double Y = 365 + 4975 / 18382.0;
        const auto years = static_cast<int64_t>(std::ceil((date - ms_epoch) / Y));
        auto year0 = static_cast<int32_t>(
                last_passing([&](const int64_t& y) { return date >= to_fixed({static_cast<int32_t>(y), 1, false, 1, false}); }, years));
        auto month0 = static_cast<int32_t>(last_passing(
                [&](const int64_t& m) { return date >= to_fixed({year0, static_cast<int32_t>(m), false, 1, false}); }, 1));
        const auto est = date - to_fixed({year0, month0, false, 1, false});
        const auto day0 = last_passing([&](const int64_t& d) { return date >= to_fixed({ year0, month0, false, static_cast<int32_t>(d), false }); }, est - 2);
        auto leap_month = day0 > 30;
        auto day = static_cast<int32_t>(adjusted_remainder(day0, 30));
        auto month = static_cast<int32_t>(
                adjusted_remainder(day > day0 ? month0 - 1 : leap_month ? month0 + 1
                                                                          : month0, 12));
        auto year = day > day0 && month0 == 1 ? year0 - 1 : leap_month && month0 == 12 ? year0 + 1 : year0;
        auto leap_day = date == to_fixed({ year, month, leap_month, day, true });
        return { year, month, leap_month, day, leap_day };

    }
    fixed_date_t tibetan_calendar::to_fixed(const lunar_date& date) const
    {
        auto [year, month, leap_month, day, leap_day] = date;
        const auto months = static_cast<int64_t>(std::floor(804 / 65.0 * (year - 1) + 67/65.0 * month +
                                                 (leap_month ? -1 : 0) + 64/ 65.0));
        const auto days = 30 * months + day;
        const auto mean = days * 11135 / 11312.0 - 30 + (leap_day ? 0 : -1) +  1071/1616.0;
        const auto solar_anomaly = mod(days *  13 / 4824.0 +  2117 / 4824.0, 1);
        const auto lunar_anomaly = mod(days *  3781 / 105840.0 + 2837 / 15120.0, 1);
        const auto sun = -tibetan_sun_equation(angle_t{12 * solar_anomaly});
        const auto moon = tibetan_moon_equation(angle_t{28 * lunar_anomaly});
        return fixed_date_t{ static_cast<int64_t>(std::floor(static_cast<int64_t>(ms_epoch) + mean + (sun + moon).degrees())) };
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
    std::vector<fixed_date_t> tibetan_calendar::new_year(int32_t gyear) const
    {
        auto range = gregorian_calendar::standard.get_year_range(gyear);
        const auto tyear = get_year(range.max.subtract(1));
        std::vector dates{
            losar(tyear - 1),losar(tyear)
        };
        auto r = dates | std::views::filter(range);
        return {r.begin(), r.end()};
    }
}