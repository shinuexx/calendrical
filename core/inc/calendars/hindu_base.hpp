#pragma once

#include "moment.hpp"

namespace calendrical::calendars{
    /**
     * @brief Shared constants and helpers for Hindu calendar families.
     */
    class hindu_base{
    protected:
        /** @brief Hindu epoch in fixed-date coordinates. */
        static constexpr fixed_date_t ms_epoch{ -1132959 };

        /** @brief Arya solar year length (days). */
        static constexpr double arya_solar_year{1577917500.0 / 4320000};
        /** @brief Arya Jovian cycle period (days). */
        static constexpr double arya_jovian_period{1577917500.0 / 364224};

        /** @brief Arya solar month length (days). */
        static constexpr double arya_solar_month{arya_solar_year / 12};
        /** @brief Arya lunar month length (days). */
        static constexpr double arya_lunar_month{1577917500.0 / 53433336};
        /** @brief Arya lunar day (tithi) length (days). */
        static constexpr double arya_lunar_day{arya_lunar_month / 30};

        /** @brief Threshold used in old Hindu leap-year logic. */
        static constexpr double lunar_leap_year_indicator{23902504679.0 / 1282400064.0};

        /** @brief Difference between Arya solar and lunar month lengths. */
        static constexpr double arya_solar_lunar_month_difference{arya_solar_month - arya_lunar_month};

    public:
        /** @brief Constructs a Hindu calendar base object. */
        hindu_base() = default;
        virtual ~hindu_base() = default;

        /** @brief Day count since the Hindu epoch. */
        [[nodiscard]] virtual int64_t day_count(const fixed_date_t& date) const;
        /** @brief Jovian year designation for a fixed date. */
        [[nodiscard]] virtual int32_t jovian_year(const fixed_date_t& date) const;
    };
}
