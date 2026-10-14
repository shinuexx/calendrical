#include "holiday_providers/modern_hindu_solar_calendar_holiday_provider.hpp"

#include <algorithm>

namespace calendrical::holiday_providers {
    std::vector<holiday_t> modern_hindu_solar_calendar_holiday_provider::in_year(int32_t gyear) const {
        std::vector<holiday_t> result;
        result.emplace_back("sowramana ugadi", sowramana_ugadi(gyear));
        for (const auto& x : ayyappa_jyothi_darshanam(gyear))
            result.emplace_back("ayyappa jyothi darshanam", x);
        std::ranges::sort(result, holiday_t::date_only_less_t{});
        return result;
    }

    std::vector<fixed_date_t> modern_hindu_solar_calendar_holiday_provider::in_gregorian(
        int32_t smonth,
        int32_t sday,
        int32_t gyear) const {
        const auto range = m_gregorian->get_year_range(gyear);
        const auto y = m_solar->to_date(range.min).year;
        auto r = std::ranges::iota_view(y - 1, y + 2) |
            std::views::transform([&](int32_t y2) { return m_solar->to_fixed({y2, smonth, sday}); }) |
            std::views::filter(range);
        return {r.begin(), r.end()};
    }

    fixed_date_t modern_hindu_solar_calendar_holiday_provider::sowramana_ugadi(int32_t gyear) const {
        // For the foreseeable future this is the modern Hindu solar date Mesha 1 in S.E. (gyear - 78).
        return m_solar->to_fixed({gyear - 78, 1, 1});
    }

    std::vector<fixed_date_t> modern_hindu_solar_calendar_holiday_provider::ayyappa_jyothi_darshanam(int32_t gyear) const {
        // Makara is the 10th sidereal solar month.
        const auto pongal = m_solar->to_fixed({gyear - 79, 10, 1});
        return {pongal.subtract(1), pongal};
    }
}
