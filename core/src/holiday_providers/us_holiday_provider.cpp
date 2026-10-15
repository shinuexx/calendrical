#include "holiday_providers/us_holiday_provider.hpp"

#include <algorithm>

namespace calendrical::holiday_providers {

    std::vector<holiday_t> us_holiday_provider::in_year(int32_t gyear) const {
        std::vector<holiday_t> holidays;
        holidays.reserve(6);
        holidays.emplace_back("daylight saving start", daylight_saving_start(gyear));
        holidays.emplace_back("memorial day", memorial_day(gyear));
        holidays.emplace_back("independence day", independence_day(gyear));
        holidays.emplace_back("labor day", labor_day(gyear));
        holidays.emplace_back("election day", election_day(gyear));
        holidays.emplace_back("daylight saving end", daylight_saving_end(gyear));
        std::ranges::sort(holidays);
        return holidays;
    }

    fixed_date_t us_holiday_provider::independence_day(int32_t year) const {
        return m_gregorian->to_fixed({year, month::july, 4});
    }

    fixed_date_t us_holiday_provider::labor_day(int32_t year) const {
        return m_gregorian->first_weekday(weekday::monday, {year, month::september, 1});
    }

    fixed_date_t us_holiday_provider::memorial_day(int32_t year) const {
        return m_gregorian->last_weekday(weekday::monday, {year, month::may, 31});
    }

    fixed_date_t us_holiday_provider::election_day(int32_t year) const {
        return m_gregorian->first_weekday(weekday::tuesday, {year, month::november, 2});
    }

    fixed_date_t us_holiday_provider::daylight_saving_start(int32_t year) const {
        return m_gregorian->nth_weekday(2, weekday::sunday, {year, month::march, 1});
    }

    fixed_date_t us_holiday_provider::daylight_saving_end(int32_t year) const {
        return m_gregorian->first_weekday(weekday::sunday, {year, month::november, 1});
    }

}
