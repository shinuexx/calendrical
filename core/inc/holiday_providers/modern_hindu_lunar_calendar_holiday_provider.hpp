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

        /** @brief Main Diwali day (Kartika 1) dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> diwali(int32_t gyear) const;
        /** @brief Diwali festival dates (Ashvina 29 through Kartika 1) in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> diwali_days(int32_t gyear) const;
        /** @brief Janmashtami (Shravana 23) dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> janmashtami(int32_t gyear) const;
        /** @brief Ganesha Chaturthi (Bhadrapada 4) dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> ganesha_chaturthi(int32_t gyear) const;
        /** @brief Durga Ashtami (Ashvina 8) dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> durga_ashtami(int32_t gyear) const;
        /** @brief Sarasvati Puja (Ashvina 9) dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> sarasvati_puja(int32_t gyear) const;
        /** @brief Dasra (Ashvina 10) dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> dasra(int32_t gyear) const;
        /** @brief Karthikai Deepam (Kartika 15) dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> karthikai_deepam(int32_t gyear) const;
        /** @brief Vaikunta Ekadashi (Margashirsha 11) dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> vaikunta_ekadashi(int32_t gyear) const;
        /**
         * @brief Buddha's Birthday dates in Gregorian year @p gyear.
         *
         * This provider uses the simple Vaiśākha 15 convention; actual observance varies by tradition.
         */
        [[nodiscard]] std::vector<fixed_date_t> buddha_birthday(int32_t gyear) const;
        /**
         * @brief Varalakshmi Vratam dates in Gregorian year @p gyear.
         *
         * Computed as the Friday strictly before Shravana 15.
         */
        [[nodiscard]] std::vector<fixed_date_t> varalakshmi_vratam(int32_t gyear) const;
        /**
         * @brief Holi dates in Gregorian year @p gyear.
         *
         * Modeled as the date on which lunar day Phalguna 15 is current in the evening.
         */
        [[nodiscard]] std::vector<fixed_date_t> holi(int32_t gyear) const;

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
        [[nodiscard]] std::vector<fixed_date_t> sacred_wednesdays_in_range(const interval<fixed_date_t>& range) const;
    };
}
