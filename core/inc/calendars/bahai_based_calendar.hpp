#pragma once

#include "gregorian_calendar.hpp"
#include "datetypes/bahai_date.hpp"

namespace calendrical::calendars {
    using bahai_date = datetypes::bahai_date;

    class bahai_based_calendar :
    public typed_calendar_if<bahai_date> {
    protected:
        static constexpr int32_t ms_gregorian_year_bahai_epoch = 1844;

        static constexpr fixed_date_t ms_epoch{ 673222 };

        const gregorian_calendar *m_gregorian;

        [[nodiscard]] virtual int32_t get_years(const fixed_date_t& date) const = 0;
        [[nodiscard]] virtual int32_t get_major(int32_t years) const;
        [[nodiscard]] virtual int32_t get_cycle(int32_t years) const;
        [[nodiscard]] virtual int32_t get_year(int32_t years) const;
        [[nodiscard]] virtual int32_t get_month(const fixed_date_t& date, int32_t major, int32_t cycle, int32_t year) const;
        [[nodiscard]] virtual int32_t get_days(const fixed_date_t& date, const fixed_date_t& new_year) const;
        [[nodiscard]] virtual int32_t get_day(const fixed_date_t& date, int32_t major, int32_t cycle, int32_t year, int32_t month) const;

    public:
        static constexpr int32_t ayyam_i_ha{ 0 };

        bahai_based_calendar();
        ~bahai_based_calendar() override = default;

        [[nodiscard]] virtual int32_t get_major(const fixed_date_t& date) const;
        [[nodiscard]] virtual int32_t get_cycle(const fixed_date_t& date) const;
        [[nodiscard]] virtual int32_t get_year(const fixed_date_t& date) const;
        [[nodiscard]] virtual int32_t get_month(const fixed_date_t& date) const;
        [[nodiscard]] virtual int32_t get_day(const fixed_date_t& date) const;

        [[nodiscard]] fixed_date_t epoch() const override { return ms_epoch; }

        [[nodiscard]] bahai_date to_date(const fixed_date_t& date) const override = 0;
        [[nodiscard]] fixed_date_t to_fixed(const bahai_date& date) const override = 0;
    };
}