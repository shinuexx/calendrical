#pragma once

#include "datetypes/hebrew_date.hpp"

namespace calendrical::calendars {
    using hebrew_date = datetypes::hebrew_date;

    class hebrew_based_calendar : public typed_calendar_if<hebrew_date> {
    protected:
        static constexpr fixed_date_t ms_epoch{ -1373427 };

    public:
        hebrew_based_calendar() = default;
        ~hebrew_based_calendar() override = default;

        [[nodiscard]] virtual fixed_date_t get_new_year(int32_t year) const = 0;
        [[nodiscard]] virtual int32_t get_year(const fixed_date_t& date) const = 0;

        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        [[nodiscard]] hebrew_date to_date(const fixed_date_t& date) const override = 0;
        [[nodiscard]] fixed_date_t to_fixed(const hebrew_date& date) const override = 0;

        [[nodiscard]] virtual hebrew_month get_month(const fixed_date_t& date) const = 0;

        [[nodiscard]] virtual int32_t get_day(const fixed_date_t& date) const = 0;

        [[nodiscard]] virtual int32_t last_day_of_month(int32_t year, hebrew_month hm) const;
        [[nodiscard]] virtual bool is_short_kislev(int32_t year) const;
        [[nodiscard]] virtual bool is_long_marheshvan(int32_t year) const;
        [[nodiscard]] virtual int32_t days_in_year(int32_t year) const;

        [[nodiscard]] virtual bool is_leap_year(int32_t year) const = 0;

        [[nodiscard]] virtual hebrew_month last_month_of_year(int32_t year) const;
    };
}