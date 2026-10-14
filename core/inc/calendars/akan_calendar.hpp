#pragma once

#include "datetypes/akan_name.hpp"
#include "types.hpp"

namespace calendrical::calendars {
    /** @brief Alias for the date type handled by @ref akan_calendar. */
    using akan_name = datetypes::akan_name;

    /**
     * @brief Akan 42-day cycle of day names (Calendrical Calculations, `akan-day-name`).
     *
     * A name repeats every 42 days, so it identifies a day only together with a
     * reference date; see @ref on_or_before.
     */
    class akan_calendar final : public typed_cyclic_calendar_if<akan_name> {
        /** @brief Fixed date whose name is position 0 of the cycle, {6, 7}. */
        static constexpr fixed_date_t ms_epoch{37};
        /** @brief Name of RD 0, the date from which @ref on_or_before counts. */
        static constexpr akan_name ms_name_of_rd_zero = akan_name::from(-static_cast<int64_t>(ms_epoch));

    public:
        constexpr akan_calendar() = default;
        ~akan_calendar() override = default;

        /** @brief Returns RD 37, the fixed date named {6, 7} (Fo-Bene). */
        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }
        /** @brief Returns 42, the number of days in the cycle. */
        [[nodiscard]] int64_t cycle_length() const override { return akan_name::cycle_length; }

        /** @brief Returns the Akan name of fixed date @p date. */
        [[nodiscard]] akan_name to_date(const fixed_date_t& date) const override;
        /**
         * @brief Returns the last date on or before @p reference whose name is @p name.
         *
         * The result `r` satisfies `r <= reference`, `reference - r < 42` and
         * `to_date(r) == name`.
         *
         * @throws std::invalid_argument if @p name is not a valid Akan name.
         */
        [[nodiscard]] fixed_date_t on_or_before(const akan_name& name, const fixed_date_t& reference) const override;
        /** @brief Returns whether @p name has a prefix in [1, 6] and a stem in [1, 7]. */
        [[nodiscard]] bool is_valid_date(const akan_name& name) const override { return name.is_valid(); }
    };
}
