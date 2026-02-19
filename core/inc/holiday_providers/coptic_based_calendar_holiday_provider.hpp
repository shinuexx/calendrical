#pragma once

#include "calendars/coptic_based_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {
    class coptic_based_calendar_holiday_provider :
    public holiday_provider_if
    {
        const calendars::coptic_based_calendar* m_calendar;
        const calendars::gregorian_calendar* m_gregorian;

    public:
        explicit coptic_based_calendar_holiday_provider(const calendars::coptic_based_calendar* calendar) :
            m_calendar(calendar),
            m_gregorian(std::addressof(calendars::gregorian_calendar::standard)) {}

        [[nodiscard]] const calendar_if* calendar() const override { return m_calendar; }

        [[nodiscard]] std::vector<holiday_t> in_year(int32_t year) const override;

        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(int32_t cmonth, int32_t cday, int32_t gyear) const;

        [[nodiscard]] std::vector<fixed_date_t> christmas(int32_t year) const;
    };
}