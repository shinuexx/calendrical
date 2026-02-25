#pragma once

#include "calendars/coptic_based_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {
    /**
     * @brief Holiday provider for Coptic/Ethiopic calendars.
     */
    class coptic_based_calendar_holiday_provider :
    public holiday_provider_if
    {
        const calendars::coptic_based_calendar* m_calendar;
        const calendars::gregorian_calendar* m_gregorian;

    public:
        /** @brief Constructs a provider bound to a specific Coptic-style calendar instance. */
        explicit coptic_based_calendar_holiday_provider(const calendars::coptic_based_calendar* calendar) :
            m_calendar(calendar),
            m_gregorian(std::addressof(calendars::gregorian_calendar::standard)) {}

        /** @brief Returns the backing calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override { return m_calendar; }

        /** @brief Returns the provider's holiday set for Gregorian year @p year. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t year) const override;

        /** @brief Maps Coptic-style month/day occurrences into Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(int32_t cmonth, int32_t cday, int32_t gyear) const;

        /** @brief Christmas dates associated with Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> christmas(int32_t year) const;
    };
}
