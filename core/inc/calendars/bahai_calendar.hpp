#pragma once

#include "bahai_based_calendar.hpp"

namespace calendrical::calendars{

    class bahai_calendar : public bahai_based_calendar{
    protected:
        [[nodiscard]] int32_t get_years(const fixed_date_t& date) const override;

    public:
        bahai_calendar() = default;
        ~bahai_calendar() override = default;

        [[nodiscard]] bahai_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const bahai_date& date) const override;
    };
}