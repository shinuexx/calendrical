#pragma once

#include "hebrew_based_calendar.hpp"
#include "moment.hpp"

namespace calendrical::calendars {

    /**
     * @brief Arithmetic/traditional Hebrew calendar implementation.
     */
    class hebrew_calendar: public hebrew_based_calendar {
        /** @brief Fractional-day offset for the molad epoch part. */
        static constexpr double molad_part{ 876.0 / 25920 };
        /** @brief Mean lunation length used for molad calculations (days). */
        static constexpr double molad_month_length = 29 + 0.5 + 793.0/25920; // { 765433.0 / 25920 }; //= 29 + rational{ 1,2 } + rational{ 793,25920 };
        /** @brief Reference molad of 1 Tishri relative to the Hebrew epoch. */
        static constexpr double molad_1_tishri = static_cast<int64_t>(ms_epoch) - molad_part;

        /** @brief Internal month extractor using a resolved Hebrew year. */
        [[nodiscard]] hebrew_month get_month(const fixed_date_t& date, int32_t year) const;

        /** @brief Year-length adjustment from postponement rules. */
        [[nodiscard]] int64_t year_length_correction(int32_t year) const;
        /** @brief Elapsed days from Hebrew epoch to the start of year @p year. */
        [[nodiscard]] int64_t elapsed_days(int32_t year) const;

        /** @brief Lower bound for fast search among possible New Year dates. */
        const fixed_date_t m_possible_days_begin;

    public:
        /** @brief Constructs a Hebrew calendar instance. */
        hebrew_calendar();
        ~hebrew_calendar() override = default;

        /** @brief Returns whether Hebrew year @p year is leap. */
        [[nodiscard]] bool is_leap_year(int32_t year) const override;
        /** @brief Validates a date against the arithmetic month lengths of year @p date.year. */
        [[nodiscard]] bool is_valid_date(const hebrew_date& date) const override;
        /** @brief Returns whether Hebrew year @p year is a sabbatical year. */
        [[nodiscard]] bool is_sabbatical_year(int32_t year) const;

        /** @brief Returns Hebrew New Year for Hebrew year @p year. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const override;
        /** @brief Returns the Hebrew year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const override;

        /** @brief Converts a fixed date to Hebrew Y-M-D components. */
        [[nodiscard]] hebrew_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts Hebrew Y-M-D components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const hebrew_date& date) const override;

        /** @brief Returns the Hebrew month containing @p date. */
        [[nodiscard]] hebrew_month get_month(const fixed_date_t& date) const override;

        /** @brief Returns the Hebrew day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const override;

        /** @brief Mean lunar conjunction (molad) for a Hebrew month/year. */
        [[nodiscard]] static moment_t molad(int32_t year, hebrew_month month);

        /** @brief Converts a molad moment to its associated fixed date. */
        [[nodiscard]] static fixed_date_t fixed_from_molad(const moment_t& moon);

        /** @brief Returns weekdays on which a given Hebrew month/day can occur. */
        [[nodiscard]] std::vector<weekday> possible_days(hebrew_month hmonth, int32_t hday) const;
    };
}
