#include "calendars/astro_bahai_calendar.hpp"
#include "time_and_astronomy.hpp"
#include "season.hpp"

namespace calendrical::calendars{
    using namespace calendrical::time_and_astronomy;

    int32_t astro_bahai_calendar::get_years(const fixed_date_t &date) const {
        const auto new_year = new_year_on_or_before(date);
        return static_cast<int32_t>(std::round(static_cast<double>(new_year - ms_epoch) / mean_tropical_year));
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    moment_t astro_bahai_calendar::bahai_sunset(const fixed_date_t& date) const // NOLINT(*-convert-member-functions-to-static)
    {
        return universal_from_standard(sunset(date, bahai_location), bahai_location);
    }
    fixed_date_t astro_bahai_calendar::new_year_on_or_before(const fixed_date_t& date) const
    {
        const auto approx = estimate_prior_solar_longitude(season::SPRING, bahai_sunset(date));
        return least_passing([&](const int64_t& day) { return solar_longitude(bahai_sunset(day)) <= angle_t{ season::SPRING } + 2_deg; }, static_cast<int64_t>(std::floor(approx)) - 1);
    }
    bahai_date astro_bahai_calendar::to_date(const fixed_date_t& date) const
    {
        const auto new_year = new_year_on_or_before(date);
        const auto years = static_cast<int32_t>(std::round(static_cast<double>(new_year - ms_epoch) / mean_tropical_year));
        auto major = get_major(years);
        auto cycle = get_cycle(years);
        auto year = get_year(years);
        auto month = get_month(date, major, cycle, year);
        auto day = get_day(date, major, cycle, year, month);
        return { major, cycle, year, month, day };
    }
    fixed_date_t astro_bahai_calendar::to_fixed(const bahai_date& date) const
    {
        auto [major, cycle, year, month, day] = date;
        const auto years = 361 * (major - 1) + 19 * (cycle - 1) + year;
        int64_t a;
        int32_t b;
        if (month == 19) {
            const auto x = static_cast<int64_t>(std::floor(mean_tropical_year * (years + 0.5)));
            a = new_year_on_or_before(ms_epoch + x);
            b = -20 + day;
        }
        else if (month == ayyam_i_ha) {
            const auto x = static_cast<int64_t>(std::floor(mean_tropical_year * (years - 0.5)));
            a = new_year_on_or_before(ms_epoch + x);
            b = 341 + day;
        }
        else {
            const auto x = static_cast<int64_t>(std::floor(mean_tropical_year * (years - 0.5)));
            a = new_year_on_or_before(ms_epoch + x);
            b = 19 * (month - 1) + day - 1;
        }
        return a + b;
    }
}