#pragma once

#include "datetypes/hebrew_date.hpp"

namespace calendrical::calendars {
    /** @brief Alias for the Hebrew date type handled by Hebrew calendar variants. */
    using hebrew_date = datetypes::hebrew_date;

    /**
     * @brief Base class for Hebrew calendar implementations.
     */
    class hebrew_based_calendar : public typed_calendar_if<hebrew_date> {
    protected:
        /** @brief Hebrew epoch in fixed-date coordinates. */
        static constexpr fixed_date_t ms_epoch{ -1373427 };

    public:
        /** @brief Constructs a Hebrew-based calendar instance. */
        hebrew_based_calendar() = default;
        ~hebrew_based_calendar() override = default;

        /** @brief Returns Hebrew New Year (1 Tishri) for Hebrew year @p year. */
        [[nodiscard]] virtual fixed_date_t get_new_year(int32_t year) const = 0;
        /** @brief Returns the Hebrew year containing @p date. */
        [[nodiscard]] virtual int32_t get_year(const fixed_date_t& date) const = 0;

        /** @brief Returns the Hebrew epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        /** @brief Converts a fixed date to Hebrew Y-M-D components. */
        [[nodiscard]] hebrew_date to_date(const fixed_date_t& date) const override = 0;
        /** @brief Converts Hebrew Y-M-D components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const hebrew_date& date) const override = 0;

        /** @brief Returns the Hebrew month containing @p date. */
        [[nodiscard]] virtual hebrew_month get_month(const fixed_date_t& date) const = 0;

        /** @brief Returns the Hebrew day-of-month for @p date. */
        [[nodiscard]] virtual int32_t get_day(const fixed_date_t& date) const = 0;

        /** @brief Returns the last valid day number in Hebrew month @p hm of year @p year. */
        [[nodiscard]] virtual int32_t last_day_of_month(int32_t year, hebrew_month hm) const;
        /** @brief Returns whether Kislev is short in Hebrew year @p year. */
        [[nodiscard]] virtual bool is_short_kislev(int32_t year) const;
        /** @brief Returns whether Marheshvan is long in Hebrew year @p year. */
        [[nodiscard]] virtual bool is_long_marheshvan(int32_t year) const;
        /** @brief Returns the number of days in Hebrew year @p year. */
        [[nodiscard]] virtual int32_t days_in_year(int32_t year) const;

        /** @brief Returns whether Hebrew year @p year is leap. */
        [[nodiscard]] virtual bool is_leap_year(int32_t year) const = 0;

        /** @brief Returns the final month of Hebrew year @p year (Adar / Adar II). */
        [[nodiscard]] virtual hebrew_month last_month_of_year(int32_t year) const;
        /** @brief Validates if the given date is valid for this calendar. */
        [[nodiscard]] bool is_valid_date(const hebrew_date& date) const override;
    };
}
