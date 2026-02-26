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
        /** @brief Rosh ha-Shanah (Tishri 1) in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t rosh_ha_shanah(int32_t year) const;
        /** @brief Sukkot (Tishri 15) in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t sukkot(int32_t year) const;
        /** @brief Hoshana Rabba (Tishri 21) in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t hoshana_rabba(int32_t year) const;
        /** @brief Shemini Atzeret (Tishri 22) in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t shemini_atzeret(int32_t year) const;
        /** @brief Simhat Torah (Tishri 23, diaspora convention) in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t simhat_torah(int32_t year) const;
        /** @brief Passover (15 Nisan) in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t passover(int32_t year) const;
        /** @brief End of Passover (Nisan 21) in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t end_of_passover(int32_t year) const;
        /** @brief Shavuot (Sivan 6) in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t shavuot(int32_t year) const;
        /** @brief Purim in Hebrew year @p year. */
        [[nodiscard]] fixed_date_t purim(int32_t year) const;
        /** @brief Shushan Purim in Gregorian year @p year (postponed from Saturday to Sunday). */
        [[nodiscard]] fixed_date_t shushan_purim(int32_t year) const;
        /** @brief Ta'anit Esther in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t ta_anit_esther(int32_t year) const;
        /** @brief Tzom Gedaliah in Gregorian year @p year (postponed from Saturday to Sunday). */
        [[nodiscard]] fixed_date_t tzom_gedaliah(int32_t year) const;
        /** @brief Tzom Tammuz in Gregorian year @p year (postponed from Saturday to Sunday). */
        [[nodiscard]] fixed_date_t tzom_tammuz(int32_t year) const;
        /** @brief Tisha B'Av in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t tishah_be_av(int32_t year) const;
        /** @brief Yom HaShoah in Gregorian year @p year (postponed if Nisan 27 falls on Sunday). */
        [[nodiscard]] fixed_date_t yom_ha_shoah(int32_t year) const;
        /** @brief Yom HaZikaron in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t yom_ha_zikkaron(int32_t year) const;

        /** @brief Sh'eilah-related dates associated with Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> sh_ela(int32_t year) const;
        /** @brief Birhath HaHama occurrences in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> birhath_ha_hama(int32_t year) const;

        /** @brief Tzom Tevet occurrences in Gregorian year @p year (may be 0, 1, or 2). */
        [[nodiscard]] std::vector<fixed_date_t> tzom_tevet(int32_t year) const;
        /** @brief Rosh Hodesh dates in Gregorian year @p year (including 2-day occurrences). */
        [[nodiscard]] std::vector<fixed_date_t> rosh_hodesh(int32_t year) const;

        /** @brief Samuel-season moments in Gregorian year @p year. */
        [[nodiscard]] std::vector<moment_t> samuel_season_in_gregorian(const angle_t& season, int32_t year) const;
        /** @brief Adda-season moments in Gregorian year @p year. */
        [[nodiscard]] std::vector<moment_t> add_season_in_gregorian(const angle_t& season, int32_t year) const;

        /** @brief Hanukkah dates in Hebrew year / Gregorian context @p year. */
        [[nodiscard]] std::vector<fixed_date_t> hanukkah(int32_t year) const;
        /** @brief All 8 Hanukkah days that fall in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> hanukkah_days(int32_t year) const;
        /** @brief Intermediate days of Sukkot (Tishri 16-21) in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> intermediate_days_of_sukkot(int32_t year) const;
        /** @brief Intermediate days of Passover (Nisan 16-20) in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> intermediate_days_of_passover(int32_t year) const;

        /** @brief Second day of Rosh ha-Shanah (Tishri 2) in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t rosh_ha_shanah_day_2(int32_t year) const;
        /** @brief Additional diaspora observance on Tishri 16 in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t sukkot_day_2_diaspora(int32_t year) const;
        /** @brief Additional diaspora observance on Nisan 16 in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t passover_day_2_diaspora(int32_t year) const;
        /** @brief Additional diaspora observance on Nisan 22 in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t passover_day_8_diaspora(int32_t year) const;
        /** @brief Additional diaspora observance on Sivan 7 in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t shavuot_day_2_diaspora(int32_t year) const;

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
