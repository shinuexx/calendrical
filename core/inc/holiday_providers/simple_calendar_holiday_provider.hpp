#pragma once

#include "calendars/simple_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers{
    /**
     * @brief Holiday provider helper for fixed-pattern holidays in a @ref simple_calendar.
     */
    class simple_calendar_holiday_provider final :
        public holiday_provider_if
    {
        const calendars::simple_calendar* m_calendar;
        const calendars::gregorian_calendar* m_gregorian;

    public:
        /** @brief Constructs a provider bound to a specific simple calendar instance. */
        explicit simple_calendar_holiday_provider(const calendars::simple_calendar* calendar) : m_calendar(calendar), m_gregorian(std::addressof(calendars::gregorian_calendar::standard)){}
        ~simple_calendar_holiday_provider() override = default;

        /** @brief Returns the backing calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override { return m_calendar; };
        /** @brief Returns holidays for Gregorian year @p gyear (empty in this base helper). */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override { return {}; }

        /** @brief Maps a simple-calendar month/day occurrence into Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(int32_t cmonth, int32_t cday, int32_t gyear) const;
    };
}
