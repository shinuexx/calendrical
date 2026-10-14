#pragma once

#include "chinese_base.hpp"
#include "datetypes/chinese_date.hpp"
#include "datetypes/sexagesimal_name.hpp"
#include "types.hpp"

namespace calendrical::calendars {
    /** @brief Alias for the Chinese date type handled by Chinese-family calendars. */
    using chinese_date = datetypes::chinese_date;
    /** @brief Alias for sexagesimal stem/branch names. */
    using sexagesimal_name = datetypes::sexagesimal_name;

    /**
     * @brief Base class for Chinese-family lunisolar calendars.
     */
    class chinese_based_calendar :
        public typed_calendar_if<chinese_date>,
        public chinese_base{
        /** @brief Fixed date of the calendar epoch. */
        fixed_date_t m_epoch;
    public:
        /** @brief Constructs a Chinese-family calendar with a custom epoch. */
        explicit chinese_based_calendar(const fixed_date_t& epoch) : m_epoch{ epoch } {}
        /** @brief Constructs a Chinese-family calendar with default epoch. */
        chinese_based_calendar();

        ~chinese_based_calendar() override = default;

        /** @brief Returns the calendar epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return m_epoch; }

        /** @brief Returns the observation location used for calculations at moment @p t. */
        [[nodiscard]] location get_location(const moment_t& t) const override = 0;

        /** @brief Converts a fixed date to Chinese calendar components. */
        [[nodiscard]] chinese_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts Chinese calendar components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const chinese_date& date) const override;

        /** @brief Returns sexagesimal cycle number for @p date. */
        [[nodiscard]] int32_t get_cycle(const fixed_date_t& date) const;
        /** @brief Returns year number within cycle for @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        /** @brief Returns lunar month number for @p date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        /** @brief Returns whether @p date lies in a leap month. */
        [[nodiscard]] bool is_leap_month(const fixed_date_t& date) const;
        /** @brief Returns day-of-month for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;

        /** @brief Sexagesimal year name from ordinal index @p n. */
        [[nodiscard]] static sexagesimal_name year_name(int32_t n);

        /** @brief Epoch offset used in month-name computations. */
        static constexpr int32_t month_name_epoch = 57;

        /** @brief Sexagesimal month name from month/year numbering. */
        [[nodiscard]] static sexagesimal_name month_name(int32_t month, int32_t year);

        /** @brief Epoch offset used in day-name computations. */
        static constexpr fixed_date_t day_name_epoch{ 45 };

        /** @brief Sexagesimal day name for a fixed date. */
        [[nodiscard]] static sexagesimal_name day_name(const fixed_date_t& date);

        /** @brief Sexagesimal day name at the library day-name epoch. */
        static const sexagesimal_name day_name_zero;

        /** @brief Latest fixed date on or before @p date having the given day name. */
        [[nodiscard]] static fixed_date_t day_name_on_or_before(const sexagesimal_name& name, const fixed_date_t& date);
    };
}
