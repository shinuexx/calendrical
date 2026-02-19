#pragma once

#include "types.hpp"
#include "calendars/islamic_based_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {

    class islamic_based_calendar_holiday_provider:
    public holiday_provider_if {
        const calendars::islamic_based_calendar *m_islamic;
        const calendars::gregorian_calendar *m_gregorian;

    public:
        explicit islamic_based_calendar_holiday_provider(const calendars::islamic_based_calendar *islamic) :
            m_islamic(islamic),
            m_gregorian(std::addressof(calendars::gregorian_calendar::standard)){}

        ~islamic_based_calendar_holiday_provider() override = default;

        [[nodiscard]] const calendar_if* calendar() const override { return m_islamic; }
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t year) const override;
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(int32_t imonth, int32_t iday, int32_t gyear) const;

        [[nodiscard]] std::vector<fixed_date_t> mawlid(int32_t year) const;
    };
}