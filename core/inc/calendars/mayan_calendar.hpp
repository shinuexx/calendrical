#pragma once

#include "datetypes/mayan_dates.hpp"
#include "types.hpp"

namespace calendrical::calendars {
    /** @brief Date type handled by the Long Count calendar. */
    using mayan_long_count = datetypes::mayan_long_count;

    /**
     * @brief Proleptic Maya Long Count using the GMT correlation JDN 584283.
     *
     * The epoch is floor(JD 584283 - 1721424.5) = RD -1137142, giving
     * 13.0.0.0.0 on Gregorian 2012-12-21. No Haab, Tzolkin or Calendar Round.
     * Only the explicitly bounded complete alautun blocks are supported.
     */
    class mayan_calendar final : public typed_calendar_if<mayan_long_count> {
    public:
        /** @brief First supported fixed date, inclusive. */
        static constexpr fixed_date_t min_date{
            -1137142LL - static_cast<int64_t>(mayan_long_count::max_alautun)
                * mayan_long_count::days_per_alautun};
        /** @brief Last supported fixed date, inclusive. */
        static constexpr fixed_date_t max_date{
            -1137142LL + static_cast<int64_t>(mayan_long_count::max_alautun)
                * mayan_long_count::days_per_alautun
                + (mayan_long_count::days_per_alautun - 1)};

        mayan_calendar() = default;
        ~mayan_calendar() override = default;

        /** @brief Returns RD -1137142 (GMT correlation JDN 584283). */
        [[nodiscard]] fixed_date_t epoch() const override { return fixed_date_t{-1137142}; }
        /** @brief Checks canonical ranges; does not normalize noncanonical fields. */
        [[nodiscard]] bool is_valid_date(const mayan_long_count& date) const override {
            return date.is_valid();
        }
        /**
         * @brief Converts canonical Long Count components to a fixed date using integer arithmetic.
         * @throws std::invalid_argument if any component is outside its supported range.
         */
        [[nodiscard]] fixed_date_t to_fixed(const mayan_long_count& date) const override;
        /**
         * @brief Converts a fixed date to canonical components, using floor/mod before the epoch.
         * @throws std::out_of_range if the date is outside [min_date, max_date].
         */
        [[nodiscard]] mayan_long_count to_date(const fixed_date_t& date) const override;
    };
}
