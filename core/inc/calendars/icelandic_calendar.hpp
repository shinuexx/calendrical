#pragma once

#include "datetypes/icelandic_date.hpp"

namespace calendrical::calendars{
    /** @brief Alias for the date type handled by @ref icelandic_calendar. */
    using icelandic_date = datetypes::icelandic_date;

    /**
     * @brief Icelandic calendar conversion and helper API.
     */
    class icelandic_calendar final:
        public typed_calendar_if<icelandic_date>{

        fixed_date_t m_epoch;

        /** @brief Coefficients used by the leap-year rule/epoch arithmetic. */
        static constexpr std::array<int64_t, 3> a{97,24,1};

        /** @brief Internal season extractor using a resolved year. */
        [[nodiscard]] season get_season(const fixed_date_t& date, int32_t year) const;

        /** @brief Internal week extractor using resolved year/season. */
        [[nodiscard]] int32_t get_week(const fixed_date_t& date, int32_t year, season s) const;
        /** @brief Internal month-index helper from Icelandic date + fixed date. */
        [[nodiscard]] int32_t get_month(const icelandic_date& odate, const fixed_date_t& date) const;

    public:
        /** @brief Constructs an Icelandic calendar instance. */
        icelandic_calendar();

        ~icelandic_calendar() override = default;

        /** @brief Start date of summer for Icelandic year @p iyear. */
        [[nodiscard]] fixed_date_t summer(int32_t iyear) const;
        /** @brief Start date of winter for Icelandic year @p iyear. */
        [[nodiscard]] fixed_date_t winter(int32_t iyear) const;

        /** @brief Returns the Icelandic New Year anchor for year @p year. */
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;
        /** @brief Returns the Icelandic year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;

        /** @brief Returns the Icelandic calendar epoch. */
        [[nodiscard]] fixed_date_t epoch() const override { return m_epoch; }

        /** @brief Converts a fixed date to Icelandic calendar components. */
        [[nodiscard]] icelandic_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts Icelandic calendar components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const icelandic_date& date) const override;

        /** @brief Returns month index for a fixed date. */
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        /** @brief Returns month index for an Icelandic date object. */
        [[nodiscard]] int32_t get_month(const icelandic_date& date) const;

        /** @brief Returns whether Icelandic year @p year is leap. */
        [[nodiscard]] bool is_leap_year(int32_t year) const;

        /** @brief Returns the season containing @p date. */
        [[nodiscard]] season get_season(const fixed_date_t& date) const;
        /** @brief Returns the week number for @p date. */
        [[nodiscard]] int32_t get_week(const fixed_date_t& date) const;
    };
}
