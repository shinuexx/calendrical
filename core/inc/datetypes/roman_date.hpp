#pragma once

#include "month.hpp"
#include "roman_event.hpp"

namespace calendrical::datetypes {
    /**
     * @brief Roman calendar date expressed relative to Kalends/Nones/Ides.
     */
    struct roman_date {
        /** @brief Roman-calendar year. */
        int32_t year;
        /** @brief Month in which the reference event occurs. */
        calendrical::month month;
        /** @brief Roman reference event (Kalends/Nones/Ides). */
        roman_event event;
        /** @brief Inclusive count backward from the reference event. */
        int32_t count;
        /** @brief Leap-day handling flag used by Roman date conversion rules. */
        bool leap;

        /** @brief Default-initialized date. */
        constexpr roman_date() = default;
        /** @brief Constructs from Roman date components. */
        constexpr roman_date(int32_t year, calendrical::month month, roman_event event, int32_t count, bool leap) : year(year), month(month), event(event), count(count), leap(leap) {}

        /** @brief Equality comparison on all components. */
        constexpr bool operator==(const roman_date& other) const = default;
        /** @brief Returns a debug-friendly string representation. */
        [[nodiscard]] std::string to_string() const;

        virtual ~roman_date() = default;
    };

    /**
     * @brief Olympiad cycle/year notation.
     */
    struct olympiad {
        /** @brief Olympiad cycle number. */
        int32_t cycle;
        /** @brief Year within the olympiad (1-4). */
        int32_t year;
        /** @brief Equality comparison on both components. */
        bool operator==(const olympiad & output) const = default;
    };

    /** @brief Returns a debug-friendly string representation of an olympiad value. */
    std::string to_string(const olympiad& odate);

    /** @brief Stream output for @ref olympiad. */
    std::ostream& operator<<(std::ostream& out, const olympiad& odate);
}
