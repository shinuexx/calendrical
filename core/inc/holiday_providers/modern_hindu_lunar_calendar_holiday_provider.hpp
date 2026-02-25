#pragma once

#include "calendars/modern_hindu_lunar_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {

    /**
     * @brief Holiday provider for modern Hindu lunar calendar observances.
     */
    class modern_hindu_lunar_calendar_holiday_provider :
        public holiday_provider_if{
        const calendars::modern_hindu_lunar_calendar* m_lunar;

    public:
        /** @brief Constructs a provider bound to a modern Hindu lunar calendar instance. */
        explicit modern_hindu_lunar_calendar_holiday_provider(const calendars::modern_hindu_lunar_calendar* lunar) : m_lunar{ lunar } {}

        /** @brief Returns the backing calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override;
        /** @brief Returns the provider's holiday set for Gregorian year @p gyear. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;

        /** @brief Maps Hindu lunar month/day occurrences into Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(int32_t cmonth, int32_t day, int32_t gyear) const;

        /** @brief Occurrence date of a Hindu lunar month/day in Hindu year @p lyear. */
        [[nodiscard]] fixed_date_t date_occur(int32_t lyear, int32_t lmonth, int32_t lday) const;
        /** @brief Hindu lunar holiday dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> lunar_holiday(int32_t lmonth, int32_t lday, int32_t gyear) const;

        /** @brief Diwali dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> diwali(int32_t gyear) const;

        /** @brief Occurrence date of a tithi-based event in Hindu year @p lyear. */
        [[nodiscard]] fixed_date_t tithi_occur(int32_t lmonth, const double& tithi, const moment_t& t, int32_t lyear) const;
        /** @brief Tithi-based event dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> lunar_event(int32_t lmonth, const double& tithi, const moment_t& t, int32_t gyear) const;

        /** @brief Maha Shivaratri-related dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> shiva(int32_t gyear) const;
        /** @brief Rama Navami-related dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> rama(int32_t gyear) const;

        /** @brief Karana index helper from sequence number @p n. */
        [[nodiscard]] static int32_t karana(int32_t n);

        /** @brief Sacred Wednesday dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> sacred_wednesdays(int32_t gyear) const;
        /** @brief Sacred Wednesday dates within an explicit fixed-date range. */
        [[nodiscard]] std::vector<fixed_date_t> sacred_wednesdays_in_range(const range<fixed_date_t>& range) const;
    };
}
