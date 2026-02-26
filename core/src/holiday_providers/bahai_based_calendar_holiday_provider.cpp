#include "holiday_providers/bahai_based_calendar_holiday_provider.hpp"

#include <algorithm>

#include "time_and_astronomy.hpp"

namespace calendrical::holiday_providers {
    const calendars::bahai_based_calendar* bahai_based_calendar_holiday_provider::calendar(bool astro) const {
        return astro ? static_cast<const calendars::bahai_based_calendar*>(m_astro) : static_cast<const calendars::bahai_based_calendar*>(m_classic);
    }

    fixed_date_t bahai_based_calendar_holiday_provider::bahai_date_in_gregorian_year(
        int32_t gyear,
        int32_t month,
        int32_t day) const {
        const auto nr = naw_ruz(gyear);
        const auto y = calendar(true)->to_date(nr);
        return calendar(true)->to_fixed({y.major, y.cycle, y.year, month, day});
    }

    std::vector<holiday_t> bahai_based_calendar_holiday_provider::in_year(const int32_t gyear) const {
        std::vector<holiday_t> result{
            {"new year", new_year(gyear)},
            {"naw ruz", naw_ruz(gyear)},
            {"feast of ridvan", feast_of_ridvan(gyear)},
            {"ridvan 9", ridvan_9(gyear)},
            {"ridvan 12", ridvan_12(gyear)},
            {"declaration of the bab", declaration_of_the_bab(gyear)},
            {"ascension of bahaullah", ascension_of_bahaullah(gyear)},
            {"martyrdom of the bab", martyrdom_of_the_bab(gyear)},
            {"birth of the bab", birth_of_the_bab(gyear)},
            {"birth of bahaullah", birth_of_bahaullah(gyear)},
            {"day of the covenant", day_of_the_covenant(gyear)},
            {"ascension of abdul baha", ascension_of_abdul_baha(gyear)}
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

    fixed_date_t bahai_based_calendar_holiday_provider::ridvan_9(int32_t gyear) const {
        return bahai_date_in_gregorian_year(gyear, 3, 2); // Jamal 2
    }

    fixed_date_t bahai_based_calendar_holiday_provider::ridvan_12(int32_t gyear) const {
        return bahai_date_in_gregorian_year(gyear, 3, 5); // Jamal 5
    }

    fixed_date_t bahai_based_calendar_holiday_provider::declaration_of_the_bab(int32_t gyear) const {
        return bahai_date_in_gregorian_year(gyear, 4, 8); // Azamat 8
    }

    fixed_date_t bahai_based_calendar_holiday_provider::ascension_of_bahaullah(int32_t gyear) const {
        return bahai_date_in_gregorian_year(gyear, 4, 13); // Azamat 13
    }

    fixed_date_t bahai_based_calendar_holiday_provider::martyrdom_of_the_bab(int32_t gyear) const {
        return bahai_date_in_gregorian_year(gyear, 6, 17); // Rahmat 17
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

    fixed_date_t bahai_based_calendar_holiday_provider::birth_of_bahaullah(int32_t gyear) const {
        return birth_of_the_bab(gyear).add(1);
    }

    fixed_date_t bahai_based_calendar_holiday_provider::day_of_the_covenant(int32_t gyear) const {
        return bahai_date_in_gregorian_year(gyear, 14, 4); // Qawl 4
    }

    fixed_date_t bahai_based_calendar_holiday_provider::ascension_of_abdul_baha(int32_t gyear) const {
        return bahai_date_in_gregorian_year(gyear, 14, 6); // Qawl 6
    }


}
