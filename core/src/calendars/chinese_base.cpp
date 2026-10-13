#include "calendars/chinese_base.hpp"
#include "calendars/gregorian_calendar.hpp"
#include "time_and_astronomy.hpp"
#include "season.hpp"
#include <cmath>

namespace calendrical::calendars{
    using namespace calendrical::time_and_astronomy;

    location chinese_base::chinese_location(const moment_t& t)
    {
        if (const auto year = gregorian_calendar::standard.get_year(t.to_fixed_date()); year < 1929)
            return {angle_t::dms(39, 55), angle_t::dms(116, 25), 43.5, moment_t::hms(1397 / 1.8e2)};
        return {angle_t::dms(39, 55), angle_t::dms(116, 25), 43.5, moment_t::hms(8)};
    }
    int32_t chinese_base::current_major_solar_term(const fixed_date_t& date) const
    {
        const auto s = solar_longitude(universal_from_standard(moment_t{ date }, get_location(moment_t{ date })));
        return static_cast<int32_t>(adjusted_remainder(2 + static_cast<int64_t>(floor_div(s, 30)), 12));
    }
    moment_t chinese_base::solar_longitude_on_or_after(const angle_t& l, const moment_t& t) const
    {
        const auto loc = get_location(t);
        const auto sun = solar_longitude_after(l, universal_from_standard(t, loc));
        return standard_from_universal(sun, loc);
    }
    moment_t chinese_base::major_solar_term_on_or_after(const fixed_date_t& date) const
    {
        const auto s = solar_longitude(midnight_in_china(date));
        const auto l = angle_t{mod(30 * std::ceil(s / 30_deg), 360)};
        return solar_longitude_on_or_after(l, moment_t{ date });
    }
    moment_t chinese_base::minor_solar_term_on_of_after(const fixed_date_t& date) const
    {
        const auto s = solar_longitude(midnight_in_china(date));
        const auto l = angle_t{mod(30 * std::ceil((s - 15_deg) / 30_deg) + 15, 360)};
        return solar_longitude_on_or_after(l, moment_t{ date });
    }
    moment_t chinese_base::midnight_in_china(const fixed_date_t& date) const
    {
        return universal_from_standard(moment_t{ date }, get_location(moment_t{ date }));
    }
    fixed_date_t chinese_base::winter_solstice_on_or_before(const fixed_date_t& date) const
    {
        const auto approx = estimate_prior_solar_longitude(season::winter, midnight_in_china(date + 1));
        return fixed_date_t{ least_passing([&](const int64_t& day) { return angle_t{season::winter} < solar_longitude(midnight_in_china(fixed_date_t{ day + 1 })); }, static_cast<int64_t>(std::floor(approx)) - 1) };
    }
    fixed_date_t chinese_base::new_moon_on_or_after(const fixed_date_t& date) const
    {
        const auto t = time_and_astronomy::new_moon_on_or_after(midnight_in_china(date));
        return fixed_date_t{ static_cast<int64_t>(std::floor(standard_from_universal(t, get_location(t)))) };
    }
    fixed_date_t chinese_base::new_moon_before(const fixed_date_t& date) const
    {
        const auto t = time_and_astronomy::new_moon_before(midnight_in_china(date));
        return fixed_date_t{ static_cast<int64_t>(std::floor(standard_from_universal(t, get_location(t)))) };
    }
    bool chinese_base::no_major_solar_term(const fixed_date_t& date) const
    {
        return current_major_solar_term(date) == current_major_solar_term(new_moon_on_or_after(date + 1));
    }
    bool chinese_base::is_prior_leap_month(const fixed_date_t& m1, const fixed_date_t& m) const // NOLINT(*-no-recursion)
    {
        return m >= m1 && (no_major_solar_term(m) || is_prior_leap_month(m1, new_moon_before(m)));
    }
    fixed_date_t chinese_base::new_year_in_sui(const fixed_date_t& date) const
    {
        const auto s1 = winter_solstice_on_or_before(date);
        const auto s2 = winter_solstice_on_or_before(s1 + 370);
        const auto m12 = new_moon_on_or_after(s1 + 1);
        const auto m13 = new_moon_on_or_after(m12 + 1);
        if (const auto next_m11 = new_moon_before(s2 + 1); std::round(static_cast<double>(next_m11 - m12) / mean_synodic_month) == 12 &&
                                                           (no_major_solar_term(m12) || no_major_solar_term(m13)))
            return new_moon_on_or_after(m13 + 1);
        return m13;
    }
    fixed_date_t chinese_base::new_year_on_or_before(const fixed_date_t& date) const
    {
        if (const auto new_year = new_year_in_sui(date); date >= new_year)
            return new_year;
        return new_year_in_sui(date.subtract(180));
    }
}