#pragma once

#include "calendars/persian_based_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {
    /**
     * @brief Holiday provider for Persian-calendar holidays.
     */
    class persian_based_calendar_holiday_provider :
        public holiday_provider_if {

        const calendars::persian_based_calendar* m_persian;

        int32_t m_gregorian_year_persian_epoch;

    public:
        /** @brief Constructs a provider bound to a specific Persian calendar implementation. */
        explicit persian_based_calendar_holiday_provider(const calendars::persian_based_calendar* persian);
        ~persian_based_calendar_holiday_provider() override = default;

        /** @brief Returns the backing calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override;
        /** @brief Returns the provider's holiday set for Gregorian year @p gyear. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;

        /** @brief Maps a Persian month/day occurrence into Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(int32_t pmonth, int32_t pday, int32_t gyear) const;

        /** @brief Nawruz date associated with Gregorian year @p year. */
        fixed_date_t nawruz(int32_t year) const;
        /** @brief Sizdah Bedar (Farvardin 13) occurrences in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> sizdah_bedar(int32_t year) const;
        /** @brief Tirgan (Tir 13) occurrences in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> tirgan(int32_t year) const;
        /** @brief Mehregan (Mehr 16) occurrences in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> mehregan(int32_t year) const;
        /**
         * @brief Yalda Night (night of Azar 30) occurrences in Gregorian year @p year.
         *
         * Represented by the civil date Azar 30; observance begins on the prior evening.
         */
        [[nodiscard]] std::vector<fixed_date_t> yalda_night(int32_t year) const;
        /** @brief Sepandarmazgan (Esfand 5) occurrences in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> sepandarmazgan(int32_t year) const;
        /**
         * @brief Chaharshanbe Suri in Gregorian year @p year.
         *
         * Modeled as the last Wednesday before Nawruz of Gregorian year @p year.
         */
        [[nodiscard]] fixed_date_t chaharshanbe_suri(int32_t year) const;
    };
}
