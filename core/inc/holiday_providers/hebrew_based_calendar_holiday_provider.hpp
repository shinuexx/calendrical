#pragma once

#include "calendars/hebrew_based_calendar.hpp"
#include "coptic_based_calendar_holiday_provider.hpp"

namespace calendrical::holiday_providers {

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
        hebrew_based_calendar_holiday_provider(const calendars::hebrew_based_calendar *hebrew); // NOLINT(*-explicit-constructor)
        ~hebrew_based_calendar_holiday_provider() override = default;

        [[nodiscard]] const calendar_if* calendar() const override { return m_hebrew; }

        [[nodiscard]] std::vector<holiday_t> in_year(int32_t year) const override;
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(hebrew_month hmonth, int32_t hday, int32_t gyear) const;

        [[nodiscard]] fixed_date_t yom_kippur(int32_t year) const;
        [[nodiscard]] fixed_date_t passover(int32_t year) const;
        [[nodiscard]] fixed_date_t purim(int32_t year) const;
        [[nodiscard]] fixed_date_t ta_anit_esther(int32_t year) const;
        [[nodiscard]] fixed_date_t tishah_be_av(int32_t year) const;
        [[nodiscard]] fixed_date_t yom_ha_zikkaron(int32_t year) const;

        [[nodiscard]] std::vector<fixed_date_t> sh_ela(int32_t year) const;
        [[nodiscard]] std::vector<fixed_date_t> birhath_ha_hama(int32_t year) const;

        [[nodiscard]] std::vector<moment_t> samuel_season_in_gregorian(const angle_t& season, int32_t year) const;
        [[nodiscard]] std::vector<moment_t> add_season_in_gregorian(const angle_t& season, int32_t year) const;

        [[nodiscard]] std::vector<fixed_date_t> hanukkah(int32_t year) const;

        [[nodiscard]] fixed_date_t birthday(const datetypes::hebrew_date& birthdate, int32_t hyear) const;
        [[nodiscard]] fixed_date_t yahrzeit(const datetypes::hebrew_date& deathdate, int32_t hyear) const;

        [[nodiscard]] std::vector<fixed_date_t> birthday_in_gregorian(const datetypes::hebrew_date& birthdate, int32_t gyear) const;
        [[nodiscard]] std::vector<fixed_date_t> yahrzeit_in_gregorian(const datetypes::hebrew_date& deathdate, int32_t gyear) const;
    };
}