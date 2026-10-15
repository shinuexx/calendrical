#pragma once

#include "datetypes/pawukon_date.hpp"
#include "interval.hpp"
#include "types.hpp"

#include <vector>

namespace calendrical::calendars {
    /** @brief Alias for the date type handled by @ref pawukon_calendar. */
    using pawukon_date = datetypes::pawukon_date;

    /**
     * @brief Balinese Pawukon calendar, a 210-day cycle of ten concurrent weeks (Calendrical Calculations, ch. 12).
     *
     * A Pawukon date repeats every 210 days, so it identifies a day only
     * together with a reference date; see @ref on_or_before and @ref in_range.
     */
    class pawukon_calendar final : public typed_cyclic_calendar_if<pawukon_date> {
        /** @brief CC `bali-epoch`: fixed date of JD 146, the first day of a Pawukon cycle. */
        static constexpr fixed_date_t ms_epoch{-1721279};

    public:
        constexpr pawukon_calendar() = default;
        ~pawukon_calendar() override = default;

        /** @brief Returns RD -1721279 (JD 146), the start of a Pawukon cycle. */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }
        /** @brief Returns 210, the number of days in the cycle. */
        [[nodiscard]] int64_t cycle_length() const override { return pawukon_date::cycle_length; }

        /** @brief Returns the position of @p date in the 210-day cycle, in range [0, 209] (CC `bali-day-from-fixed`). */
        [[nodiscard]] int32_t day_from_fixed(const fixed_date_t& date) const;
        /** @brief Returns the Pawukon date of fixed date @p date (CC `bali-pawukon-from-fixed`). */
        [[nodiscard]] pawukon_date to_date(const fixed_date_t& date) const override;
        /**
         * @brief Returns the last date on or before @p reference whose Pawukon date is @p date (CC `bali-on-or-before`).
         *
         * The result `r` satisfies `r <= reference`, `reference - r < 210` and
         * `to_date(r) == date`.
         *
         * @throws std::invalid_argument if @p date is not a valid Pawukon date.
         */
        [[nodiscard]] fixed_date_t on_or_before(const pawukon_date& date, const fixed_date_t& reference) const override;
        /** @brief Returns whether all components of @p date are in range and consistent (see @ref pawukon_date::is_valid). */
        [[nodiscard]] bool is_valid_date(const pawukon_date& date) const override { return date.is_valid(); }

        /**
         * @brief Returns all dates in @p range whose position in the cycle is @p day modulo @p period, in ascending order.
         *
         * This is CC `positions-in-range` for a subcycle of the Pawukon: the
         * dates `d` in @p range with `day_from_fixed(d) mod period == day mod period`.
         * The boundary policy of @p range is respected.
         *
         * @throws std::invalid_argument if @p period is not a positive divisor of 210.
         */
        [[nodiscard]] std::vector<fixed_date_t> positions_in_range(int64_t day, int64_t period, const interval<fixed_date_t>& range) const;
        /**
         * @brief Returns all dates in @p range whose Pawukon date is @p date, in ascending order.
         *
         * @throws std::invalid_argument if @p date is not a valid Pawukon date.
         */
        [[nodiscard]] std::vector<fixed_date_t> in_range(const pawukon_date& date, const interval<fixed_date_t>& range) const;
    };
}
