#pragma once

#include "types.hpp"
#include "hindu_base.hpp"
#include "datetypes/simple_date.hpp"

namespace calendrical::calendars {
    /** @brief Alias for the simple date type handled by old Hindu solar calendar. */
    using simple_date = datetypes::simple_date;

    /**
     * @brief Old Hindu solar calendar (Arya Siddhanta style) implementation.
     */
    class old_hindu_solar_calendar final :
        public typed_calendar_if<simple_date>,
        public hindu_base
    {
        /** @brief Internal solar position value for a fixed date. */
        [[nodiscard]] double get_sun(const fixed_date_t& date) const;
        /** @brief Internal year extraction from solar position. */
        [[nodiscard]] int32_t get_year(const double& sun) const;
        /** @brief Internal month extraction from solar position. */
        [[nodiscard]] int32_t get_month(const double& sun) const;
        /** @brief Internal day extraction from solar position. */
        [[nodiscard]] int32_t get_day(const double& sun) const;

    public:
        /** @brief Constructs an old Hindu solar calendar instance. */
        old_hindu_solar_calendar() = default;
        ~old_hindu_solar_calendar() override = default;

        /** @brief Returns New Year for an old Hindu solar year. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;
        /** @brief Returns the old Hindu solar year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;

        /** @brief Returns the Hindu epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        /** @brief Converts a fixed date to old Hindu solar date components. */
        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts old Hindu solar date components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;

        /** @brief Returns the month containing @p date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        /** @brief Returns the day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
    };
}
