#pragma once

#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {

    /**
     * @brief Holiday provider for United States public holidays and daylight saving time events.
     *
     * Implements calculations from Calendrical Calculations (CC 2.32–2.40):
     * - Independence Day (CC 2.32)
     * - Labor Day (CC 2.36)
     * - Memorial Day (CC 2.37)
     * - Election Day (CC 2.38)
     * - Daylight Saving Start (CC 2.39)
     * - Daylight Saving End (CC 2.40)
     */
    class us_holiday_provider final : public holiday_provider_if {
        /** @brief Gregorian calendar used for date calculations. */
        const calendars::gregorian_calendar* m_gregorian;

    public:
        /**
         * @brief Constructs a provider bound to a specific Gregorian calendar instance.
         * @param gregorian Gregorian calendar instance (not owned, must outlive provider).
         */
        explicit us_holiday_provider(const calendars::gregorian_calendar* gregorian) : m_gregorian(gregorian) {}

        /**
         * @brief Constructs a provider using the standard Gregorian calendar singleton.
         */
        us_holiday_provider() : m_gregorian(std::addressof(calendars::gregorian_calendar::standard)) {}

        ~us_holiday_provider() override = default;

        /** @brief Returns the backing calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override { return m_gregorian; }

        /**
         * @brief Returns the provider's holiday set for Gregorian year @p gyear.
         * @param gyear Gregorian year.
         * @return Chronologically sorted vector of holidays.
         */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;

        /**
         * @brief Independence Day in Gregorian year @p year (CC 2.32). July 4.
         * @param year Gregorian year.
         * @return Fixed date of Independence Day.
         */
        [[nodiscard]] fixed_date_t independence_day(int32_t year) const;

        /**
         * @brief Labor Day in Gregorian year @p year (CC 2.36). First Monday in September.
         * @param year Gregorian year.
         * @return Fixed date of Labor Day.
         */
        [[nodiscard]] fixed_date_t labor_day(int32_t year) const;

        /**
         * @brief Memorial Day in Gregorian year @p year (CC 2.37). Last Monday in May.
         * @param year Gregorian year.
         * @return Fixed date of Memorial Day.
         */
        [[nodiscard]] fixed_date_t memorial_day(int32_t year) const;

        /**
         * @brief Election Day in Gregorian year @p year (CC 2.38).
         *
         * First Tuesday on or after November 2 (Tuesday following the first Monday in November).
         *
         * @param year Gregorian year.
         * @return Fixed date of Election Day.
         */
        [[nodiscard]] fixed_date_t election_day(int32_t year) const;

        /**
         * @brief Daylight Saving Time start in Gregorian year @p year (CC 2.39). Second Sunday in March.
         *
         * @note Follows the uniform rule from CC (enacted in the US in 2007) applied across all years,
         *       rather than historical DST transition rules.
         * @param year Gregorian year.
         * @return Fixed date of Daylight Saving Time start.
         */
        [[nodiscard]] fixed_date_t daylight_saving_start(int32_t year) const;

        /**
         * @brief Daylight Saving Time end in Gregorian year @p year (CC 2.40). First Sunday in November.
         *
         * @note Follows the uniform rule from CC (enacted in the US in 2007) applied across all years,
         *       rather than historical DST transition rules.
         * @param year Gregorian year.
         * @return Fixed date of Daylight Saving Time end.
         */
        [[nodiscard]] fixed_date_t daylight_saving_end(int32_t year) const;
    };
}
