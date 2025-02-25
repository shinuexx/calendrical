#pragma once

#include "datetypes/simple_date.hpp"
#include "location.hpp"

namespace calendrical::calendars {
    using simple_date = datetypes::simple_date;

    class french_based_calendar :
        public typed_calendar_if<simple_date>
    {
    protected:
        static constexpr fixed_date_t ms_epoch{654415};

        int32_t get_month(const fixed_date_t& date, const fixed_date_t& new_year) const;
        int32_t get_day(const fixed_date_t& date, const fixed_date_t& new_year) const;

    public:
        static constexpr location paris{angle_t::dms(48,50,11), angle_t::dms(2,20,15), 27, moment_t::hms(1)};

        french_based_calendar() = default;
        ~french_based_calendar() override = default;

        [[nodiscard]] virtual fixed_date_t get_new_year(int32_t year) const = 0;
        [[nodiscard]] virtual int32_t get_year(const fixed_date_t& date) const = 0;

        [[nodiscard]] fixed_date_t epoch() const override;

        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override = 0;
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override = 0;

        [[nodiscard]] virtual int32_t get_month(const fixed_date_t& date) const = 0;

        [[nodiscard]] virtual int32_t get_day(const fixed_date_t& date) const = 0;

        [[nodiscard]] virtual bool is_leap_year(int32_t year) const = 0;
    };
}