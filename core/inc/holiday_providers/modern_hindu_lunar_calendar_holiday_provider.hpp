#pragma once

#include "calendars/modern_hindu_lunar_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {

    class modern_hindu_lunar_calendar_holiday_provider :
        public holiday_provider_if{
        const calendars::modern_hindu_lunar_calendar* m_lunar;

    public:

        modern_hindu_lunar_calendar_holiday_provider(const calendars::modern_hindu_lunar_calendar* lunar) : m_lunar{ lunar } {}

        [[nodiscard]] const calendar_if* calendar() const override;
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;

        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(int32_t cmonth, int32_t day, int32_t gyear) const;

        [[nodiscard]] fixed_date_t date_occur(int32_t lyear, int32_t lmonth, int32_t lday) const;
        [[nodiscard]] std::vector<fixed_date_t> lunar_holiday(int32_t lmonth, int32_t lday, int32_t gyear) const;

        [[nodiscard]] std::vector<fixed_date_t> diwali(int32_t gyear) const;

        [[nodiscard]]fixed_date_t tithi_occur(int32_t lmonth, const double& tithi, const moment_t& t, int32_t lyear) const;
        [[nodiscard]] std::vector<fixed_date_t> lunar_event(int32_t lmonth, const double& tithi, const moment_t& t, int32_t gyear) const;

        [[nodiscard]] std::vector<fixed_date_t> shiva(int32_t gyear) const;
        [[nodiscard]] std::vector<fixed_date_t> rama(int32_t gyear) const;

        [[nodiscard]] static int32_t karana(int32_t n);

        [[nodiscard]] std::vector<fixed_date_t> sacred_wednesdays(int32_t gyear) const;
        [[nodiscard]] std::vector<fixed_date_t> sacred_wednesdays_in_range(const range<fixed_date_t>& range) const;
    };
}