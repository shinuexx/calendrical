#include "calendars/babylonian_calendar.hpp"
#include "time_and_astronomy.hpp"
#include "range.hpp"
#include "lunar_phase.hpp"

namespace calendrical::calendars {
    using namespace calendrical::time_and_astronomy;

    bool babylonian_calendar::babylonian_criterion(const fixed_date_t& date)
    {
        constexpr auto m48 = moment_t::hms(0, 48, 0);
        auto set = sunset(date.subtract(1), babylon);
        auto t = universal_from_standard(set, babylon);
        auto phase = time_and_astronomy::lunar_phase(t);
        return range_opened_between<angle_t>(calendrical::lunar_phase::NEW, calendrical::lunar_phase::FIRST_QUARTER).contains(phase)
            && ((double)new_moon_before(t) <= ((double)t - 1))
            && (moonlag(date - 1, babylon) > m48);
    }
    fixed_date_t babylonian_calendar::new_month_on_or_before(const fixed_date_t& date) const
    {
        auto moon = lunar_phase_at_or_before(lunar_phase::NEW, date).to_fixed_date();
        auto age = date - moon;
        auto T = age <= 3 && !babylonian_criterion(date) ? moon.subtract(30) : moon;
        return least_passing(make_predicate(babylonian_criterion), T);
    }

    bool babylonian_calendar::is_leap_year(int32_t year) const
    {
        return mod(7LL * year + 13, 19) < 7;
    }

    fixed_date_t babylonian_calendar::to_fixed(const simple_lunar_date& date) const
    {
        auto [year, month, leap, day] = date.tie();
        auto month1 = leap || (mod(year, 19LL) == 18 && (month > 6)) ? month : month - 1;
        auto months = floor_div((year - 1LL) * 235 + 13, 19) + month1;
        auto midmonth = ms_epoch + static_cast<int64_t>(std::round(mean_synodic_month * months)) + 15;
        return new_month_on_or_before(midmonth) + day - 1;
    }

    simple_lunar_date babylonian_calendar::to_date(const fixed_date_t& date) const
    {
        auto crescent = new_month_on_or_before(date);
        auto months = static_cast<int64_t>(std::round((crescent - ms_epoch) / mean_synodic_month));
        auto year = static_cast<int32_t>(floor_div(19 * months + 5, 235)) + 1;
        auto approx = ms_epoch + static_cast<int64_t>(std::round(floor_div(235 * (year - 1LL) + 13, 19) * mean_synodic_month));
        auto new_year = new_month_on_or_before(approx + 15);
        auto month1 = static_cast<int32_t>(std::round((crescent - new_year) / 29.5)) + 1;
        auto special = mod(year, 19LL) == 18;
        auto leap = special ? month1 == 7 : month1 == 13;
        auto month = leap || (special && month1 > 6) ? month1 - 1 : month1;
        auto day = date - crescent + 1;
        return { year, month, leap, static_cast<int32_t>(day) };
    }

    int32_t babylonian_calendar::get_year(const fixed_date_t& date) const
    {
        auto crescent = new_month_on_or_before(date);
        auto months = static_cast<int64_t>(std::round((crescent - ms_epoch) / mean_synodic_month));
        return static_cast<int32_t>(floor_div(19 * months + 5, 235)) + 1;
    }
    int32_t babylonian_calendar::get_month(const fixed_date_t& date) const
    {
        return to_date(date).month;
    }
    bool babylonian_calendar::is_leap_month(const fixed_date_t& date) const
    {
        return to_date(date).leap_month;
    }
    int32_t babylonian_calendar::get_day(const fixed_date_t& date) const
    {
        return to_date(date).day;
    }
}