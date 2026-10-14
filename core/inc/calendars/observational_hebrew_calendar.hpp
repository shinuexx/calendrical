#pragma once

#include "hebrew_based_calendar.hpp"
#include "gregorian_calendar.hpp"
#include "location.hpp"
#include "hebrew_calendar.hpp"

namespace calendrical::calendars{

    /**
     * @brief Observational Hebrew calendar based on visible crescent calculations.
     */
    class observational_hebrew_calendar: public hebrew_based_calendar{
        /** @brief Gregorian backend used for year anchoring. */
        const gregorian_calendar* m_gregorian;
        /** @brief Arithmetic Hebrew helper used for some intermediate calculations. */
        std::unique_ptr<const hebrew_calendar> m_hebrew;

        /** @brief Approximate first day of Nisan in Gregorian year @p gyear. */
        [[nodiscard]] static fixed_date_t first_of_nisan(int32_t gyear);

        /** @brief Finds the crescent-based month start near @p date. */
        [[nodiscard]] fixed_date_t get_crescent(const fixed_date_t& date) const;

    public:
        /** @brief Reference location used for observational Hebrew calculations. */
        static constexpr location hebrew_location{ 32.82_deg, 35_deg, 0, moment_t::hms(2) };

        /** @brief Constructs an observational Hebrew calendar instance. */
        observational_hebrew_calendar();
        ~observational_hebrew_calendar() override = default;

        /** @brief Returns observational Hebrew New Year for Hebrew year @p year. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const override;
        /** @brief Returns the observational Hebrew year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const override;

        /** @brief Converts a fixed date to observational Hebrew Y-M-D components. */
        [[nodiscard]] hebrew_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts observational Hebrew Y-M-D components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const hebrew_date& date) const override;

        /** @brief Returns the observational Hebrew month containing @p date. */
        [[nodiscard]] hebrew_month get_month(const fixed_date_t& date) const override;

        /** @brief Returns the observational Hebrew day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const override;

        /** @brief Returns whether observational Hebrew year @p year is leap. */
        [[nodiscard]] bool is_leap_year(int32_t year) const override;
        /**
         * @brief Validates a date against observed month starts.
         *
         * Month lengths depend on crescent visibility, so after basic range checks the date
         * must survive a round-trip through fixed-date representation.
         */
        [[nodiscard]] bool is_valid_date(const hebrew_date& date) const override;

        /** @brief Classical Passover eve date corresponding to Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t classical_passover_eve(int32_t gyear) const;
    };
}
