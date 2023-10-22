#pragma once

#include "interfaces.hpp"
#include "calendars/hebrew_based_calendar.hpp"
#include "coptic_based_calendar_holiday_provider.hpp"

namespace calendrical::holiday_providers {
    class hebrew_based_calendar_holiday_provider :
        public holiday_provider_if,
        public in_gregorian_aware_provider_if<hebrew_month>{
        
        std::shared_ptr<const hebrew_based_calendar> m_hebrew;
        std::shared_ptr<const gregorian_calendar> m_gregorian;
        std::shared_ptr<const coptic_based_calendar> m_coptic;
        std::shared_ptr<const coptic_based_calendar_holiday_provider> m_coptic_provider;

        int32_t m_gregorian_year_hebrew_epoch;

        fixed_date_t start_1_adar_21;
        fixed_date_t start_1_adar_28;

        static const rational ADDA_Y;

    public:
        hebrew_based_calendar_holiday_provider(std::shared_ptr<const hebrew_based_calendar> hebrew);
        virtual ~hebrew_based_calendar_holiday_provider() = default;

        virtual std::shared_ptr<const calendar_if> get_calendar() const override;
        virtual std::list<holiday_t> get_holiday_in_year(int32_t gyear) const override;

        virtual std::list<fixed_date_t> in_gregorian(hebrew_month hmonth, int32_t hday, int32_t gyear) const override;

        fixed_date_t yom_kippur(int32_t year) const;
        fixed_date_t passover(int32_t year) const;
        fixed_date_t purim(int32_t year) const;
        fixed_date_t ta_anit_esther(int32_t year) const;
        fixed_date_t tishah_be_av(int32_t year) const;
        fixed_date_t yom_ha_zikkaron(int32_t year) const;

        std::list<fixed_date_t> sh_ela(int32_t year) const;
        std::list<fixed_date_t> birhath_ha_hama(int32_t year) const;

        std::list<moment_t> samuel_season_in_gregorian(const angle_t& season, int32_t year) const;
        std::list<rational_moment_t> add_season_in_gregorian(const angle_t& season, int32_t year) const;

        std::list<fixed_date_t> hanukkah(int32_t year) const;

        fixed_date_t birthday(const hebrew_date& birthdate, int32_t hyear) const;
        fixed_date_t yahrzeit(const hebrew_date& deathdate, int32_t hyear) const;

        std::list<fixed_date_t> birthday_in_gregorian(const hebrew_date& birthdate, int32_t gyear) const;
        std::list<fixed_date_t> yahrzeit_in_gregorian(const hebrew_date& deathdate, int32_t gyear) const;
    };
}