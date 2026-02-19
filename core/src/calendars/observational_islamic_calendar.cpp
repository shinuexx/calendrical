#include "calendars/observational_islamic_calendar.hpp"
#include "time_and_astronomy.hpp"
#include <cmath>

namespace calendrical::calendars{
    using namespace calendrical::time_and_astronomy;

    // ReSharper disable once CppMemberFunctionMayBeStatic
    fixed_date_t observational_islamic_calendar::get_crescent(const fixed_date_t& date) const // NOLINT(*-convert-member-functions-to-static)
    {
        return phasis_on_or_before(date, islamic_location);
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t observational_islamic_calendar::get_elapsed_months(const fixed_date_t& crescent) const // NOLINT(*-convert-member-functions-to-static)
    {
        return static_cast<int32_t>(std::round(static_cast<double>(crescent - ms_epoch) / mean_synodic_month));
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t observational_islamic_calendar::get_year(int32_t elapsed_months) const // NOLINT(*-convert-member-functions-to-static)
    {
        return static_cast<int32_t>(floor_div(elapsed_months, 12)) + 1;
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t observational_islamic_calendar::get_month(int32_t elapsed_months) const // NOLINT(*-convert-member-functions-to-static)
    {
        return static_cast<int32_t>(mod(elapsed_months, 12) + 1);
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t observational_islamic_calendar::get_day(const fixed_date_t& date, const fixed_date_t& crescent) const // NOLINT(*-convert-member-functions-to-static)
    {
        return static_cast<int32_t>(date - crescent + 1);
    }
    int32_t observational_islamic_calendar::get_year(const fixed_date_t& date) const
    {
        return get_year(get_elapsed_months(get_crescent(date)));
    }
    simple_date observational_islamic_calendar::to_date(const fixed_date_t& date) const
    {
        const auto crescent = get_crescent(date);
        const auto elapsed_months = get_elapsed_months(crescent);
        auto year = get_year(elapsed_months);
        auto month = get_month(elapsed_months);
        auto day = get_day(date, crescent);
        return { year, month, day };
    }
    fixed_date_t observational_islamic_calendar::to_fixed(const simple_date& date) const
    {
        auto [year, month, day] = date;
        const auto m = 12 * (year - 1) + month - 0.5;
        const auto midmonth = ms_epoch + static_cast<int64_t>(std::floor(m * mean_synodic_month));
        return phasis_on_or_before(midmonth, islamic_location) + day - 1;
    }
    int32_t observational_islamic_calendar::get_month(const fixed_date_t& date) const
    {
        return get_month(get_elapsed_months(get_crescent(date)));
    }
    int32_t observational_islamic_calendar::get_day(const fixed_date_t& date) const
    {
        return get_day(date, get_crescent(date));
    }
    bool observational_islamic_calendar::is_leap_year(int32_t year) const
    {
        return to_fixed({year + 1, 1, 1}) - to_fixed({year, 1, 1}) > 354;
    }
}