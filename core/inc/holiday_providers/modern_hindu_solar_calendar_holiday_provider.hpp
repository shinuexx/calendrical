#pragma once

#include "calendars/gregorian_calendar.hpp"
#include "calendars/modern_hindu_solar_calendar.hpp"

namespace calendrical::holiday_providers {
    /**
     * @brief Holiday provider for selected modern Hindu solar observances.
     */
    class modern_hindu_solar_calendar_holiday_provider final : public holiday_provider_if {
        /** @brief Calendar the holidays are defined in (not owned). */
        const calendars::modern_hindu_solar_calendar* m_solar;
        /** @brief Gregorian calendar used to map holidays into Gregorian years. */
        const calendars::gregorian_calendar* m_gregorian;

    public:
        /** @brief Constructs a provider bound to a modern Hindu solar calendar instance. */
        explicit modern_hindu_solar_calendar_holiday_provider(const calendars::modern_hindu_solar_calendar* solar)
            : m_solar(solar),
              m_gregorian(std::addressof(calendars::gregorian_calendar::standard)) {}

        ~modern_hindu_solar_calendar_holiday_provider() override = default;

        /** @brief Returns the backing calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override { return m_solar; }
        /** @brief Returns the provider's holiday set for Gregorian year @p gyear. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;

        /** @brief Maps a Hindu solar month/day occurrence into Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(int32_t smonth, int32_t sday, int32_t gyear) const;

        /** @brief Solar New Year (Sowramana Ugadi): Mesha 1 in Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t sowramana_ugadi(int32_t gyear) const;

        /**
         * @brief Ayyappa Jyothi Darshanam (Pongal) observance days in Gregorian year @p gyear.
         *
         * The observance includes Makara 1 and the preceding day.
         */
        [[nodiscard]] std::vector<fixed_date_t> ayyappa_jyothi_darshanam(int32_t gyear) const;
    };
}
