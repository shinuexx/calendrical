#pragma once

#include "calendars/julian_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers{
    /**
     * @brief Holiday provider for Julian/Orthodox feast calculations.
     */
    class julian_calendar_holiday_provider :
    public holiday_provider_if
    {
        /** @brief Calendar the holidays are defined in (not owned). */
        const calendars::julian_calendar* m_julian;
        /** @brief Gregorian calendar used to map holidays into Gregorian years. */
        const calendars::gregorian_calendar* m_gregorian;

        /** @brief Julian 23 March, 1 BCE: anchor for the Julian seasonal cycle. */
        fixed_date_t m_season_start;

    public:
        /** @brief Constructs a provider with default Julian/Gregorian backends. */
        julian_calendar_holiday_provider();

        /** @brief Returns the backing Julian calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override { return m_julian; }

        /** @brief Returns the provider's holiday set for Gregorian year @p gyear. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;
        /** @brief Maps Julian month/day occurrences into Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(month cmonth, int32_t cday, int32_t gyear) const;

        /** @brief Orthodox Christmas dates associated with Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> orthodox_christmas(int32_t year) const;

        /** @brief Orthodox Easter date for Gregorian year @p year. */
        [[nodiscard]] fixed_date_t orthodox_easter(int32_t year) const;
        /** @brief Orthodox Pentecost date for Gregorian year @p year. */
        [[nodiscard]] fixed_date_t orthodox_pentecost(int32_t year) const;

        /** @brief Seasonal solar events in Gregorian year @p year under Julian-oriented rules. */
        [[nodiscard]] std::vector<moment_t> season_in_gregorian(const angle_t& season, int32_t year) const;

        ~julian_calendar_holiday_provider() override = default;
    };
}
