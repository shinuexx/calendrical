#include "calendars/old_hindu_solar_calendar.hpp"
#include <cmath>

namespace calendrical::calendars {
    static constexpr auto quarter{ 0.25 };

    double old_hindu_solar_calendar::get_sun(const fixed_date_t& date) const {
        return day_count(date) + quarter;
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t old_hindu_solar_calendar::get_year(const double& sun) const {
        const auto y = floor_div(sun, arya_solar_year);
        return static_cast<int32_t>(y);
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t old_hindu_solar_calendar::get_month(const double& sun) const {
        const auto m = mod(floor_div(sun, arya_solar_month), 12) + 1;
        return static_cast<int32_t>(m);
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    int32_t old_hindu_solar_calendar::get_day(const double& sun) const {
        const auto d = std::floor(mod(sun, arya_solar_month)) + 1;
        return static_cast<int32_t>(d);
    }

    fixed_date_t old_hindu_solar_calendar::get_new_year(int32_t year) const {
        return to_fixed({year, 1, 1});
    }

    int32_t old_hindu_solar_calendar::get_year(const fixed_date_t& date) const {
        return get_year(get_sun(date));
    }

    static constexpr double h30{30 / 24.0};

    simple_date old_hindu_solar_calendar::to_date(const fixed_date_t& date) const {
        const auto sun = get_sun(date);
        auto year = get_year(sun);
        auto month = get_month(sun);
        auto day = get_day(sun);
        return {year, month, day};
    }

    fixed_date_t old_hindu_solar_calendar::to_fixed(const simple_date& date) const {
        auto& [year, month, day] = date;
        const double a = static_cast<int64_t>(ms_epoch)
            + year * arya_solar_year
            + (month - 1) * arya_solar_month
            + day
            - h30;
        return static_cast<int64_t>(std::ceil(a));
    }

    int32_t old_hindu_solar_calendar::get_month(const fixed_date_t& date) const {
        return get_month(get_sun(date));
    }

    int32_t old_hindu_solar_calendar::get_day(const fixed_date_t& date) const {
        return get_day(get_sun(date));
    }
}