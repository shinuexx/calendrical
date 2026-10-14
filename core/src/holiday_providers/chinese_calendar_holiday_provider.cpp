#include "holiday_providers/chinese_calendar_holiday_provider.hpp"
#include "helpers.hpp"

#include <algorithm>

namespace calendrical::holiday_providers {
    std::vector<holiday_t> chinese_calendar_holiday_provider::in_year(int32_t gyear) const {
        std::vector<holiday_t> result{
            {"chinese new year", new_year(gyear)},
            {"qingming", qing_ming(gyear)},
            {"dragon festival", dragon_festival(gyear)}
        };
        std::ranges::sort(result);
        return result;
    }

    fixed_date_t chinese_calendar_holiday_provider::new_year(int32_t gyear) const {
        return m_chinese->new_year_on_or_before(m_gregorian->to_fixed({gyear, month::july, 1}));
    }

    fixed_date_t chinese_calendar_holiday_provider::qing_ming(int32_t gyear) const {
        return m_chinese->minor_solar_term_on_of_after(m_gregorian->to_fixed({gyear, month::march, 30})).to_fixed_date();
    }

    fixed_date_t chinese_calendar_holiday_provider::dragon_festival(int32_t gyear) const {
        const auto elapsed_years = 1 + gyear - m_gregorian->get_year(m_chinese->epoch());
        const auto cycle = static_cast<int32_t>(1 + floor_div(elapsed_years - 1, 60));
        const auto year = static_cast<int32_t>(adjusted_remainder(elapsed_years, 60));
        return m_chinese->to_fixed({cycle, year, 5, false, 5});
    }
}
