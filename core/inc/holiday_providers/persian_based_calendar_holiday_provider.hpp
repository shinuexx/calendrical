#pragma once

#include "calendars/persian_based_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {
    /**
     * @brief Holiday provider for Persian-calendar holidays.
     */
    class persian_based_calendar_holiday_provider :
        public holiday_provider_if {

        const calendars::persian_based_calendar* m_persian;

        int32_t m_gregorian_year_persian_epoch;

    public:
        /** @brief Constructs a provider bound to a specific Persian calendar implementation. */
        explicit persian_based_calendar_holiday_provider(const calendars::persian_based_calendar* persian);
        ~persian_based_calendar_holiday_provider() override = default;

        /** @brief Returns the backing calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override;
        /** @brief Returns the provider's holiday set for Gregorian year @p gyear. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;

        /** @brief Nawruz date associated with Gregorian year @p year. */
        fixed_date_t nawruz(int32_t year) const;
    };
}
