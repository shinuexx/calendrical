#pragma once

#include "islamic_based_calendar.hpp"

namespace calendrical::calendars{
    /**
     * @brief Arithmetic/tabular Islamic calendar implementation.
     */
    class islamic_calendar : public islamic_based_calendar{
        /** @brief Internal month extractor using a precomputed year value. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date, int32_t year) const;

    public:
        /** @brief Constructs an Islamic calendar instance. */
        islamic_calendar() = default;
        ~islamic_calendar() override = default;

        /** @brief Returns the Islamic year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const override;
        /** @brief Returns the Islamic month containing @p date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const override;
        /** @brief Returns the Islamic day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const override;

        /** @brief Converts a fixed date to Islamic simple-date components. */
        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts Islamic simple-date components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;

        /** @brief Returns whether an Islamic year is leap in the tabular cycle. */
        [[nodiscard]] bool is_leap_year(int32_t year) const;
    };
}
