#pragma once

#include "datetypes/gregorian_date.hpp"
#include "interval.hpp"
#include "moment.hpp"
#include "angle.hpp"


namespace calendrical::calendars{
    /** @brief Alias for the date type handled by @ref gregorian_calendar. */
    using gregorian_date = datetypes::gregorian_date;
    /**
     * @brief Proleptic Gregorian calendar conversion and utility API.
     */
    class gregorian_calendar final :
    public typed_calendar_if<gregorian_date>
    {
        /** @brief Fixed date of the calendar epoch. */
        fixed_date_t m_epoch;

        /** @brief Internal month extractor using an already resolved Gregorian @p year. */
        [[nodiscard]] month get_month(const fixed_date_t &date, int32_t year) const;

    public:
        /** @brief Constructs the standard Gregorian calendar (epoch fixed date 1). */
        constexpr gregorian_calendar() : m_epoch{1} {}
        // ReSharper disable once CppNonExplicitConvertingConstructor
        /** @brief Constructs a Gregorian calendar with a custom epoch. */
        constexpr gregorian_calendar(const fixed_date_t &date) : m_epoch{date} {} // NOLINT(*-explicit-constructor)

        constexpr ~gregorian_calendar() override = default;

        /** @brief Returns the calendar epoch. */
        [[nodiscard]] constexpr fixed_date_t epoch() const override { return m_epoch; }

        /** @brief Converts a fixed date to Gregorian Y-M-D. */
        [[nodiscard]] gregorian_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts Gregorian Y-M-D to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const gregorian_date& date) const override;
        /** @brief Validates if the given date is valid for this calendar. */
        [[nodiscard]] bool is_valid_date(const gregorian_date& date) const override;

        /** @brief Returns January 1 of the given Gregorian year. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;
        /** @brief Returns the Gregorian year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t &date) const;

        /** @brief Returns the Gregorian month containing @p date. */
        [[nodiscard]] month get_month(const fixed_date_t &date) const;
        /** @brief Returns the Gregorian day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t &date) const;
        /** @brief Returns whether the Gregorian year is leap. */
        [[nodiscard]] bool is_leap_year(int32_t year) const;

        /** @brief Returns the last fixed date in the given Gregorian year. */
        [[nodiscard]] fixed_date_t get_year_end(int32_t year) const;
        /** @brief Returns the inclusive fixed-date range for a Gregorian year. */
        [[nodiscard]] interval<fixed_date_t> get_year_range(int32_t year) const;

        /** @brief Returns days remaining in the Gregorian year after @p date. */
        [[nodiscard]] int32_t get_remaining_days(const gregorian_date &date) const;

        /** @brief Returns the n-th occurrence of weekday @p k relative to @p date. */
        [[nodiscard]] fixed_date_t nth_weekday(int64_t n, weekday k, const gregorian_date &date) const;

        /** @brief Returns the first weekday @p k on or after the date anchor. */
        [[nodiscard]] fixed_date_t first_weekday(weekday k, const gregorian_date &date) const;
        /** @brief Returns the last weekday @p k on or before the date anchor. */
        [[nodiscard]] fixed_date_t last_weekday(weekday k, const gregorian_date &date) const;

        /** @brief Returns solar cycle events that fall within a Gregorian year. */
        [[nodiscard]] std::ranges::transform_view<std::ranges::owning_view<std::vector<double>>, bind_constructor<moment_t>> cycle_in_gregorian(const angle_t &season, int32_t year, double L, const moment_t& start) const;

        /** @brief Standard Gregorian calendar singleton instance. */
        static const gregorian_calendar standard;
    };

    inline constexpr gregorian_calendar gregorian_calendar::standard{};
}
