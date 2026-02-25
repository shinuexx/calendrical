#pragma once

#include "calendars/hebrew_based_calendar.hpp"
#include "coptic_based_calendar_holiday_provider.hpp"

namespace calendrical::holiday_providers {

    /**
     * @brief Holiday provider for Hebrew-calendar and Jewish observance dates.
     */
    class hebrew_based_calendar_holiday_provider:
    public holiday_provider_if{

        const calendars::hebrew_based_calendar* m_hebrew;
        const calendars::gregorian_calendar* m_gregorian;
        const calendars::coptic_based_calendar* m_coptic;
        std::unique_ptr<const coptic_based_calendar_holiday_provider> m_coptic_provider;

        int32_t m_gregorian_year_hebrew_epoch;

        fixed_date_t m_start_1_adar_21;
        fixed_date_t m_start_1_adar_28;

        static constexpr double ADDA_Y{35975351 / 98496.0};

    public:
        // ReSharper disable once CppNonExplicitConvertingConstructor
        /** @brief Constructs a provider bound to a specific Hebrew calendar implementation. */
        hebrew_based_calendar_holiday_provider(const calendars::hebrew_based_calendar *hebrew); // NOLINT(*-explicit-constructor)
        ~hebrew_based_calendar_holiday_provider() override = default;

        /** @brief Returns the backing Hebrew calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override { return m_hebrew; }

        /** @brief Returns the provider's holiday set for Gregorian year @p year. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t year) const override;
        /** @brief Maps Hebrew month/day occurrences into Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(hebrew_month hmonth, int32_t hday, int32_t gyear) const;

        /** @brief Yom Kippur in Hebrew year @p year. */
        [[nodiscard]] fixed_date_t yom_kippur(int32_t year) const;
        /** @brief Passover (15 Nisan) in Hebrew year @p year. */
        [[nodiscard]] fixed_date_t passover(int32_t year) const;
        /** @brief Purim in Hebrew year @p year. */
        [[nodiscard]] fixed_date_t purim(int32_t year) const;
        /** @brief Ta'anit Esther in Hebrew year @p year. */
        [[nodiscard]] fixed_date_t ta_anit_esther(int32_t year) const;
        /** @brief Tisha B'Av in Hebrew year @p year. */
        [[nodiscard]] fixed_date_t tishah_be_av(int32_t year) const;
        /** @brief Yom HaZikaron in Hebrew year @p year. */
        [[nodiscard]] fixed_date_t yom_ha_zikkaron(int32_t year) const;

        /** @brief Sh'eilah-related dates associated with Hebrew/Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> sh_ela(int32_t year) const;
        /** @brief Birhath HaHama occurrences in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> birhath_ha_hama(int32_t year) const;

        /** @brief Samuel-season moments in Gregorian year @p year. */
        [[nodiscard]] std::vector<moment_t> samuel_season_in_gregorian(const angle_t& season, int32_t year) const;
        /** @brief Adda-season moments in Gregorian year @p year. */
        [[nodiscard]] std::vector<moment_t> add_season_in_gregorian(const angle_t& season, int32_t year) const;

        /** @brief Hanukkah dates in Hebrew year / Gregorian context @p year. */
        [[nodiscard]] std::vector<fixed_date_t> hanukkah(int32_t year) const;

        /** @brief Birthday anniversary in Hebrew year @p hyear for a Hebrew birth date. */
        [[nodiscard]] fixed_date_t birthday(const datetypes::hebrew_date& birthdate, int32_t hyear) const;
        /** @brief Yahrzeit anniversary in Hebrew year @p hyear for a Hebrew death date. */
        [[nodiscard]] fixed_date_t yahrzeit(const datetypes::hebrew_date& deathdate, int32_t hyear) const;

        /** @brief Birthday anniversaries that fall in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> birthday_in_gregorian(const datetypes::hebrew_date& birthdate, int32_t gyear) const;
        /** @brief Yahrzeit anniversaries that fall in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> yahrzeit_in_gregorian(const datetypes::hebrew_date& deathdate, int32_t gyear) const;
    };
}
