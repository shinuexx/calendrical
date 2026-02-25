#pragma once

namespace calendrical::datetypes {
    /**
     * @brief Chinese lunisolar calendar date.
     */
    struct chinese_date{
        /** @brief Sexagesimal cycle number. */
        int32_t cycle;
        /** @brief Year number within the cycle. */
        int32_t year;
        /** @brief Month number. */
        int32_t month;
        /** @brief True if the month is an intercalary (leap) month. */
        bool leap_month;
        /** @brief Day of month. */
        int32_t day;

        /** @brief Default-initialized date. */
        constexpr chinese_date() = default;
        /** @brief Constructs from Chinese date components. */
        constexpr chinese_date(int32_t cycle, int32_t year, int32_t month, bool leap_month, int32_t day)
            : cycle(cycle), year(year), month(month), leap_month(leap_month), day(day) {}

        /** @brief Equality comparison on all components. */
        constexpr bool operator==(const chinese_date &other) const = default;

        /** @brief Returns a debug-friendly string representation. */
        [[nodiscard]] std::string to_string() const;

        virtual ~chinese_date() = default;
    };
}
