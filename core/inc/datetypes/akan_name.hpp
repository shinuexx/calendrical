#pragma once

#include "helpers.hpp"
#include <compare>
#include <cstdint>
#include <iosfwd>
#include <string>

namespace calendrical::datetypes {
    /**
     * @brief Akan day name: a position in the 42-day Akan cycle.
     *
     * The name combines a prefix from a 6-day cycle with a stem from a 7-day
     * cycle (Calendrical Calculations, `akan-day-name`). Because 6 and 7 are
     * coprime, each of the 42 valid (prefix, stem) pairs occurs exactly once
     * per cycle. The stem cycle is aligned with the week: stem 1 (Wukuo) is
     * always a Wednesday and stem 7 (Bene) a Tuesday.
     */
    struct akan_name {
        /** @brief Number of prefixes (length of the prefix cycle). */
        static constexpr int32_t prefix_count = 6;
        /** @brief Number of stems (length of the stem cycle). */
        static constexpr int32_t stem_count = 7;
        /** @brief Length of the combined cycle in days. */
        static constexpr int32_t cycle_length = prefix_count * stem_count;

        /** @brief Prefix index in range [1, 6] (Nwona, Nkyi, Kuru, Kwa, Mono, Fo). */
        int32_t prefix;
        /** @brief Stem index in range [1, 7] (Wukuo, Yaw, Fie, Memene, Kwasi, Dwo, Bene). */
        int32_t stem;

        /** @brief Constructs the name {1, 1}, the first position of the cycle. */
        constexpr akan_name() : prefix{1}, stem{1} {}
        /** @brief Constructs from prefix and stem indices; use @ref is_valid to check the domain. */
        constexpr akan_name(int32_t prefix, int32_t stem) : prefix(prefix), stem(stem) {}

        /** @brief Returns whether the prefix is in [1, 6] and the stem in [1, 7]. */
        [[nodiscard]] constexpr bool is_valid() const {
            return prefix >= 1 && prefix <= prefix_count && stem >= 1 && stem <= stem_count;
        }

        /**
         * @brief Returns how many days after this name @p other occurs.
         *
         * The result is in range [1, 42]; 42 means both names are equal.
         * Both names must be valid.
         */
        [[nodiscard]] int32_t difference(const akan_name &other) const;
        /** @brief Returns the name at position @p n of the cycle; position 0 is {6, 7}. */
        [[nodiscard]] static constexpr akan_name from(int64_t n) {
            return {
                static_cast<int32_t>(adjusted_remainder(n, prefix_count)),
                static_cast<int32_t>(adjusted_remainder(n, stem_count)),
            };
        }
        /** @brief Returns a debug-friendly string representation. */
        [[nodiscard]] std::string to_string() const;
        /** @brief Equality comparison on prefix and stem. */
        constexpr bool operator==(const akan_name &other) const = default;
        /** @brief Lexicographic ordering by prefix, then stem; allows use as an ordered container key. */
        constexpr auto operator<=>(const akan_name &other) const = default;
    };

    /** @brief Stream output for @ref akan_name. */
    std::ostream& operator<<(std::ostream& out, const akan_name& name);
}
