#pragma once

#include "datetypes/simple_date.hpp"

namespace calendrical::calendars{
    /** @brief Alias for the simple date type used by Islamic calendar variants. */
    using simple_date = datetypes::simple_date;

    /**
     * @brief Base class for Islamic calendar implementations.
     */
    class islamic_based_calendar : public typed_calendar_if<simple_date>{
    protected:
        /** @brief Islamic epoch in fixed-date coordinates. */
        static constexpr fixed_date_t ms_epoch{ 227015 };

    public:
        /** @brief Constructs an Islamic-based calendar instance. */
        islamic_based_calendar() = default;
        ~islamic_based_calendar() override = default;

        /** @brief Returns the first day of the given Islamic year. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const { return to_fixed({year, 1, 1}); }
        /** @brief Returns the Islamic year containing @p date. */
        [[nodiscard]] virtual int32_t get_year(const fixed_date_t &date) const = 0;

        /** @brief Returns the Islamic epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        /** @brief Converts an Islamic simple date to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override = 0;
        /** @brief Converts a fixed date to Islamic simple-date representation. */
        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override = 0;

        /** @brief Returns the Islamic month containing @p date. */
        [[nodiscard]] virtual int32_t get_month(const fixed_date_t& date) const = 0;

        /** @brief Returns the Islamic day-of-month for @p date. */
        [[nodiscard]] virtual int32_t get_day(const fixed_date_t& date) const = 0;
    };
}
