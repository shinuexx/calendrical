#include "holiday_providers/simple_calendar_holiday_provider.hpp"

#include <algorithm>

namespace calendrical::holiday_providers {
    std::vector<holiday_t> simple_calendar_holiday_provider::in_year(int32_t gyear) const {
        std::vector<holiday_t> result;
        for (const auto& x : navasard(gyear))
            result.emplace_back("navasard", x);
        std::ranges::sort(result, holiday_t::date_only_less_t{});
        return result;
    }

    std::vector<fixed_date_t> simple_calendar_holiday_provider::in_gregorian(int32_t cmonth, int32_t cday, int32_t gyear) const {
        const auto range = m_gregorian->get_year_range(gyear);
        const auto y = m_calendar->to_date(range.min).year;
        auto r = std::ranges::iota_view(y - 1, y + 2) |
            std::views::transform([&](int32_t y2) { return m_calendar->to_fixed({y2, cmonth, cday}); }) |
            std::views::filter(range);
        return {r.begin(), r.end()};
    }

    std::vector<fixed_date_t> simple_calendar_holiday_provider::navasard(int32_t gyear) const {
        return in_gregorian(1, 1, gyear);
    }
}
