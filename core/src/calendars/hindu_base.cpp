#include "calendars/hindu_base.hpp"

namespace calendrical::calendars{
    const rational hindu_base::arya_solar_year{ 1577917500ll,4320000 };
    const rational hindu_base::arya_jovian_period{ 1577917500ll, 364224 };

    const rational hindu_base::arya_solar_month{ arya_solar_year / 12 };
    const rational hindu_base::arya_lunar_month{ 1577917500ll, 53433336 };
    const rational hindu_base::arya_lunar_day{ arya_lunar_month / 30 };

    const rational hindu_base::lunar_leap_year_indicator{ 23902504679ll, 1282400064ll };

    const rational hindu_base::arya_solar_lunar_month_difference{ arya_solar_month - arya_lunar_month };

    int64_t hindu_base::day_count(const fixed_date_t& date) const {
        return date - ms_epoch;
    }

    int32_t hindu_base::jovian_year(const fixed_date_t& date) const {
        const auto a = static_cast<bigint>(floor_div(day_count(date), arya_jovian_period / 12) + 27);
        return static_cast<int32_t>(adjusted_remainder(a, 60));
    }
}