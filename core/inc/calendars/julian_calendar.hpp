#pragma once

#include "datetypes/julian_date.hpp"
#include "types.hpp"
#include "fixed_date.hpp"

namespace calendrical::calendars{
    /** @brief Alias for the date type handled by @ref julian_calendar. */
    using julian_date = datetypes::julian_date;

    /**
     * @brief Proleptic Julian calendar conversion API.
     */
    class julian_calendar final :
    public typed_calendar_if<julian_date>{

        /** @brief Calendar epoch in fixed-date coordinates. */
        static constexpr fixed_date_t ms_epoch{-1};

        /** @brief Internal month extractor using an already resolved Julian @p year. */
        [[nodiscard]] month get_month(const fixed_date_t& date, int32_t year) const;

    public:
        /** @brief Constructs a Julian calendar instance. */
        constexpr julian_calendar() = default;

        /** @brief Returns January 1 of the given Julian year. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;
        /** @brief Returns the Julian year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        /** @brief Returns the Julian day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
        /** @brief Returns whether the Julian year is leap. */
        [[nodiscard]] bool is_leap_year(int32_t year) const;

        /** @brief Returns the calendar epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }
        /** @brief Converts a fixed date to Julian Y-M-D. */
        [[nodiscard]] julian_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts Julian Y-M-D to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const julian_date& date) const override;
        /** @brief Validates if the given date is valid for this calendar. */
        [[nodiscard]] bool is_valid_date(const julian_date& date) const override;

        /** @brief Returns the Julian month containing @p date. */
        [[nodiscard]] month get_month(const fixed_date_t& date) const;
    };
}
