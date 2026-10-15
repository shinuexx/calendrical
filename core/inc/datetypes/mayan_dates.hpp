#pragma once

#include <cstdint>
#include <iosfwd>
#include <limits>
#include <string>

namespace calendrical::datetypes {
    /**
     * @brief Canonical nine-field proleptic Maya Long Count, from alautun to kin.
     *
     * Only alautun is signed. All lower fields are in [0, 19], except uinal in
     * [0, 17]. There is no wrap at 13 baktuns. Negative counts borrow from alautun.
     * The numeric domain consists of complete alautun blocks with int64_t headroom.
     */
    struct mayan_long_count {
        /** @brief Number of days in one alautun. */
        static constexpr int64_t days_per_alautun = 23040000000LL;
        /** @brief Largest permitted absolute alautun, leaving one block of int64_t headroom. */
        static constexpr int32_t max_alautun = static_cast<int32_t>(
            std::numeric_limits<int64_t>::max() / days_per_alautun - 1);

        int32_t alautun{};    ///< Signed most significant field, in [-max_alautun, max_alautun].
        int32_t kinchiltun{}; ///< 0..19; one kinchiltun is 1,152,000,000 days.
        int32_t cakabtun{};   ///< 0..19; one cakabtun is 57,600,000 days.
        int32_t pictun{};     ///< 0..19; one pictun is 2,880,000 days.
        int32_t baktun{};     ///< 0..19; one baktun is 144,000 days.
        int32_t katun{};      ///< 0..19; one katun is 7,200 days.
        int32_t tun{};        ///< 0..19; one tun is 360 days.
        int32_t uinal{};      ///< 0..17; one uinal is 20 days.
        int32_t kin{};        ///< 0..19; one kin is one day.

        /** @brief Checks canonical field ranges and the supported numeric domain. */
        [[nodiscard]] constexpr bool is_valid() const {
            return alautun >= -max_alautun && alautun <= max_alautun
                && kinchiltun >= 0 && kinchiltun < 20
                && cakabtun >= 0 && cakabtun < 20
                && pictun >= 0 && pictun < 20
                && baktun >= 0 && baktun < 20
                && katun >= 0 && katun < 20
                && tun >= 0 && tun < 20
                && uinal >= 0 && uinal < 18
                && kin >= 0 && kin < 20;
        }

        /** @brief Compares all nine components. */
        constexpr bool operator==(const mayan_long_count&) const = default;
        /** @brief Returns the structured form mayan_long_count{alautun:..., ...,kin:...}. */
        [[nodiscard]] std::string to_string() const;
    };

    /** @brief Writes the structured Long Count representation (ADL). */
    std::ostream& operator<<(std::ostream& out, const mayan_long_count& date);
}
