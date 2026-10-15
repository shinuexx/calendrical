#include "holiday_providers/julian_calendar_holiday_provider.hpp"

#include <algorithm>
#include <array>

namespace calendrical::holiday_providers{

    static constexpr calendars::julian_calendar julian{};

    julian_calendar_holiday_provider::julian_calendar_holiday_provider() :
        m_julian{std::addressof(julian)},
        m_gregorian{std::addressof(calendars::gregorian_calendar::standard)} {
        m_season_start = m_julian->to_fixed({-1, month::march, 23});
    }

    std::vector<holiday_t> julian_calendar_holiday_provider::in_year(int32_t gyear) const {
        std::vector<holiday_t> result{};
        for (auto& x : orthodox_christmas(gyear))
            result.emplace_back("orthodox christmas", x);
        result.emplace_back("orthodox easter", orthodox_easter(gyear));
        result.emplace_back("orthodox pentecost", orthodox_pentecost(gyear));
        std::ranges::sort(result, holiday_t::date_only_less_t{});
        return result;
    }
    std::vector<fixed_date_t> julian_calendar_holiday_provider::in_gregorian(month cmonth, int32_t cday, int32_t gyear) const {
        const auto range = m_gregorian->get_year_range(gyear);
        const auto y = m_julian->get_year(range.min);
        const auto y1 = y == -1 ? 1 : y + 1;
        auto r = std::array<int32_t, 2>({y, y1}) |
            std::views::transform([&](int32_t y2) { return m_julian->to_fixed({y2, cmonth, cday}); }) |
            std::views::filter(range);
        return {r.begin(), r.end()};
    }

    std::vector<fixed_date_t> julian_calendar_holiday_provider::orthodox_christmas(int32_t year) const {
        return in_gregorian(month::december, 25, year);
    }

    fixed_date_t julian_calendar_holiday_provider::orthodox_easter(int32_t year) const {
        const auto shifted_epact = mod(14 + 11 * mod(year, 19), 30);
        const auto jyear = year > 0 ? year : year - 1;
        const auto paschal_moon = m_julian->to_fixed({jyear, month::april, 19}) - shifted_epact;
        return weekday::sunday.after(paschal_moon);
    }
    fixed_date_t julian_calendar_holiday_provider::orthodox_pentecost(int32_t year) const {
        return orthodox_easter(year).add(49);
    }

    std::vector<moment_t> julian_calendar_holiday_provider::season_in_gregorian(const angle_t& season, int32_t year) const {
        constexpr double Y = 365 + moment_t::hms(6).days();
        const double offset = season / angle_t::full_circle * Y;
        auto r = m_gregorian->cycle_in_gregorian(season, year, Y, moment_t{ m_season_start } + offset);
        return {r.begin(), r.end()};
    }
}