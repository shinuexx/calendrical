#pragma once

#include "gregorian_calendar.hpp"
#include "datetypes/iso_date.hpp"

namespace calendrical::calendars{
    /** @brief Alias for the date type handled by @ref iso_calendar. */
    using iso_date = datetypes::iso_date;
    /**
     * @brief ISO week-date calendar built on top of the Gregorian calendar.
     */
    class iso_calendar final :
        public typed_calendar_if<iso_date>
    {
        /** @brief Gregorian calendar used to locate ISO year boundaries. */
        const gregorian_calendar* m_gregorian;

        /** @brief Fixed date of the calendar epoch. */

        fixed_date_t m_epoch;

        /** @brief Internal week extractor using an already resolved ISO @p year. */
        [[nodiscard]] int32_t get_week(const fixed_date_t& date, int32_t year) const;

    public:
        /** @brief Constructs an ISO calendar using the standard Gregorian calendar backend. */
        iso_calendar();

        ~iso_calendar() override = default;

        /** @brief Returns the ISO calendar epoch in fixed-date coordinates. */
        [[nodiscard]] fixed_date_t epoch() const override;

        /** @brief Returns the first day of the ISO year. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;

        /** @brief Converts a fixed date to ISO year-week-day. */
        [[nodiscard]] iso_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts ISO year-week-day to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const iso_date& date) const override;

        /** @brief Returns the ISO week-numbering year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        /** @brief Returns whether the corresponding Gregorian year is leap. */
        [[nodiscard]] bool is_leap_year(int32_t year) const;
        /** @brief Returns whether the ISO year has 53 weeks. */
        [[nodiscard]] bool is_long_year(int32_t year) const;

        /** @brief Returns the ISO weekday component for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
        /** @brief Returns the ISO week number for @p date. */
        [[nodiscard]] int32_t get_week(const fixed_date_t& date) const;
    };
}
