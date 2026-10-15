#include "holiday_providers/pawukon_calendar_holiday_provider.hpp"

#include <algorithm>

namespace calendrical::holiday_providers {
    std::vector<holiday_t> pawukon_calendar_holiday_provider::in_year(int32_t gyear) const {
        std::vector<holiday_t> result;
        for (const auto& x : kajeng_keliwon(gyear))
            result.emplace_back("kajeng keliwon", x);
        for (const auto& x : tumpek(gyear))
            result.emplace_back("tumpek", x);
        std::ranges::sort(result);
        return result;
    }

    std::vector<fixed_date_t> pawukon_calendar_holiday_provider::kajeng_keliwon(int32_t gyear) const {
        return m_pawukon->positions_in_range(kajeng_keliwon_day, kajeng_keliwon_period, m_gregorian->get_year_range(gyear));
    }

    std::vector<fixed_date_t> pawukon_calendar_holiday_provider::tumpek(int32_t gyear) const {
        return m_pawukon->positions_in_range(tumpek_day, tumpek_period, m_gregorian->get_year_range(gyear));
    }
}
