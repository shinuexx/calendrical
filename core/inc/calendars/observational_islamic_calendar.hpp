#pragma once

#include "islamic_based_calendar.hpp"
#include "location.hpp"

namespace calendrical::calendars {
    /**
     * @brief Observational Islamic calendar based on visible crescent calculations.
     */
    class observational_islamic_calendar : public islamic_based_calendar {

        /** @brief Finds crescent-based month start near @p date. */
        [[nodiscard]] fixed_date_t get_crescent(const fixed_date_t& date) const;
        /** @brief Elapsed months since epoch from a crescent date. */
        [[nodiscard]] int32_t get_elapsed_months(const fixed_date_t& crescent) const;

        /** @brief Resolves Islamic year from elapsed months. */
        [[nodiscard]] int32_t get_year(int32_t elapsed_months) const;
        /** @brief Resolves Islamic month from elapsed months. */
        [[nodiscard]] int32_t get_month(int32_t elapsed_months) const;
        /** @brief Resolves Islamic day-of-month from date and crescent start. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date, const fixed_date_t& crescent) const;

    public:
        /** @brief Reference location used by the observational Islamic model. */
        static constexpr location islamic_location{ 30.1, 31.3, 200, moment_t::hms(2) };

        /** @brief Constructs an observational Islamic calendar instance. */
        observational_islamic_calendar() = default;
        ~observational_islamic_calendar() override = default;

        /** @brief Returns the observational Islamic year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t &date) const override;

        /** @brief Converts a fixed date to observational Islamic simple-date components. */
        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts observational Islamic simple-date components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;

        /** @brief Returns the observational Islamic month containing @p date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t &date) const override;

        /** @brief Returns the observational Islamic day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t &date) const override;

        /** @brief Returns whether observational Islamic year @p year is leap. */
        [[nodiscard]] bool is_leap_year(int32_t year) const;
    };
}
