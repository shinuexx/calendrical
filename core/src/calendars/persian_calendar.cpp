#include "calendars/persian_calendar.hpp"
#include "time_and_astronomy.hpp"
#include "season.hpp"
#include <cmath>

namespace calendrical::calendars {
    using namespace calendrical::time_and_astronomy;

    // ReSharper disable once CppMemberFunctionMayBeStatic
    moment_t persian_calendar::midday_in_tehran(const fixed_date_t& date) const // NOLINT(*-convert-member-functions-to-static)
    {
        return midday(date, tehran);
    }
    fixed_date_t persian_calendar::new_year_on_or_before(const fixed_date_t& date) const
    {
        const auto approx = estimate_prior_solar_longitude(season::spring, midday_in_tehran(date));
        return least_passing([&](const int64_t &day) { return solar_longitude(midday_in_tehran(day)) <= angle_t{ season::spring } + 2_deg; }, static_cast<int64_t>(std::floor(approx)) - 2);
    }
    fixed_date_t persian_calendar::get_new_year(int32_t year) const
    {
        return to_fixed({year, 1, 1});
    }
    int32_t persian_calendar::get_year(const fixed_date_t& date) const
    {
        const auto new_year = new_year_on_or_before(date);
        const auto y = std::round(static_cast<double>(new_year - ms_epoch) / mean_tropical_year) + 1;
        const auto year = 0 < y ? y : y - 1;
        return static_cast<int32_t>(year);
    }
    fixed_date_t persian_calendar::to_fixed(const simple_date& date) const
    {
        auto &[year, month, day] = date;
        const auto y = 0 < year ? year - 1 : year;
        const int64_t beg = ms_epoch + 180 + static_cast<int64_t>(std::floor(mean_tropical_year * y));
        const auto new_year = new_year_on_or_before(beg);
        return new_year - 1
            + (month <= 7 ? 31 * (month - 1) : 30 * (month - 1) + 6)
            + day;
    }
}