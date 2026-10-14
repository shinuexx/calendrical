#pragma once

#include "datetypes/simple_lunar_date.hpp"
#include "location.hpp"

namespace calendrical::calendars {
    /** @brief Alias for the Babylonian date type (`simple_lunar_date`). */
    using simple_lunar_date = datetypes::simple_lunar_date;
    /**
     * @brief Babylonian observational lunisolar calendar implementation.
     */
    class babylonian_calendar final :
        public typed_calendar_if<simple_lunar_date> {

        /** @brief Babylonian epoch in fixed-date coordinates. */
        static constexpr fixed_date_t ms_epoch { -113502 };
        /** @brief Babylon reference location for visibility calculations. */
        static constexpr location babylon{ 32.4794_deg, 44.4328_deg, 26, moment_t::hms(3.5) };

        /** @brief Crescent-visibility criterion used by the Babylonian model. */
        static bool babylonian_criterion(const fixed_date_t& date);

        /** @brief Start of the current/previous Babylonian month on or before @p date. */
        [[nodiscard]] fixed_date_t new_month_on_or_before(const fixed_date_t& date) const;

    public:
        /** @brief Constructs a Babylonian calendar instance. */
        babylonian_calendar() = default;
        ~babylonian_calendar() override = default;

        /** @brief Returns whether Babylonian year @p year is leap. */
        [[nodiscard]] bool is_leap_year(int32_t year) const;

        /** @brief Returns the Babylonian epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        /** @brief Converts Babylonian date components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_lunar_date& date) const override;
        /** @brief Converts a fixed date to Babylonian date components. */
        [[nodiscard]] simple_lunar_date to_date(const fixed_date_t& date) const override;

        /** @brief Returns the Babylonian year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        /** @brief Returns the Babylonian month containing @p date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        /** @brief Returns whether @p date lies in a Babylonian leap month. */
        [[nodiscard]] bool is_leap_month(const fixed_date_t& date) const;
        /** @brief Returns the Babylonian day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
    };
}
