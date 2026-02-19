#include "holiday_providers/simple_calendar_holiday_provider.hpp"

namespace calendrical::holiday_providers {
    std::vector<fixed_date_t> simple_calendar_holiday_provider::in_gregorian(int32_t cmonth, int32_t cday, int32_t gyear) const {
        const auto range = m_gregorian->get_year_range(gyear);
        const auto y = m_calendar->to_date(range.min).year;
        auto r = std::ranges::iota_view(y - 1, y + 2) |
            std::views::transform([&](int32_t y2) { return m_calendar->to_fixed({y2, cmonth, cday}); }) |
            std::views::filter(range);
        return {r.begin(), r.end()};
    }
}