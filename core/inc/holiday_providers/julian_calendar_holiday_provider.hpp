#pragma once

#include "calendars/julian_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers{
    class julian_calendar_holiday_provider :
    public holiday_provider_if
    {
        const calendars::julian_calendar* m_julian;
        const calendars::gregorian_calendar* m_gregorian;

        fixed_date_t m_season_start;

    public:
        julian_calendar_holiday_provider();

        [[nodiscard]] const calendar_if* calendar() const override { return m_julian; }

        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(month cmonth, int32_t cday, int32_t gyear) const;

        [[nodiscard]] std::vector<fixed_date_t> orthodox_christmas(int32_t year) const;

        [[nodiscard]] fixed_date_t orthodox_easter(int32_t year) const;
        [[nodiscard]] fixed_date_t orthodox_pentecost(int32_t year) const;

        [[nodiscard]] std::vector<moment_t> season_in_gregorian(const angle_t& season, int32_t year) const;

        ~julian_calendar_holiday_provider() override = default;
    };
}