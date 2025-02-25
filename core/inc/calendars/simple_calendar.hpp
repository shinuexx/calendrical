#pragma once

#include "../datetypes/simple_date.hpp"

namespace calendrical::calendars{
    using simple_date = datetypes::simple_date;
    class simple_calendar final :
        public typed_calendar_if<simple_date>
    {
        fixed_date_t m_epoch;

    public:
        constexpr simple_calendar(const fixed_date_t& epoch) : m_epoch(epoch) {} // NOLINT(*-explicit-constructor)

        ~simple_calendar() override = default;

        [[nodiscard]] constexpr fixed_date_t epoch() const override { return m_epoch; }

        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;

        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;

        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
    };

    extern const simple_calendar egyptian_calendar;
    extern const simple_calendar armenian_calendar;
}