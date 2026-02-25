#pragma once

#include "persian_based_calendar.hpp"

namespace calendrical::calendars {
    /**
     * @brief Arithmetic (tabular) Persian calendar implementation.
     */
    class arithmetic_persian_calendar final : public persian_based_calendar {
    public:
        /** @brief Returns Persian New Year (Nowruz) for a Persian year. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const override;
        /** @brief Returns the Persian year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const override;

        /** @brief Converts arithmetic Persian date to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;
        /** @brief Returns whether a Persian year is leap in the arithmetic scheme. */
        [[nodiscard]] bool is_leap_year(int32_t year) const override;
    };
}
