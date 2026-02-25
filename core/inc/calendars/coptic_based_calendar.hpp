#pragma once

#include "datetypes/simple_date.hpp"

namespace calendrical::calendars {
    /** @brief Alias for the simple date type used by Coptic/Ethiopic calendars. */
    using simple_date = datetypes::simple_date;

    /**
     * @brief Shared implementation for Coptic-style solar calendars.
     */
    class coptic_based_calendar :
    public typed_calendar_if<simple_date> {
        fixed_date_t m_epoch;

        /** @brief Internal month extractor using a resolved year. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date, int32_t year) const;
    public:
        // ReSharper disable once CppNonExplicitConvertingConstructor
        /** @brief Constructs a Coptic-style calendar with a custom epoch. */
        constexpr coptic_based_calendar(const fixed_date_t& epoch) : m_epoch(epoch) {} // NOLINT(*-explicit-constructor)

        ~coptic_based_calendar() override = default;

        /** @brief Returns the calendar epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return m_epoch; }

        /** @brief Returns the year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        /** @brief Returns New Year for the given year number. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;

        /** @brief Converts Coptic-style simple date to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;
        /** @brief Converts a fixed date to Coptic-style simple-date representation. */
        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;

        /** @brief Returns the month containing @p date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        /** @brief Returns the day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
        /** @brief Returns whether the year is leap in the Coptic-style scheme. */
        [[nodiscard]] bool is_leap_year(int32_t year) const;
    };

    /** @brief Standard Coptic calendar instance. */
    extern const coptic_based_calendar coptic_calendar;
    /** @brief Standard Ethiopic calendar instance. */
    extern const coptic_based_calendar ethiopic_calendar;
}
