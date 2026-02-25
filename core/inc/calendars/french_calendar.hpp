#pragma once

#include "french_based_calendar.hpp"

namespace calendrical::calendars{
    /**
     * @brief Astronomical French Republican calendar implementation (Paris-based).
     */
    class french_calendar : public french_based_calendar {
    protected:
        using french_based_calendar::get_month;
        using french_based_calendar::get_day;

        /** @brief Apparent midnight in Paris for the given fixed date. */
        [[nodiscard]] moment_t midnight_in_paris(const fixed_date_t& date) const;
        /** @brief Finds French Republican New Year on or before a fixed date. */
        [[nodiscard]] fixed_date_t new_year_on_or_before(const fixed_date_t& date) const;

    public:
        /** @brief Constructs an astronomical French Republican calendar instance. */
        french_calendar() = default;
        ~french_calendar() override = default;

        /** @brief Returns French Republican New Year for a year number. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const override;
        /** @brief Returns the French Republican year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const override;

        /** @brief Converts a fixed date to French Republican simple-date representation. */
        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts French Republican simple-date representation to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;

        /** @brief Returns the month containing @p date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const override;

        /** @brief Returns the day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const override;

        /** @brief Returns whether the French Republican year is leap. */
        [[nodiscard]] bool is_leap_year(int32_t year) const override;
    };
}
