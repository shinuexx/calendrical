#pragma once

#include "bahai_based_calendar.hpp"

namespace calendrical::calendars{

    /**
     * @brief Baha'i calendar implementation using the historical/tabular year model.
     */
    class bahai_calendar : public bahai_based_calendar{
    protected:
        /** @brief Returns elapsed Baha'i years from epoch for a fixed date. */
        [[nodiscard]] int32_t get_years(const fixed_date_t& date) const override;

    public:
        /** @brief Constructs a Baha'i calendar instance. */
        bahai_calendar() = default;
        ~bahai_calendar() override = default;

        /** @brief Converts a fixed date to Baha'i date components. */
        [[nodiscard]] bahai_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts Baha'i date components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const bahai_date& date) const override;
    };
}
