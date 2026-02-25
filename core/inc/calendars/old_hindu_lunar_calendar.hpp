#pragma once

#include "types.hpp"
#include "hindu_base.hpp"
#include "datetypes/simple_lunar_date.hpp"

namespace calendrical::calendars {
    /** @brief Alias for the lunar date type used by old Hindu lunar calendar. */
    using simple_lunar_date = datetypes::simple_lunar_date;

    /**
     * @brief Old Hindu lunar calendar (Arya Siddhanta style) implementation.
     */
    class old_hindu_lunar_calendar :
        public typed_calendar_if<simple_lunar_date>,
        public hindu_base {

        /** @brief Internal solar position value for a fixed date. */
        [[nodiscard]] double get_sun(const fixed_date_t& date) const;
        /** @brief Internal new-moon reference from solar position. */
        [[nodiscard]] double get_new_moon(const double& sun) const;
        /** @brief Internal month extraction from new-moon position. */
        [[nodiscard]] int32_t get_month(const double& new_moon) const;
        /** @brief Internal day extraction from solar position. */
        [[nodiscard]] int32_t get_day(const double& sun) const;
        /** @brief Internal year extraction from new-moon position. */
        [[nodiscard]] int32_t get_year(const double& new_moon) const;
        /** @brief Internal leap-month extraction from new-moon position. */
        [[nodiscard]] bool get_leap(const double& new_moon) const;

    public:
        /** @brief Constructs an old Hindu lunar calendar instance. */
        old_hindu_lunar_calendar() = default;
        ~old_hindu_lunar_calendar() override = default;

        /** @brief Returns New Year for an old Hindu lunar year. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;
        /** @brief Returns the old Hindu lunar year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;

        /** @brief Returns the Hindu epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        /** @brief Converts a fixed date to old Hindu lunar date components. */
        [[nodiscard]] simple_lunar_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts old Hindu lunar date components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_lunar_date& date) const override;

        /** @brief Returns the lunar month containing @p date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        /** @brief Returns the lunar day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;

        /** @brief Returns whether an old Hindu lunar year is leap. */
        [[nodiscard]] bool is_leap_year(int32_t year) const;
    };
}
