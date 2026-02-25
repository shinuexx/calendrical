#pragma once

#include "datetypes/lunar_date.hpp"
#include "modern_hindu_base.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    /**
     * @brief Astronomical Hindu lunar calendar implementation.
     */
    class astro_hindu_lunar_calendar final :
        public typed_calendar_if<lunar_date>,
        public modern_hindu_base
    {

    public:
        /** @brief Constructs an astronomical Hindu lunar calendar instance. */
        astro_hindu_lunar_calendar() = default;
        ~astro_hindu_lunar_calendar() override = default;

        /** @brief Returns the Hindu epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        /** @brief Converts a fixed date to astronomical Hindu lunar date components. */
        [[nodiscard]] lunar_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts astronomical Hindu lunar date components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const lunar_date& date) const override;

        /** @brief Returns the astronomical Hindu lunar year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        /** @brief Returns the astronomical Hindu lunar month containing @p date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        /** @brief Returns the astronomical Hindu lunar day number for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;

        /** @brief Returns whether @p date lies in an astronomical leap month. */
        [[nodiscard]] bool is_leap_month(const fixed_date_t& date) const;
        /** @brief Returns whether @p date is an astronomical leap day. */
        [[nodiscard]] bool is_leap_day(const fixed_date_t& date) const;
    };
}
