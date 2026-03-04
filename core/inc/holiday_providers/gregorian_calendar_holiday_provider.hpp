#pragma once

#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers{
    /**
     * @brief Holiday provider for common Gregorian/Christian movable feasts.
     */
    class gregorian_calendar_holiday_provider final :
    public holiday_provider_if {
        const calendars::gregorian_calendar* m_gregorian;

    public:
        /** @brief Constructs a provider bound to a specific Gregorian calendar instance. */
        explicit gregorian_calendar_holiday_provider(const calendars::gregorian_calendar* gregorian) : m_gregorian(gregorian){}
        /** @brief Constructs a provider using the standard Gregorian calendar singleton. */
        gregorian_calendar_holiday_provider() : m_gregorian(std::addressof(calendars::gregorian_calendar::standard)){}

        /** @brief Returns the backing calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override { return m_gregorian; }
        /** @brief Returns the provider's holiday set for Gregorian year @p gyear. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;

        /** @brief Christmas date in Gregorian year @p year. */
        [[nodiscard]] fixed_date_t christmas(int32_t year) const;
        /** @brief Advent anchor date in Gregorian year @p year. */
        fixed_date_t advent(int32_t year) const;
        /** @brief Epiphany date in Gregorian year @p year. */
        fixed_date_t epiphany(int32_t year) const;
        /** @brief Western Easter Sunday in Gregorian year @p year. */
        fixed_date_t easter(int32_t year) const;
        /** @brief Pentecost date derived from Easter in Gregorian year @p year. */
        fixed_date_t pentecost(int32_t year) const;

        /** @brief Friday-the-13th dates within an explicit fixed-date range. */
        std::vector<fixed_date_t> unlucky_fridays_in_range(const interval<fixed_date_t>& range) const;
        /** @brief Friday-the-13th dates in Gregorian year @p year. */
        std::vector<fixed_date_t> unlucky_fridays(int32_t year) const;

        /** @brief Astronomical Easter approximation/date variant for Gregorian year @p year. */
        fixed_date_t astronomical_easter(int32_t year) const;
    };
}
