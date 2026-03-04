#include "holiday_providers/gregorian_calendar_holiday_provider.hpp"

#include <algorithm>
#include <lunar_phase.hpp>
#include <season.hpp>
#include <time_and_astronomy.hpp>

namespace calendrical::holiday_providers {
    std::vector<holiday_t> gregorian_calendar_holiday_provider::in_year(int32_t gyear) const{
        std::vector<holiday_t> holidays{};
        holidays.emplace_back( "new year", m_gregorian->get_new_year(gyear) );
        holidays.emplace_back( "christmas", christmas(gyear) );
        holidays.emplace_back( "advent", advent(gyear) );
        holidays.emplace_back( "epiphany", epiphany(gyear) );
        holidays.emplace_back( "easter", easter(gyear) );
        holidays.emplace_back( "pentecost", pentecost(gyear) );
        holidays.emplace_back( "astronomical easter", astronomical_easter(gyear) );
        for (const auto x : unlucky_fridays(gyear))
            holidays.emplace_back("unlucky friday", x );
        std::ranges::sort(holidays);
        return holidays;
    }

    fixed_date_t gregorian_calendar_holiday_provider::christmas(int32_t year) const{
        return m_gregorian->to_fixed({year, month::december, 25});
    }
    fixed_date_t gregorian_calendar_holiday_provider::advent(int32_t year) const{
        return weekday::sunday.nearest(m_gregorian->to_fixed({year, month::november, 30}));
    }
    fixed_date_t gregorian_calendar_holiday_provider::epiphany(int32_t year) const {
        return m_gregorian->first_weekday(weekday::sunday, {year, month::january, 2});
    }
    fixed_date_t gregorian_calendar_holiday_provider::easter(int32_t year) const {
        const auto century = floor_div(year, 100) + 1;
        const auto shifted_epact = mod(14 + 11 * mod(year, 19) - floor_div(3 * century, 4) +
                                               floor_div(5 + 8 * century, 25),
                                       30);
        const auto adjusted_epact =
                shifted_epact == 0 || (shifted_epact == 1 && 10 < mod(year, 19))
                        ? shifted_epact + 1
                        : shifted_epact;
        const auto paschal_moon = m_gregorian->to_fixed({ year, month::april, 19 }) - adjusted_epact;
        return weekday::sunday.after(paschal_moon);
    }
    fixed_date_t gregorian_calendar_holiday_provider::pentecost(int32_t year) const {
        return easter(year).add(49);
    }

    std::vector<fixed_date_t> gregorian_calendar_holiday_provider::unlucky_fridays_in_range(const interval<fixed_date_t>& range) const {
        std::vector<fixed_date_t> fridays{};
        for (auto fri = weekday::friday.on_or_after(range.min); fri < range.max; fri = weekday::friday.on_or_after(fri.add(1))) {
            if (m_gregorian->to_date(fri).day == 13)
                fridays.push_back(fri);
        }
        std::ranges::sort(fridays);
        return fridays;
    }
    std::vector<fixed_date_t> gregorian_calendar_holiday_provider::unlucky_fridays(int32_t year) const {
        return unlucky_fridays_in_range(m_gregorian->get_year_range(year));
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    fixed_date_t gregorian_calendar_holiday_provider::astronomical_easter(int32_t year) const { // NOLINT(*-convert-member-functions-to-static)
        using namespace time_and_astronomy;
        const auto equinox = season_in_gregorian(season::spring, year);
        const auto full_moon = lunar_phase_at_or_after(lunar_phase::full_moon, equinox);
        const auto paschal_moon = apparent_from_universal(full_moon, locations::jerusalem).to_fixed_date();
        return weekday::sunday.after(paschal_moon);
    }
}