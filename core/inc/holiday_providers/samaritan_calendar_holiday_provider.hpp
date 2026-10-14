#pragma once

#include "calendars/gregorian_calendar.hpp"
#include "calendars/samaritan_calendar.hpp"

namespace calendrical::holiday_providers {
    /**
     * @brief Holiday provider for fixed-date Samaritan holidays and festivals.
     *
     * This batch covers fixed month/day observances and fixed day ranges.
     * Moveable Pentecost and preparatory Simmut observances are handled separately.
     */
    class samaritan_calendar_holiday_provider final : public holiday_provider_if {
        /** @brief Calendar the holidays are defined in (not owned). */
        const calendars::samaritan_calendar* m_samaritan;
        /** @brief Gregorian calendar used to map holidays into Gregorian years. */
        const calendars::gregorian_calendar* m_gregorian;

        /** @brief Helper for inclusive fixed-date ranges in a Samaritan month. */
        [[nodiscard]] std::vector<fixed_date_t> fixed_range_in_gregorian(int32_t smonth, int32_t start_day, int32_t end_day, int32_t gyear) const;

    public:
        /** @brief Constructs a provider bound to a Samaritan calendar implementation. */
        explicit samaritan_calendar_holiday_provider(const calendars::samaritan_calendar* samaritan)
            : m_samaritan(samaritan),
              m_gregorian(std::addressof(calendars::gregorian_calendar::standard)) {}

        ~samaritan_calendar_holiday_provider() override = default;

        /** @brief Returns the backing Samaritan calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override { return m_samaritan; }
        /** @brief Returns Samaritan holidays/festivals that fall in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;
        /** @brief Maps a Samaritan month/day occurrence into Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(int32_t smonth, int32_t sday, int32_t gyear) const;

        /** @brief Passover (month 1, day 14) occurrences in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> passover(int32_t gyear) const;
        /** @brief Festival of the Unleavened Bread (month 1, days 15-21) in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> festival_of_unleavened_bread(int32_t gyear) const;
        /** @brief Festival of the Seventh Month (month 7, day 1) in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> festival_of_the_seventh_month(int32_t gyear) const;
        /** @brief Day of Atonement (month 7, day 10) in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> day_of_atonement(int32_t gyear) const;
        /** @brief Festival of Tabernacles (month 7, days 15-21) in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> festival_of_tabernacles(int32_t gyear) const;
        /** @brief The Eighth Day (month 7, day 22) in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> eighth_day(int32_t gyear) const;
        /** @brief Festival of Pentecost (eighth Sunday after Passover) in Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t festival_of_pentecost(int32_t gyear) const;
        /** @brief Simmut of Passover (Sabbath seven weeks before Passover) in Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t simmut_of_passover(int32_t gyear) const;
        /** @brief Simmut of Tabernacles (Sabbath seven weeks before Tabernacles) in Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t simmut_of_tabernacles(int32_t gyear) const;
    };
}
