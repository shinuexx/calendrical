#pragma once

namespace calendrical::datetypes {
    /**
     * @brief Baha'i calendar date (major-cycle-year-month-day).
     */
    struct bahai_date {
        /** @brief Kull-i-Shay' (major cycle). */
        int32_t major;
        /** @brief Vahid (cycle) within the major cycle. */
        int32_t cycle;
        /** @brief Year within the cycle. */
        int32_t year;
        /** @brief Month number. */
        int32_t month;
        /** @brief Day of month. */
        int32_t day;

        /** @brief Default-initialized date. */
        constexpr bahai_date() = default;
        /** @brief Constructs from Baha'i date components. */
        constexpr bahai_date(int32_t major, int32_t cycle, int32_t year, int32_t month, int32_t day): major(major), cycle(cycle), year(year), month(month), day(day) {}

        /** @brief Equality comparison on all components. */
        [[nodiscard]] constexpr bool operator==(const bahai_date &other) const = default;

        /** @brief Returns a debug-friendly string representation. */
        [[nodiscard]] std::string to_string() const;

        virtual ~bahai_date() = default;
    };
}
