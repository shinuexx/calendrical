#pragma once

#include "types.hpp"
#include "calendars/bahai_calendar.hpp"
#include "calendars/astro_bahai_calendar.hpp"

namespace calendrical::holiday_providers {
    /**
     * @brief Holiday provider for Baha'i holidays (classic and astronomical variants).
     */
    class bahai_based_calendar_holiday_provider:
        public holiday_provider_if
    {
        const calendars::astro_bahai_calendar* m_astro;
        const calendars::bahai_calendar* m_classic;
        const calendars::gregorian_calendar* m_gregorian;

        /** @brief Converts a fixed Bahai month/day in the astronomical Bahai year of @p gyear to a fixed date. */
        [[nodiscard]] fixed_date_t bahai_date_in_gregorian_year(int32_t gyear, int32_t month, int32_t day) const;

    public:
        /** @brief Constructs a provider with both astronomical and classic Baha'i calendars. */
        bahai_based_calendar_holiday_provider(const calendars::astro_bahai_calendar* astro, const calendars::bahai_calendar* classic) {
            m_astro = astro;
            m_classic = classic;
            m_gregorian = std::addressof(calendars::gregorian_calendar::standard);
        }

        ~bahai_based_calendar_holiday_provider() override = default;

        /** @brief Returns the default backing calendar interface (classic Baha'i). */
        [[nodiscard]] const calendar_if* calendar() const override { return m_classic; }
        /** @brief Returns either the astronomical or classic Baha'i calendar backend. */
        [[nodiscard]] const calendars::bahai_based_calendar* calendar(bool astro) const;

        /** @brief Baha'i New Year date associated with Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t new_year(int32_t gyear) const;
        /** @brief Naw-Ruz date associated with Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t naw_ruz(int32_t gyear) const;
        /** @brief First day of Ridvan associated with Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t feast_of_ridvan(int32_t gyear) const;
        /** @brief Ninth day of Ridvan associated with Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t ridvan_9(int32_t gyear) const;
        /** @brief Twelfth day of Ridvan associated with Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t ridvan_12(int32_t gyear) const;
        /** @brief Declaration of the Bab associated with Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t declaration_of_the_bab(int32_t gyear) const;
        /** @brief Ascension of Baha'u'llah associated with Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t ascension_of_bahaullah(int32_t gyear) const;
        /** @brief Martyrdom of the Bab associated with Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t martyrdom_of_the_bab(int32_t gyear) const;
        /** @brief Birth of the Bab holiday date associated with Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t birth_of_the_bab(int32_t gyear) const;
        /** @brief Birth of Baha'u'llah holiday date associated with Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t birth_of_bahaullah(int32_t gyear) const;
        /** @brief Day of the Covenant associated with Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t day_of_the_covenant(int32_t gyear) const;
        /** @brief Ascension of 'Abdu'l-Baha associated with Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t ascension_of_abdul_baha(int32_t gyear) const;

        /** @brief Returns the provider's Baha'i holiday set for Gregorian year @p year. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t year) const override;
    };
}
