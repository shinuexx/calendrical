#pragma once

#include "gregorian_calendar.hpp"
#include "datetypes/iso_date.hpp"

namespace calendrical::calendars{
    using iso_date = datetypes::iso_date;
    class iso_calendar final :
        public typed_calendar_if<iso_date>
    {
        const gregorian_calendar* m_gregorian;

        fixed_date_t m_epoch;

        [[nodiscard]] int32_t get_week(const fixed_date_t& date, int32_t year) const;

    public:
        iso_calendar();

        ~iso_calendar() override = default;

        [[nodiscard]] fixed_date_t epoch() const override;

        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;

        [[nodiscard]] iso_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const iso_date& date) const override;

        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        [[nodiscard]] bool is_leap_year(int32_t year) const;
        [[nodiscard]] bool is_long_year(int32_t year) const;

        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_week(const fixed_date_t& date) const;
    };
}