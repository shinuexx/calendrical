#include "calendars/saudi_islamic_calendar.hpp"
#include "time_and_astronomy.hpp"
#include "location.hpp"
#include "lunar_phase.hpp"
#include "interval.hpp"
#include <cmath>

namespace calendrical::calendars {

    using namespace calendrical::time_and_astronomy;

    bool saudi_islamic_calendar::saudi_criterion(const fixed_date_t& date)
    {
        const auto set = sunset(date - 1, locations::mecca);
        const auto t = universal_from_standard(set, locations::mecca);
        const auto phase = time_and_astronomy::lunar_phase(t);
        return interval_opened_between<angle_t>(lunar_phase::new_moon, lunar_phase::first_quarter).contains(phase)
            && moonlag(date - 1, locations::mecca) > moment_t{ 0 };
    }
    fixed_date_t saudi_islamic_calendar::new_month_on_or_before(const fixed_date_t& date)
    {
        const auto moon = lunar_phase_at_or_before(lunar_phase::new_moon, moment_t{ date }).to_fixed_date();
        const auto age = date - moon;
        const auto T = age <= 3 && !saudi_criterion(date) ? moon.subtract(30) : moon;
        return fixed_date_t{ least_passing(make_predicate(saudi_criterion), T.value()) };
    }
    fixed_date_t saudi_islamic_calendar::to_fixed(const simple_date& date) const
    {
        auto &[year, month, day] = date;
        const auto m = 12 * (year - 1) + month - 0.5;
        const auto midmonth = ms_epoch + static_cast<int64_t>(std::floor(m * mean_synodic_month));
        return new_month_on_or_before(midmonth) + day - 1;
    }
    bool saudi_islamic_calendar::is_leap_year(int32_t year) const {
        return to_fixed({year + 1, 1, 1}) - to_fixed({year, 1, 1}) > 354;
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    fixed_date_t saudi_islamic_calendar::get_crescent(const fixed_date_t &date) const {
        return new_month_on_or_before(date);
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t saudi_islamic_calendar::get_elapsed_months(const fixed_date_t &crescent) const {
        return static_cast<int32_t>(std::round((crescent - ms_epoch) / mean_synodic_month));
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t saudi_islamic_calendar::get_year(int32_t elapsed_months) const {
        return static_cast<int32_t>(floor_div(elapsed_months, 12)) + 1;
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t saudi_islamic_calendar::get_month(int32_t elapsed_months) const {
        return static_cast<int32_t>(mod(elapsed_months, 12) + 1);
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t saudi_islamic_calendar::get_day(const fixed_date_t& date, const fixed_date_t& crescent) const
    {
        return static_cast<int32_t>(date - crescent + 1);
    }
    int32_t saudi_islamic_calendar::get_year(const fixed_date_t& date) const
    {
        return get_year(get_elapsed_months(get_crescent(date)));
    }
    simple_date saudi_islamic_calendar::to_date(const fixed_date_t& date) const
    {
        const auto crescent = get_crescent(date);
        const auto elapsed_months = get_elapsed_months(crescent);
        auto year = get_year(elapsed_months);
        auto month = get_month(elapsed_months);
        auto day = get_day(date, crescent);
        return { year, month, day };
    }
    int32_t saudi_islamic_calendar::get_month(const fixed_date_t& date) const
    {
        return get_month(get_elapsed_months(get_crescent(date)));
    }
    int32_t saudi_islamic_calendar::get_day(const fixed_date_t& date) const
    {
        return get_day(date, get_crescent(date));
    }
}