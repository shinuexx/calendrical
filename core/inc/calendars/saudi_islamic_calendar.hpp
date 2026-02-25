#pragma once

#include "islamic_based_calendar.hpp"
#include "location.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    /**
     * @brief Saudi/Umm al-Qura style Islamic calendar implementation.
     */
    class saudi_islamic_calendar :
        public islamic_based_calendar{

        /** @brief Saudi month-start criterion predicate. */
        [[nodiscard]] static bool saudi_criterion(const fixed_date_t& date);

        /** @brief Month start on or before @p date under Saudi criterion. */
        [[nodiscard]] static fixed_date_t new_month_on_or_before(const fixed_date_t& date);

        /** @brief Crescent-based month start helper for @p date. */
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
        /** @brief Reference location used by the Saudi Islamic model. */
        static constexpr location islamic_location{ 30.1, 31.3, 200, moment_t::hms(2) };

        /** @brief Constructs a Saudi Islamic calendar instance. */
        saudi_islamic_calendar() = default;
        ~saudi_islamic_calendar() override = default;

        /** @brief Returns the Saudi Islamic year containing @p date. */
        [[nodiscard]]  int32_t get_year(const fixed_date_t& date) const override;

        /** @brief Converts a fixed date to Saudi Islamic simple-date components. */
        [[nodiscard]]  simple_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts Saudi Islamic simple-date components to fixed-date representation. */
        [[nodiscard]]  fixed_date_t to_fixed(const simple_date& date) const override;

        /** @brief Returns the Saudi Islamic month containing @p date. */
        [[nodiscard]]  int32_t get_month(const fixed_date_t& date) const override;

        /** @brief Returns the Saudi Islamic day-of-month for @p date. */
        [[nodiscard]]  int32_t get_day(const fixed_date_t& date) const override;

        /** @brief Returns whether Saudi Islamic year @p year is leap. */
        [[nodiscard]]  bool is_leap_year(int32_t year) const;
    };
}
