#include "calendars/babylonian_calendar.hpp"
#include "time_and_astronomy.hpp"
#include "interval.hpp"
#include "lunar_phase.hpp"
#include <cmath>

namespace calendrical::calendars {
    using namespace calendrical::time_and_astronomy;

    bool babylonian_calendar::babylonian_criterion(const fixed_date_t &date) {
        constexpr auto m48 = moment_t::hms(0, 48, 0);
        const auto set = sunset(date.subtract(1), babylon);
        const auto t = universal_from_standard(set, babylon);
        const auto phase = time_and_astronomy::lunar_phase(t);
        return interval_opened_between<angle_t>(lunar_phase::new_moon, lunar_phase::first_quarter).contains(phase) &&
               new_moon_before(t) <= t - 1 &&
               moonlag(date - 1, babylon) > m48;
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    fixed_date_t babylonian_calendar::new_month_on_or_before( // NOLINT(*-convert-member-functions-to-static)
            const fixed_date_t &date) const
    {
        const auto moon = lunar_phase_at_or_before(lunar_phase::new_moon, moment_t{ date }).to_fixed_date();
        const auto age = date - moon;
        const auto T = age <= 3 && !babylonian_criterion(date) ? moon.subtract(30) : moon;
        return fixed_date_t{ least_passing(make_predicate(babylonian_criterion), T.value()) };
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    bool babylonian_calendar::is_leap_year(int32_t year) const // NOLINT(*-convert-member-functions-to-static)
    {
        return mod(7 * year + 13, 19) < 7;
    }

    fixed_date_t babylonian_calendar::to_fixed(const simple_lunar_date& date) const
    {
        auto [year, month, leap, day] = date;
        const auto month1 = (leap || (mod(year, 19) == 18 && month > 6)) ? month : month - 1;
        const auto months = floor_div((year - 1) * 235 + 13, 19) + month1;
        const auto midmonth = ms_epoch + static_cast<int64_t>(std::round(mean_synodic_month * static_cast<double>(months))) + 15;
        return new_month_on_or_before(midmonth) + day - 1;
    }

    simple_lunar_date babylonian_calendar::to_date(const fixed_date_t& date) const
    {
        const auto crescent = new_month_on_or_before(date);
        const auto months = static_cast<int64_t>(std::round(static_cast<double>(crescent - ms_epoch) / mean_synodic_month));
        auto year = static_cast<int32_t>(floor_div(19 * months + 5, 235)) + 1;
        const auto approx =
                ms_epoch + static_cast<int64_t>(std::round(static_cast<double>(floor_div(235 * (year - 1) + 13, 19)) *
                                                           mean_synodic_month));
        const auto new_year = new_month_on_or_before(approx + 15);
        const auto month1 = static_cast<int32_t>(std::round(static_cast<double>(crescent - new_year) / 29.5)) + 1;
        const auto special = mod(year, 19) == 18;
        auto leap = special ? month1 == 7 : month1 == 13;
        auto month = leap || (special && month1 > 6) ? month1 - 1 : month1;
        const auto day = date - crescent + 1;
        return { year, month, leap, static_cast<int32_t>(day) };
    }

    int32_t babylonian_calendar::get_year(const fixed_date_t& date) const
    {
        const auto crescent = new_month_on_or_before(date);
        const auto months = static_cast<int64_t>(std::round(static_cast<double>(crescent - ms_epoch) / mean_synodic_month));
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