#include "holiday_providers/bahai_based_calendar_holiday_provider.hpp"
#include "time_and_astronomy.hpp"

namespace calendrical::holiday_providers {
    std::shared_ptr<const calendar_if> bahai_based_calendar_holiday_provider::get_calendar() const
    {
        return get_calendar(false);
    }
    std::shared_ptr<const bahai_based_calendar> bahai_based_calendar_holiday_provider::get_calendar(bool astro) const
    {
        if (astro)
            return m_astro;
        else
            return m_classic;
    }
    std::list<holiday_t> bahai_based_calendar_holiday_provider::get_holiday_in_year(int32_t gyear) const
    {
        std::list<holiday_t> holidays{
            {"new year", new_year(gyear)},
            {"naw ruz", naw_ruz(gyear)},
            {"feast of ridvan", feast_of_ridvan(gyear)},
            {"birth of the bab", birth_of_the_bab(gyear)}
        };
        holidays.sort();
        return holidays;
    }
    fixed_date_t bahai_based_calendar_holiday_provider::new_year(int32_t gyear) const
    {
        return m_gregorian->to_fixed({ gyear, month::march, 21 });
    }
    fixed_date_t bahai_based_calendar_holiday_provider::naw_ruz(int32_t gyear) const
    {
        return m_astro->new_year_on_or_before(m_gregorian->get_new_year(gyear + 1));
    }
    fixed_date_t bahai_based_calendar_holiday_provider::feast_of_ridvan(int32_t gyear) const
    {
        return naw_ruz(gyear) + 31;
    }
    fixed_date_t bahai_based_calendar_holiday_provider::birth_of_the_bab(int32_t gyear) const
    {
        using namespace time_and_astronomy;
        auto ny = naw_ruz(gyear);
        auto set1 = m_astro->bahai_sunset(ny);
        auto m1 = new_moon_on_or_after(set1);
        auto m8 = new_moon_on_or_after(m1 + 190);
        auto day = m8.to_fixed_date();
        auto set8 = m_astro->bahai_sunset(day);
        if (m8 < set8)
            return day.add(1);
        else
            return day.add(2);
    }
}