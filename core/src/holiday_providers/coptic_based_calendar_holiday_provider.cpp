#include "holiday_providers/coptic_based_calendar_holiday_provider.hpp"

namespace calendrical::holiday_providers {

    std::vector<holiday_t> coptic_based_calendar_holiday_provider::in_year(int32_t year) const {
        std::vector<holiday_t> result{};
        for (const auto &x : christmas(year))
            result.emplace_back("christmas", x);
        std::ranges::sort(result);
        return result;
    }

    std::vector<fixed_date_t> coptic_based_calendar_holiday_provider::in_gregorian(int32_t cmonth, int32_t cday, int32_t gyear) const {
        const auto range = m_gregorian->get_year_range(gyear);
        const auto y = m_calendar->get_year(range.min);
        auto r = std::views::iota(y, y + 3) |
            std::views::transform([&](int32_t y) { return m_calendar->to_fixed({y, cmonth, cday});}) |
            std::views::filter(range);
        return {r.begin(), r.end()};
    }

    std::vector<fixed_date_t> coptic_based_calendar_holiday_provider::christmas(int32_t year) const {
        return in_gregorian(4, 29, year);
    }
}