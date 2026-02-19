#pragma once

#include "datetypes/lunar_date.hpp"
#include "angle.hpp"

namespace calendrical::calendars {
    using lunar_date = datetypes::lunar_date;

    class tibetan_calendar :
        public typed_calendar_if<lunar_date>
    {
        static const fixed_date_t ms_epoch;

        [[nodiscard]] static angle_t tibetan_sun_equation(const angle_t& a);
        [[nodiscard]] static angle_t tibetan_moon_equation(const angle_t& a);

    public:

        tibetan_calendar() = default;
        ~tibetan_calendar() override = default;

        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        [[nodiscard]] lunar_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const lunar_date& date) const override;

        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        [[nodiscard]] bool is_leap_month(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
        [[nodiscard]] bool is_leap_day(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;

        [[nodiscard]] bool is_leap_month(int32_t year, int32_t month) const;
        [[nodiscard]] bool is_leap_day(int32_t year, int32_t month, int32_t day) const;
 
        [[nodiscard]] fixed_date_t losar(int32_t tyear) const;

        [[nodiscard]] std::vector<fixed_date_t> new_year(int32_t gyear) const;
    };
}