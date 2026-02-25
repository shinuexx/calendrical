#pragma once

#include "datetypes/simple_date.hpp"
#include "location.hpp"

namespace calendrical::calendars {
    /** @brief Alias for the simple date type used by Persian calendar variants. */
    using simple_date = datetypes::simple_date;

    /**
     * @brief Base class for Persian calendar implementations.
     */
    class persian_based_calendar :
        public typed_calendar_if<simple_date>
    {
    protected:
        /** @brief Persian calendar epoch in fixed-date coordinates. */
        static const fixed_date_t ms_epoch;

        /** @brief Returns month from day-of-year. */
        [[nodiscard]] int32_t get_month(int32_t day_of_year) const;
        /** @brief Returns day-of-year for a fixed date and resolved year. */
        [[nodiscard]] int32_t get_day_of_year(const fixed_date_t& date, int32_t year) const;
        /** @brief Returns day-of-month for a fixed date and resolved year/month. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date, int32_t year, int32_t month) const;

    public:
        /** @brief Tehran reference location used by astronomical Persian variants. */
        static constexpr location tehran{ 35.68, 51.42, 1100, moment_t::hms(3.5) };

        /** @brief Constructs a Persian-based calendar instance. */
        persian_based_calendar() = default;
        ~persian_based_calendar() override = default;

        /** @brief Returns Persian New Year (Nowruz) for a Persian year. */
        [[nodiscard]] virtual fixed_date_t get_new_year(int32_t year) const = 0;
        /** @brief Returns the Persian year containing @p date. */
        [[nodiscard]] virtual int32_t get_year(const fixed_date_t& date) const = 0;

        /** @brief Returns the Persian epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        /** @brief Converts a fixed date to a Persian simple-date representation. */
        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts a Persian simple-date representation to fixed date. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override = 0;

        /** @brief Returns the Persian month containing @p date. */
        [[nodiscard]] virtual int32_t get_month(const fixed_date_t& date) const;

        /** @brief Returns the Persian day-of-month for @p date. */
        [[nodiscard]] virtual int32_t get_day(const fixed_date_t& date) const;

        /** @brief Returns whether a Persian year is leap (implementation-specific rule). */
        [[nodiscard]] virtual bool is_leap_year(int32_t year) const;
    };
}
