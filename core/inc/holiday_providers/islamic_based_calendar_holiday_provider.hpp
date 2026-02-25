#pragma once

#include "types.hpp"
#include "calendars/islamic_based_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {

    /**
     * @brief Holiday provider for Islamic-calendar holidays.
     */
    class islamic_based_calendar_holiday_provider:
    public holiday_provider_if {
        const calendars::islamic_based_calendar *m_islamic;
        const calendars::gregorian_calendar *m_gregorian;

    public:
        /** @brief Constructs a provider bound to a specific Islamic calendar implementation. */
        explicit islamic_based_calendar_holiday_provider(const calendars::islamic_based_calendar *islamic) :
            m_islamic(islamic),
            m_gregorian(std::addressof(calendars::gregorian_calendar::standard)){}

        ~islamic_based_calendar_holiday_provider() override = default;

        /** @brief Returns the backing Islamic calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override { return m_islamic; }
        /** @brief Returns the provider's holiday set for Gregorian year @p year. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t year) const override;
        /** @brief Maps Islamic month/day occurrences into Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(int32_t imonth, int32_t iday, int32_t gyear) const;

        /** @brief Mawlid dates in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> mawlid(int32_t year) const;
    };
}
