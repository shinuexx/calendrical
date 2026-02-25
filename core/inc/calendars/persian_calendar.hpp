#pragma once

#include "persian_based_calendar.hpp"

namespace calendrical::calendars {

    /**
     * @brief Astronomical Persian calendar implementation (Tehran-based).
     */
    class persian_calendar: public persian_based_calendar {
    protected:
        /** @brief Returns apparent midday in Tehran for the given fixed date. */
        moment_t midday_in_tehran(const fixed_date_t& date) const;
        /** @brief Finds the Persian New Year on or before a fixed date. */
        fixed_date_t new_year_on_or_before(const fixed_date_t& date) const;

    public:
        /** @brief Returns Persian New Year (Nowruz) for a Persian year. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const override;
        /** @brief Returns the Persian year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const override;
        /** @brief Converts astronomical Persian date to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;
    };
}
