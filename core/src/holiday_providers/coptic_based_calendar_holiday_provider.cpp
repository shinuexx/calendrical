#include "holiday_providers/coptic_based_calendar_holiday_provider.hpp"

#include <algorithm>

namespace calendrical::holiday_providers {

    std::vector<holiday_t> coptic_based_calendar_holiday_provider::in_year(int32_t year) const {
        std::vector<holiday_t> result{};
        for (const auto &x : christmas(year))
            result.emplace_back("christmas", x);
        for (const auto &x : building_of_the_cross(year))
            result.emplace_back("building of the cross", x);
        for (const auto &x : jesus_circumcision(year))
            result.emplace_back("jesus circumcision", x);
        for (const auto &x : epiphany(year))
            result.emplace_back("epiphany", x);
        for (const auto &x : mary_announcement(year))
            result.emplace_back("mary announcement", x);
        for (const auto &x : jesus_transfiguration(year))
            result.emplace_back("jesus transfiguration", x);
        std::ranges::sort(result, holiday_t::date_only_less_t{});
        return result;
    }

    std::vector<fixed_date_t> coptic_based_calendar_holiday_provider::in_gregorian(int32_t cmonth, int32_t cday, int32_t gyear) const {
        const auto range = m_gregorian->get_year_range(gyear);
        const auto y = m_calendar->get_year(range.min);
        auto r = std::views::iota(y, y + 3) |
            std::views::transform([&](int32_t y2) { return m_calendar->to_fixed({y2, cmonth, cday});}) |
            std::views::filter(range);
        return {r.begin(), r.end()};
    }

    std::vector<fixed_date_t> coptic_based_calendar_holiday_provider::christmas(int32_t year) const {
        return in_gregorian(4, 29, year);
    }

    std::vector<fixed_date_t> coptic_based_calendar_holiday_provider::building_of_the_cross(int32_t year) const {
        return in_gregorian(1, 17, year); // Thoout 17
    }

    std::vector<fixed_date_t> coptic_based_calendar_holiday_provider::jesus_circumcision(int32_t year) const {
        return in_gregorian(5, 6, year); // Tobe 6
    }

    std::vector<fixed_date_t> coptic_based_calendar_holiday_provider::epiphany(int32_t year) const {
        return in_gregorian(5, 11, year); // Tobe 11
    }

    std::vector<fixed_date_t> coptic_based_calendar_holiday_provider::mary_announcement(int32_t year) const {
        return in_gregorian(7, 29, year); // Paremotep 29
    }

    std::vector<fixed_date_t> coptic_based_calendar_holiday_provider::jesus_transfiguration(int32_t year) const {
        return in_gregorian(12, 13, year); // Mesore 13
    }
}
