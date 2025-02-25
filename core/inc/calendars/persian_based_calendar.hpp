#pragma once

#include "datetypes/simple_date.hpp"
#include "location.hpp"

namespace calendrical::calendars {
    using simple_date = datetypes::simple_date;

    class persian_based_calendar :
        public typed_calendar_if<simple_date>
    {
    protected:
        static const fixed_date_t ms_epoch;

        [[nodiscard]] int32_t get_month(int32_t day_of_year) const;
        [[nodiscard]] int32_t get_day_of_year(const fixed_date_t& date, int32_t year) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date, int32_t year, int32_t month) const;

    public:
        static constexpr location tehran{ 35.68, 51.42, 1100, moment_t::hms(3.5) };

        persian_based_calendar() = default;
        ~persian_based_calendar() override = default;

        [[nodiscard]] virtual fixed_date_t get_new_year(int32_t year) const = 0;
        [[nodiscard]] virtual int32_t get_year(const fixed_date_t& date) const = 0;

        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override = 0;

        [[nodiscard]] virtual int32_t get_month(const fixed_date_t& date) const;

        [[nodiscard]] virtual int32_t get_day(const fixed_date_t& date) const;

        [[nodiscard]] virtual bool is_leap_year(int32_t year) const;
    };
}