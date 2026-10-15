#pragma once

#include "datetypes/lunar_date.hpp"
#include "angle.hpp"

namespace calendrical::calendars {
    /** @brief Alias for the date type handled by @ref tibetan_calendar. */
    using lunar_date = datetypes::lunar_date;

    /**
     * @brief Tibetan lunisolar calendar implementation.
     */
    class tibetan_calendar :
        public typed_calendar_if<lunar_date>
    {
        /** @brief Tibetan epoch in fixed-date coordinates. */
        static const fixed_date_t ms_epoch;

        /** @brief Solar equation correction used by the Tibetan calendar model (in days). */
        [[nodiscard]] static double tibetan_sun_equation(double a);
        /** @brief Lunar equation correction used by the Tibetan calendar model (in days). */
        [[nodiscard]] static double tibetan_moon_equation(double a);

    public:

        /** @brief Constructs a Tibetan calendar instance. */
        tibetan_calendar() = default;
        ~tibetan_calendar() override = default;

        /** @brief Returns the Tibetan epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        /** @brief Converts a fixed date to Tibetan lunar date components. */
        [[nodiscard]] lunar_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts Tibetan lunar date components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const lunar_date& date) const override;

        /** @brief Returns the Tibetan month containing @p date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        /** @brief Returns whether @p date lies in a Tibetan leap month. */
        [[nodiscard]] bool is_leap_month(const fixed_date_t& date) const;
        /** @brief Returns the Tibetan day number for @p date. */
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
        /** @brief Returns whether @p date is a Tibetan leap day. */
        [[nodiscard]] bool is_leap_day(const fixed_date_t& date) const;
        /** @brief Returns the Tibetan year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;

        /** @brief Returns whether a given Tibetan year/month pair is leap. */
        [[nodiscard]] bool is_leap_month(int32_t year, int32_t month) const;
        /** @brief Returns whether a given Tibetan year/month/day is leap. */
        [[nodiscard]] bool is_leap_day(int32_t year, int32_t month, int32_t day) const;
 
        /** @brief Losar (Tibetan New Year) for Tibetan year @p tyear. */
        [[nodiscard]] fixed_date_t losar(int32_t tyear) const;

        /** @brief Tibetan New Year dates that fall in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> new_year(int32_t gyear) const;
    };
}
