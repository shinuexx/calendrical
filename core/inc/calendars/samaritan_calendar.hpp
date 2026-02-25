#pragma once

#include "datetypes/simple_date.hpp"
#include "julian_calendar.hpp"
#include "holiday_providers/julian_calendar_holiday_provider.hpp"
#include "location.hpp"

namespace calendrical::calendars{
    /** @brief Alias for the simple date type used by @ref samaritan_calendar. */
    using simple_date = datetypes::simple_date;
    /**
     * @brief Samaritan calendar implementation with lunar observation logic.
     */
    class samaritan_calendar : public typed_calendar_if<simple_date>{\
        /** @brief Local noon approximation used by Samaritan month calculations. */
        [[nodiscard]] static moment_t noon(const fixed_date_t& date);
        /** @brief First observed new moon after moment @p t. */
        [[nodiscard]] static fixed_date_t new_moon_after(const moment_t& t);
        /** @brief Observed new moon on or before moment @p t. */
        [[nodiscard]] static fixed_date_t new_moon_at_or_before(const moment_t& t);

        /** @brief Finds Samaritan New Year on or before a fixed date. */
        [[nodiscard]] fixed_date_t new_year_on_or_before(const fixed_date_t& date) const;

        /** @brief Julian backend used by Samaritan calculations. */
        std::unique_ptr<const julian_calendar> m_julian;
        /** @brief Julian holiday provider helper used by Samaritan rules. */
        holiday_providers::julian_calendar_holiday_provider m_julian_provider;

    public:
        /** @brief Reference location used by the Samaritan calendar. */
        static constexpr location samaritan_location{ 32.1994, 35.2728, 881, moment_t::hms(2) };
        /** @brief Runtime epoch derived from the implemented Samaritan rules. */
        fixed_date_t m_epoch;

        /** @brief Constructs a Samaritan calendar instance. */
        samaritan_calendar();
        ~samaritan_calendar() override = default;

        /** @brief Returns the Samaritan year containing @p date. */
        [[nodiscard]]  int32_t get_year(const fixed_date_t& date) const;

        /** @brief Returns the Samaritan epoch. */
        [[nodiscard]]  fixed_date_t epoch() const override { return m_epoch; }

        /** @brief Converts a fixed date to Samaritan simple-date components. */
        [[nodiscard]]  simple_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts Samaritan simple-date components to fixed-date representation. */
        [[nodiscard]]  fixed_date_t to_fixed(const simple_date& date) const override;

        /** @brief Returns the Samaritan month containing @p date. */
        [[nodiscard]]  int32_t get_month(const fixed_date_t& date) const;
        /** @brief Returns the Samaritan day-of-month for @p date. */
        [[nodiscard]]  int32_t get_day(const fixed_date_t& date) const;
    };
}
