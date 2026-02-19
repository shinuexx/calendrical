#pragma once

#include "calendars/persian_based_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {
    class persian_based_calendar_holiday_provider :
        public holiday_provider_if {

        const calendars::persian_based_calendar* m_persian;

        int32_t m_gregorian_year_persian_epoch;

    public:
        explicit persian_based_calendar_holiday_provider(const calendars::persian_based_calendar* persian);
        ~persian_based_calendar_holiday_provider() override = default;

        [[nodiscard]] const calendar_if* calendar() const override;
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;

        fixed_date_t nawruz(int32_t year) const;
    };
}