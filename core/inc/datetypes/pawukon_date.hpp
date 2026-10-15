#pragma once

#include "helpers.hpp"
#include <algorithm>
#include <array>
#include <compare>
#include <cstdint>
#include <iosfwd>
#include <string>

namespace calendrical::datetypes {
    /**
     * @brief Balinese Pawukon date: positions of a day in ten concurrent weeks.
     *
     * The Pawukon cycle lasts 210 days and runs ten weeks of 1 to 10 days in
     * parallel (Calendrical Calculations, `balinese-date`). Every component is
     * stored, as in CC; they are all determined by the position in the 210-day
     * cycle (@ref day), so an arbitrary combination is not necessarily a valid
     * date. Use @ref is_valid to check, and @ref from_day to build a date from
     * its position.
     *
     * Component ranges follow CC: luang is a boolean, dwiwara 1-2, triwara 1-3,
     * caturwara 1-4, pancawara 1-5, sadwara 1-6, saptawara 1-7, asatawara 1-8,
     * sangawara 1-9 and dasawara 0-9.
     */
    struct pawukon_date {
        /** @brief Length of the Pawukon cycle in days. */
        static constexpr int32_t cycle_length = 210;

        /** @brief 1-day week: whether the day is "in" the luang week (even dasawara). */
        bool luang;
        /** @brief 2-day week (dwiwara), in range [1, 2]. */
        int32_t dwiwara;
        /** @brief 3-day week (triwara), in range [1, 3]. */
        int32_t triwara;
        /** @brief 4-day week (caturwara), in range [1, 4]. */
        int32_t caturwara;
        /** @brief 5-day week (pancawara), in range [1, 5]; 5 is Keliwon. */
        int32_t pancawara;
        /** @brief 6-day week (sadwara), in range [1, 6]. */
        int32_t sadwara;
        /** @brief 7-day week (saptawara), in range [1, 7]; 1 is Sunday, 7 is Saturday. */
        int32_t saptawara;
        /** @brief 8-day week (asatawara), in range [1, 8]. */
        int32_t asatawara;
        /** @brief 9-day week (sangawara), in range [1, 9]. */
        int32_t sangawara;
        /** @brief 10-day week (dasawara), in range [0, 9]. */
        int32_t dasawara;

        /** @brief Constructs the date at position 0 of the cycle. */
        constexpr pawukon_date() : pawukon_date(from_day(0)) {}
        /** @brief Constructs from all ten components; use @ref is_valid to check consistency. */
        constexpr pawukon_date(bool luang, int32_t dwiwara, int32_t triwara, int32_t caturwara, int32_t pancawara,
                               int32_t sadwara, int32_t saptawara, int32_t asatawara, int32_t sangawara, int32_t dasawara)
            : luang(luang), dwiwara(dwiwara), triwara(triwara), caturwara(caturwara), pancawara(pancawara),
              sadwara(sadwara), saptawara(saptawara), asatawara(asatawara), sangawara(sangawara), dasawara(dasawara) {}

        /**
         * @brief Returns the date at position @p n of the cycle.
         *
         * @p n is reduced modulo 210, so any integer is accepted; position 0 is
         * the first day of the cycle (CC `bali-pawukon-from-fixed` applied to
         * `bali-epoch + n`).
         */
        [[nodiscard]] static constexpr pawukon_date from_day(int64_t n) {
            const auto d = mod(n, cycle_length);
            const auto pancawara = adjusted_remainder(d + 2, 5);
            const auto saptawara = mod(d, 7) + 1;
            constexpr std::array<int64_t, 5> pancawara_urip{ 5, 9, 7, 4, 8 };
            constexpr std::array<int64_t, 7> saptawara_urip{ 5, 4, 3, 7, 8, 6, 9 };
            const auto dasawara = mod(1 + pancawara_urip[static_cast<size_t>(pancawara - 1)]
                                        + saptawara_urip[static_cast<size_t>(saptawara - 1)], 10);
            // The 8- and 9-day weeks do not divide 210; CC repeats a day to fit them into the cycle.
            const auto asatawara = mod(std::max<int64_t>(6, 4 + mod(d - 70, cycle_length)), 8) + 1;
            const auto sangawara = mod(std::max<int64_t>(0, d - 3), 9) + 1;
            return {
                dasawara % 2 == 0,
                static_cast<int32_t>(adjusted_remainder(dasawara, 2)),
                static_cast<int32_t>(mod(d, 3) + 1),
                static_cast<int32_t>(adjusted_remainder(asatawara, 4)),
                static_cast<int32_t>(pancawara),
                static_cast<int32_t>(mod(d, 6) + 1),
                static_cast<int32_t>(saptawara),
                static_cast<int32_t>(asatawara),
                static_cast<int32_t>(sangawara),
                static_cast<int32_t>(dasawara),
            };
        }

        /**
         * @brief Returns the position of this date in the 210-day cycle, in range [0, 209].
         *
         * The position is recovered from the pancawara, sadwara and saptawara
         * components alone (as in CC `bali-on-or-before`); for an invalid date the
         * result names the position those three components point to.
         */
        [[nodiscard]] constexpr int32_t day() const {
            const auto a5 = static_cast<int64_t>(pancawara) - 1;
            const auto a6 = static_cast<int64_t>(sadwara) - 1;
            const auto b7 = static_cast<int64_t>(saptawara) - 1;
            // Chinese remainder theorem: 35-day position from the 5- and 7-day weeks, then 210 from 35 and 6.
            const auto b35 = mod(a5 + 14 + 15 * (b7 - a5), 35);
            return static_cast<int32_t>(mod(a6 + 36 * (b35 - a6), cycle_length));
        }

        /** @brief Returns the Pawukon week number, in range [1, 30] (CC `bali-week-from-fixed`). */
        [[nodiscard]] constexpr int32_t week() const { return day() / 7 + 1; }

        /**
         * @brief Returns whether all components are in range and consistent with one position of the cycle.
         *
         * Only 210 of the possible combinations of the ten components occur.
         */
        [[nodiscard]] constexpr bool is_valid() const {
            return dwiwara >= 1 && dwiwara <= 2 && triwara >= 1 && triwara <= 3 && caturwara >= 1 && caturwara <= 4
                && pancawara >= 1 && pancawara <= 5 && sadwara >= 1 && sadwara <= 6 && saptawara >= 1 && saptawara <= 7
                && asatawara >= 1 && asatawara <= 8 && sangawara >= 1 && sangawara <= 9 && dasawara >= 0 && dasawara <= 9
                && from_day(day()) == *this;
        }

        /** @brief Returns a debug-friendly string representation. */
        [[nodiscard]] std::string to_string() const;
        /** @brief Equality comparison on all ten components. */
        constexpr bool operator==(const pawukon_date& other) const = default;
        /** @brief Lexicographic ordering by component, luang first; allows use as an ordered container key. */
        constexpr auto operator<=>(const pawukon_date& other) const = default;
    };

    /** @brief Stream output for @ref pawukon_date. */
    std::ostream& operator<<(std::ostream& out, const pawukon_date& date);
}
