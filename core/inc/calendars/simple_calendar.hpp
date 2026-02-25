#pragma once

#include "../datetypes/simple_date.hpp"

namespace calendrical::calendars{
    /** @brief Alias for the date type handled by @ref simple_calendar. */
    using simple_date = datetypes::simple_date;
    /**
     * @brief Generic fixed-length 12x30-style calendar with configurable epoch.
     */
    class simple_calendar final :
        public typed_calendar_if<simple_date>
    {
        fixed_date_t m_epoch;

    public:
        /** @brief Constructs a simple calendar with the given fixed-date epoch. */
        explicit constexpr simple_calendar(const fixed_date_t& epoch) : m_epoch(epoch) {} // NOLINT(*-explicit-constructor)

        ~simple_calendar() override = default;

        /** @brief Returns the calendar epoch. */
        [[nodiscard]] constexpr fixed_date_t epoch() const override { return m_epoch; }

        /** @brief Converts a fixed date to a @ref calendrical::datetypes::simple_date. */
        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts a @ref calendrical::datetypes::simple_date to a fixed date. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;

        /** @brief Returns the first day of the given year. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;
        /** @brief Returns the year component for a fixed date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;

        /** @brief Returns the month component for a fixed date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        /** @brief Returns the day component for a fixed date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
    };

    /** @brief Egyptian calendar instance (simple-calendar variant). */
    extern const simple_calendar egyptian_calendar;
    /** @brief Armenian calendar instance (simple-calendar variant). */
    extern const simple_calendar armenian_calendar;
}
