#include "holiday_providers/islamic_based_calendar_holiday_provider.hpp"

namespace calendrical::holiday_providers {

    std::vector<holiday_t> islamic_based_calendar_holiday_provider::in_year(int32_t year) const {
        std::vector<holiday_t> result;
        for (auto& x: mawlid(year))
            result.emplace_back("Mawlid", x);
        std::ranges::sort(result);
        return result;
    }

    std::vector<fixed_date_t> islamic_based_calendar_holiday_provider::in_gregorian(int32_t imonth, int32_t iday,
                                                                                    int32_t gyear) const {
        const auto range = m_gregorian->get_year_range(gyear);
        const auto y = m_islamic->to_date(range.min).year;
        auto r = std::ranges::iota_view(y - 1, y + 2) |
            std::views::transform([&](int32_t y2) { return m_islamic->to_fixed({y2, imonth, iday}); }) |
            std::views::filter(range);
        return {r.begin(), r.end()};
    }

    std::vector<fixed_date_t> islamic_based_calendar_holiday_provider::mawlid(int32_t year) const {
        return in_gregorian(3, 12, year);
    }


}