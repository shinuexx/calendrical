#pragma once

#include "datetypes/simple_date.hpp"
#include "location.hpp"

namespace calendrical::calendars {
    /** @brief Alias for the simple date type used by French Republican calendar variants. */
    using simple_date = datetypes::simple_date;

    /**
     * @brief Base class for French Republican calendar implementations.
     */
    class french_based_calendar :
        public typed_calendar_if<simple_date>
    {
    protected:
        /** @brief French Republican epoch in fixed-date coordinates. */
        static constexpr fixed_date_t ms_epoch{654415};

        /** @brief Computes month from date and the year's new-year anchor. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date, const fixed_date_t& new_year) const;
        /** @brief Computes day-of-month from date and the year's new-year anchor. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date, const fixed_date_t& new_year) const;

    public:
        /** @brief Paris reference location used by astronomical French variants. */
        static constexpr location paris{angle_t::dms(48,50,11), angle_t::dms(2,20,15), 27, moment_t::hms(1)};

        /** @brief Constructs a French-based calendar instance. */
        french_based_calendar() = default;
        ~french_based_calendar() override = default;

        /** @brief Returns French Republican New Year for a year number. */
        [[nodiscard]] virtual fixed_date_t get_new_year(int32_t year) const = 0;
        /** @brief Returns the French Republican year containing @p date. */
        [[nodiscard]] virtual int32_t get_year(const fixed_date_t& date) const = 0;

        /** @brief Returns the French Republican epoch. */
        [[nodiscard]] fixed_date_t epoch() const override;

        /** @brief Converts a fixed date to French Republican simple-date representation. */
        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override = 0;
        /** @brief Converts French Republican simple-date representation to fixed date. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override = 0;

        /** @brief Returns the month containing @p date. */
        [[nodiscard]] virtual int32_t get_month(const fixed_date_t& date) const = 0;

        /** @brief Returns the day-of-month for @p date. */
        [[nodiscard]] virtual int32_t get_day(const fixed_date_t& date) const = 0;

        /** @brief Returns whether the French Republican year is leap. */
        [[nodiscard]] virtual bool is_leap_year(int32_t year) const = 0;
    };
}
