#pragma once

#include "gregorian_calendar.hpp"
#include "datetypes/bahai_date.hpp"

namespace calendrical::calendars {
    /** @brief Alias for the Baha'i date type handled by Baha'i calendar classes. */
    using bahai_date = datetypes::bahai_date;

    /**
     * @brief Base class for Baha'i calendar implementations.
     */
    class bahai_based_calendar :
    public typed_calendar_if<bahai_date> {
    protected:
        /** @brief Gregorian year containing the Baha'i epoch. */
        static constexpr int32_t ms_gregorian_year_bahai_epoch = 1844;

        /** @brief Baha'i epoch in fixed-date coordinates. */
        static constexpr fixed_date_t ms_epoch{ 673222 };

        /** @brief Gregorian backend used for date partitioning and conversions. */
        const gregorian_calendar *m_gregorian;

        /** @brief Returns elapsed Baha'i years from epoch for a fixed date. */
        [[nodiscard]] virtual int32_t get_years(const fixed_date_t& date) const = 0;
        /** @brief Computes major cycle number from elapsed years. */
        [[nodiscard]] virtual int32_t get_major(int32_t years) const;
        /** @brief Computes cycle number within a major cycle from elapsed years. */
        [[nodiscard]] virtual int32_t get_cycle(int32_t years) const;
        /** @brief Computes year number within a cycle from elapsed years. */
        [[nodiscard]] virtual int32_t get_year(int32_t years) const;
        /** @brief Computes Baha'i month for a fixed date and resolved Y/C/M context. */
        [[nodiscard]] virtual int32_t get_month(const fixed_date_t& date, int32_t major, int32_t cycle, int32_t year) const;
        /** @brief Days elapsed since Baha'i New Year for a fixed date. */
        [[nodiscard]] virtual int32_t get_days(const fixed_date_t& date, const fixed_date_t& new_year) const;
        /** @brief Computes day-of-month for a fixed date and resolved Baha'i components. */
        [[nodiscard]] virtual int32_t get_day(const fixed_date_t& date, int32_t major, int32_t cycle, int32_t year, int32_t month) const;

    public:
        /** @brief Sentinel month code representing Ayyam-i-Ha intercalary days. */
        static constexpr int32_t ayyam_i_ha{ 0 };

        /** @brief Constructs a Baha'i-based calendar with Gregorian backend. */
        bahai_based_calendar();
        ~bahai_based_calendar() override = default;

        /** @brief Returns Baha'i major cycle for a fixed date. */
        [[nodiscard]] virtual int32_t get_major(const fixed_date_t& date) const;
        /** @brief Returns Baha'i cycle number for a fixed date. */
        [[nodiscard]] virtual int32_t get_cycle(const fixed_date_t& date) const;
        /** @brief Returns Baha'i year number for a fixed date. */
        [[nodiscard]] virtual int32_t get_year(const fixed_date_t& date) const;
        /** @brief Returns Baha'i month code for a fixed date. */
        [[nodiscard]] virtual int32_t get_month(const fixed_date_t& date) const;
        /** @brief Returns Baha'i day-of-month for a fixed date. */
        [[nodiscard]] virtual int32_t get_day(const fixed_date_t& date) const;

        /** @brief Returns the Baha'i epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        /** @brief Converts a fixed date to Baha'i date components. */
        [[nodiscard]] bahai_date to_date(const fixed_date_t& date) const override = 0;
        /** @brief Converts Baha'i date components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const bahai_date& date) const override = 0;
    };
}
