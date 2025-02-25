#pragma once

#include "datetypes/simple_date.hpp"

namespace calendrical::calendars{
    using simple_date = datetypes::simple_date;

    class islamic_based_calendar : public typed_calendar_if<simple_date>{
    protected:
        static constexpr fixed_date_t ms_epoch{ 227015 };

    public:
        islamic_based_calendar() = default;
        ~islamic_based_calendar() override = default;

        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const { return to_fixed({year, 1, 1}); }
        [[nodiscard]] virtual int32_t get_year(const fixed_date_t &date) const = 0;

        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override = 0;
        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override = 0;

        [[nodiscard]] virtual int32_t get_month(const fixed_date_t& date) const = 0;

        [[nodiscard]] virtual int32_t get_day(const fixed_date_t& date) const = 0;
    };
}