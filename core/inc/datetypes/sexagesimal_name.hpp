#pragma once

#include <cstdint>
#include <string>

namespace calendrical::datetypes {
    /**
     * @brief Chinese sexagesimal name represented by stem and branch indices.
     */
    struct sexagesimal_name {
        /** @brief Heavenly stem index. */
        int32_t stem;
        /** @brief Earthly branch index. */
        int32_t branch;

        /** @brief Default-initialized name. */
        constexpr sexagesimal_name() = default;
        /** @brief Constructs from stem/branch indices. */
        constexpr sexagesimal_name(int32_t stem, int32_t branch) : stem(stem), branch(branch) {}

        /**
         * @brief Number of days forward from this name to @p other, in the range 1..60.
         *
         * `a.difference(b)` counts days from `a` to `b`; equal names yield 60.
         */
        [[nodiscard]] int32_t difference(const sexagesimal_name &other) const;
        /** @brief Creates the sexagesimal name corresponding to sequence index @p n. */
        [[nodiscard]] static sexagesimal_name from(int64_t n);
        /** @brief Returns a debug-friendly string representation. */
        [[nodiscard]] std::string to_string() const;
        /** @brief Equality comparison on stem and branch. */
        constexpr bool operator==(const sexagesimal_name &other) const = default;
    };

    /** @brief Stream output for @ref sexagesimal_name. */
    std::ostream& operator<<(std::ostream& out, const sexagesimal_name& odate);
}
