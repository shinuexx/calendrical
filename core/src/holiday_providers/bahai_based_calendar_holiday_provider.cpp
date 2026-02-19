#include "holiday_providers/bahai_based_calendar_holiday_provider.hpp"

#include <algorithm>

#include "time_and_astronomy.hpp"

namespace calendrical::holiday_providers {
    const calendars::bahai_based_calendar* bahai_based_calendar_holiday_provider::calendar(bool astro) const {
        return astro ? static_cast<const calendars::bahai_based_calendar*>(m_astro) : static_cast<const calendars::bahai_based_calendar*>(m_classic);
    }

    std::vector<holiday_t> bahai_based_calendar_holiday_provider::in_year(const int32_t gyear) const {
        std::vector<holiday_t> result{
            {"new year", new_year(gyear)},
            {"naw ruz", naw_ruz(gyear)},
            {"feast of ridvan", feast_of_ridvan(gyear)},
            {"birth of the bab", birth_of_the_bab(gyear)}
        };
        std::ranges::sort(result);
        return result;
    }

    fixed_date_t bahai_based_calendar_holiday_provider::new_year(int32_t gyear) const {
        return m_gregorian->to_fixed({gyear, month::march, 21});
    }

    fixed_date_t bahai_based_calendar_holiday_provider::naw_ruz(int32_t gyear) const {
        return m_astro->new_year_on_or_before(m_gregorian->get_new_year(gyear + 1));
    }

    fixed_date_t bahai_based_calendar_holiday_provider::feast_of_ridvan(int32_t gyear) const {
        return naw_ruz(gyear).add(31);
    }

    fixed_date_t bahai_based_calendar_holiday_provider::birth_of_the_bab(int32_t gyear) const {
        using namespace time_and_astronomy;
        const auto ny = naw_ruz(gyear);
        const auto set1 = m_astro->bahai_sunset(ny);
        const auto m1 = new_moon_on_or_after(set1);
        const auto m8 = new_moon_on_or_after(m1 + 190);
        const auto day = m8.to_fixed_date();
        const auto set8 = m_astro->bahai_sunset(day);
        return m8 < set8 ? day.add(1) : day.add(2);
    }


}