#pragma once

#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers{
    class gregorian_calendar_holiday_provider final :
    public holiday_provider_if {
        const calendars::gregorian_calendar* m_gregorian;

    public:
        explicit gregorian_calendar_holiday_provider(const calendars::gregorian_calendar* gregorian) : m_gregorian(gregorian){}
        gregorian_calendar_holiday_provider() : m_gregorian(std::addressof(calendars::gregorian_calendar::standard)){}

        [[nodiscard]] const calendar_if* calendar() const override { return m_gregorian; }
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;

        [[nodiscard]] fixed_date_t christmas(int32_t year) const;
        fixed_date_t advent(int32_t year) const;
        fixed_date_t epiphany(int32_t year) const;
        fixed_date_t easter(int32_t year) const;
        fixed_date_t pentecost(int32_t year) const;

        std::vector<fixed_date_t> unlucky_fridays_in_range(const range<fixed_date_t>& range) const;
        std::vector<fixed_date_t> unlucky_fridays(int32_t year) const;

        fixed_date_t astronomical_easter(int32_t year) const;
    };
}