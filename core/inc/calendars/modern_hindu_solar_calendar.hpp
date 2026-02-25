#pragma once

#include "datetypes/simple_date.hpp"
#include "modern_hindu_base.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    /**
     * @brief Modern Hindu solar calendar implementation.
     */
    class modern_hindu_solar_calendar :
        public typed_calendar_if<simple_date>,
        public modern_hindu_base
    {

    public:
        /** @brief Constructs a modern Hindu solar calendar using default location. */
        modern_hindu_solar_calendar() = default;
        /** @brief Constructs a modern Hindu solar calendar for a custom location. */
        explicit modern_hindu_solar_calendar(const location& hindu_location) : modern_hindu_base{ hindu_location }{}
        ~modern_hindu_solar_calendar() override = default;

        /** @brief Returns the Hindu epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        /** @brief Converts a fixed date to modern Hindu solar date components. */
        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts modern Hindu solar date components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;

        /** @brief Returns the modern Hindu solar year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        /** @brief Returns the modern Hindu solar month containing @p date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        /** @brief Returns the modern Hindu solar day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
    };
}
