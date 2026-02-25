#pragma once

#include "datetypes/lunar_date.hpp"
#include "modern_hindu_base.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    /**
     * @brief Modern Hindu lunar calendar implementation.
     */
    class modern_hindu_lunar_calendar :
        public typed_calendar_if<lunar_date>,
        public modern_hindu_base
    {

    public:
        /** @brief Constructs a modern Hindu lunar calendar using default location. */
        modern_hindu_lunar_calendar() = default;
        /** @brief Constructs a modern Hindu lunar calendar for a custom location. */
        explicit modern_hindu_lunar_calendar(const location& hindu_location) : modern_hindu_base{ hindu_location }{}
        ~modern_hindu_lunar_calendar() override = default;

        /** @brief Returns the Hindu epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        /** @brief Converts a fixed date to modern Hindu lunar date components. */
        [[nodiscard]] lunar_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts modern Hindu lunar date components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const lunar_date& date) const override;

        /** @brief Returns the modern Hindu lunar year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        /** @brief Returns the modern Hindu lunar month containing @p date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        /** @brief Returns the modern Hindu lunar day number for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;

        /** @brief Returns whether @p date lies in a leap month. */
        [[nodiscard]] bool is_leap_month(const fixed_date_t& date) const;
        /** @brief Returns whether @p date is a leap day. */
        [[nodiscard]] bool is_leap_day(const fixed_date_t& date) const;

        /** @brief Returns whether lunar month @p lmonth in year @p lyear is expunged. */
        [[nodiscard]] bool is_hindu_expunged(int32_t lyear, int32_t lmonth) const;

        /** @brief Converts to the full-moon month naming convention. */
        [[nodiscard]] lunar_date to_full_moon_date(const fixed_date_t& date) const;
        /** @brief Converts a full-moon-convention date to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_full_moon_fixed(lunar_date date) const;

        /** @brief Ordering predicate for Hindu lunar dates under library conventions. */
        [[nodiscard]] bool hindu_lunar_on_or_before(const lunar_date& date1, const lunar_date& date2) const;
    };
}
