#pragma once

#include "french_based_calendar.hpp"

namespace calendrical::calendars{
    /**
     * @brief Arithmetic (tabular) French Republican calendar implementation.
     */
    class arithmetic_french_calendar final :
        public french_based_calendar {
    protected:
        using french_based_calendar::get_month;
        using french_based_calendar::get_day;

    public:
        /** @brief Constructs an arithmetic French Republican calendar instance. */
        arithmetic_french_calendar() = default;
        ~arithmetic_french_calendar() override = default;

        /** @brief Returns French Republican New Year for a year number. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const override;
        /** @brief Returns the French Republican year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const override;

        /** @brief Converts a fixed date to French Republican simple-date representation. */
        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts French Republican simple-date representation to fixed date. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;

        /** @brief Returns the month containing @p date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const override;

        /** @brief Returns the day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const override;

        /** @brief Returns whether the year is leap in the arithmetic scheme. */
        [[nodiscard]] bool is_leap_year(int32_t year) const override;
    };
}
