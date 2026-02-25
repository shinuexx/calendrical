#include "holiday_providers/islamic_based_calendar_holiday_provider.hpp"

#include <algorithm>

namespace calendrical::holiday_providers {

    std::vector<holiday_t> islamic_based_calendar_holiday_provider::in_year(int32_t year) const {
        std::vector<holiday_t> result;
        for (auto& x: islamic_new_year(year))
            result.emplace_back("islamic new year", x);
        for (auto& x: ashura(year))
            result.emplace_back("ashura", x);
        for (auto& x: mawlid(year))
            result.emplace_back("Mawlid", x);
        for (auto& x: lailat_al_miraj(year))
            result.emplace_back("lailat al miraj", x);
        for (auto& x: lailat_al_baraa(year))
            result.emplace_back("lailat al baraa", x);
        for (auto& x: ramadan(year))
            result.emplace_back("ramadan", x);
        for (auto& x: lailat_al_qadr(year))
            result.emplace_back("lailat al qadr", x);
        for (auto& x: eid_ul_fitr(year))
            result.emplace_back("eid ul fitr", x);
        for (auto& x: eid_ul_adha(year))
            result.emplace_back("eid ul adha", x);
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

    std::vector<fixed_date_t> islamic_based_calendar_holiday_provider::islamic_new_year(int32_t year) const {
        return in_gregorian(1, 1, year);
    }

    std::vector<fixed_date_t> islamic_based_calendar_holiday_provider::ashura(int32_t year) const {
        return in_gregorian(1, 10, year);
    }

    std::vector<fixed_date_t> islamic_based_calendar_holiday_provider::lailat_al_miraj(int32_t year) const {
        return in_gregorian(7, 27, year);
    }

    std::vector<fixed_date_t> islamic_based_calendar_holiday_provider::lailat_al_baraa(int32_t year) const {
        return in_gregorian(8, 15, year);
    }

    std::vector<fixed_date_t> islamic_based_calendar_holiday_provider::ramadan(int32_t year) const {
        return in_gregorian(9, 1, year);
    }

    std::vector<fixed_date_t> islamic_based_calendar_holiday_provider::lailat_al_qadr(int32_t year) const {
        return in_gregorian(9, 27, year);
    }

    std::vector<fixed_date_t> islamic_based_calendar_holiday_provider::eid_ul_fitr(int32_t year) const {
        return in_gregorian(10, 1, year);
    }

    std::vector<fixed_date_t> islamic_based_calendar_holiday_provider::eid_ul_adha(int32_t year) const {
        return in_gregorian(12, 10, year);
    }


} 
