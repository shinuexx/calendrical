#include "calendars/chinese_base.hpp"
#include "calendars/gregorian_calendar.hpp"
#include "time_and_astronomy.hpp"
#include "season.hpp"

namespace calendrical::calendars {
    
    using namespace calendrical::time_and_astronomy;

    location chinese_base::chinese_location(const moment_t& t)
    {
        auto year = gregorian_calendar::standard.get_year(t.to_fixed_date());
        if (year < 1929)
            return { angle_t::dms(39, 55), angle_t::dms(116,25), 43.5, moment_t::hms(1397 / 1.8e2) };
        else
            return { angle_t::dms(39, 55), angle_t::dms(116,25), 43.5, moment_t::hms(8) };
    }
    int32_t chinese_base::current_major_solar_term(const fixed_date_t& date) const
    {
        auto s = solar_longitude(universal_from_standard(date, get_location(date)));
        return (int32_t)adjusted_remainder(2 + (int64_t)floor_div(s, 30), 12);
    }
    moment_t chinese_base::solar_longitude_on_or_after(const angle_t& l, const moment_t& t) const
    {
        auto loc = get_location(t);
        auto sun = solar_longitude_after(l, universal_from_standard(t, loc));
        return standard_from_universal(sun, loc);
    }
    moment_t chinese_base::major_solar_term_on_or_after(const fixed_date_t& date) const
    {
        auto s = solar_longitude(midnight_in_china(date));
        auto l = mod(30 * std::ceil(s / 30_deg), 360);
        return solar_longitude_on_or_after(l, date);
    }
    moment_t chinese_base::minor_solar_term_on_of_after(const fixed_date_t& date) const
    {
        auto s = solar_longitude(midnight_in_china(date));
        auto l = mod(30 * std::ceil((s - 15_deg) / 30_deg) + 15, 360);
        return solar_longitude_on_or_after(l, date);
    }
    moment_t chinese_base::midnight_in_china(const fixed_date_t& date) const
    {
        return universal_from_standard(date, get_location(date));
    }
    fixed_date_t chinese_base::winter_solstice_on_or_before(const fixed_date_t& date) const
    {
        auto approx = estimate_prior_solar_longitude(season::WINTER, midnight_in_china(date + 1));
        return least_passing([=](int64_t day) { return season::WINTER < solar_longitude(midnight_in_china(day + 1)); }, (int64_t)std::floor(approx) - 1);
    }
    fixed_date_t chinese_base::new_moon_on_or_after(const fixed_date_t& date) const
    {
        auto t = time_and_astronomy::new_moon_on_or_after(midnight_in_china(date));
        return (int64_t)std::floor(standard_from_universal(t, get_location(t)));
    }
    fixed_date_t chinese_base::new_moon_before(const fixed_date_t& date) const
    {
        auto t = time_and_astronomy::new_moon_before(midnight_in_china(date));
        return (int64_t)std::floor(standard_from_universal(t, get_location(t)));
    }
    bool chinese_base::no_major_solar_term(const fixed_date_t& date) const
    {
        return current_major_solar_term(date) == current_major_solar_term(new_moon_on_or_after(date + 1));
    }
    bool chinese_base::is_prior_leap_month(const fixed_date_t& m1, const fixed_date_t& m) const
    {
        return (m >= m1) && (no_major_solar_term(m) || is_prior_leap_month(m1, new_moon_before(m)));
    }
    fixed_date_t chinese_base::new_year_in_sui(const fixed_date_t& date) const
    {
        auto s1 = winter_solstice_on_or_before(date);
        auto s2 = winter_solstice_on_or_before(s1 + 370);
        auto m12 = new_moon_on_or_after(s1 + 1);
        auto m13 = new_moon_on_or_after(m12 + 1);
        auto next_m11 = new_moon_before(s2 + 1);
        if (std::round((next_m11 - m12) / mean_synodic_month) == 12 && (no_major_solar_term(m12) || no_major_solar_term(m13)))
            return new_moon_on_or_after(m13 + 1);
        else
            return m13;
    }
    fixed_date_t chinese_base::new_year_on_or_before(const fixed_date_t& date) const
    {
        auto new_year = new_year_in_sui(date);
        if (date >= new_year)
            return new_year;
        else
            return new_year_in_sui(date.subtract(180));
    }
}