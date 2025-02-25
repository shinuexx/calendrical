#include "calendars/french_calendar.hpp"
#include "time_and_astronomy.hpp"
#include "season.hpp"

namespace calendrical::calendars{
    using namespace calendrical::time_and_astronomy;

    // ReSharper disable once CppMemberFunctionMayBeStatic
    moment_t french_calendar::midnight_in_paris(const fixed_date_t& date) const
    {
        return midnight(date.add(1), paris);
    }

    fixed_date_t french_calendar::new_year_on_or_before(const fixed_date_t& date) const
    {
        const auto approx = estimate_prior_solar_longitude(season::AUTUMN, midnight_in_paris(date));
        return least_passing([&](const int64_t& day) { return angle_t{season::AUTUMN} <= solar_longitude(midnight_in_paris(day)); }, static_cast<int64_t>(std::floor(approx) - 1));
    }

    fixed_date_t french_calendar::get_new_year(int32_t year) const
    {
        auto approx = std::floor(ms_epoch + 180 + mean_tropical_year * (year - 1));
        return new_year_on_or_before(static_cast<int64_t>(approx));
    }
    int32_t french_calendar::get_year(const fixed_date_t& date) const
    {
        const auto new_year = new_year_on_or_before(date);
        return static_cast<int32_t>(std::round((new_year - ms_epoch) / mean_tropical_year) + 1);
    }
    simple_date french_calendar::to_date(const fixed_date_t& date) const
    {
        const auto new_year = new_year_on_or_before(date);
        auto year = static_cast<int32_t>(std::round((new_year - ms_epoch) / mean_tropical_year) + 1);
        auto month = get_month(date, new_year);
        auto day = get_day(date, new_year);
        return { year, month, day };
    }
    fixed_date_t french_calendar::to_fixed(const simple_date& date) const
    {
        auto [year, month, day] = date;
        const auto new_year = get_new_year(year);
        return new_year - 1
            + 30 * (month - 1)
            + day;
    }
    int32_t french_calendar::get_month(const fixed_date_t& date) const
    {
        return get_month(date, new_year_on_or_before(date));
    }
    int32_t french_calendar::get_day(const fixed_date_t& date) const
    {
        return get_day(date, new_year_on_or_before(date));
    }
    bool french_calendar::is_leap_year(int32_t year) const
    {
        return get_new_year(year + 1) - get_new_year(year) > 365;
    }
}