#pragma once

#include "types.hpp"
#include "calendars/bahai_calendar.hpp"
#include "calendars/astro_bahai_calendar.hpp"

namespace calendrical::holiday_providers {
    class bahai_based_calendar_holiday_provider:
        public holiday_provider_if
    {
        const calendars::astro_bahai_calendar* m_astro;
        const calendars::bahai_calendar* m_classic;
        const calendars::gregorian_calendar* m_gregorian;

    public:
        bahai_based_calendar_holiday_provider(const calendars::astro_bahai_calendar* astro, const calendars::bahai_calendar* classic) {
            m_astro = astro;
            m_classic = classic;
            m_gregorian = std::addressof(calendars::gregorian_calendar::standard);
        }

        ~bahai_based_calendar_holiday_provider() override = default;

        [[nodiscard]] const calendar_if* calendar() const override { return m_classic; }
        [[nodiscard]] const calendars::bahai_based_calendar* calendar(bool astro) const;

        [[nodiscard]] fixed_date_t new_year(int32_t gyear) const;
        [[nodiscard]] fixed_date_t naw_ruz(int32_t gyear) const;
        [[nodiscard]] fixed_date_t feast_of_ridvan(int32_t gyear) const;
        [[nodiscard]] fixed_date_t birth_of_the_bab(int32_t gyear) const;

        [[nodiscard]] std::vector<holiday_t> in_year(int32_t year) const override;
    };
}