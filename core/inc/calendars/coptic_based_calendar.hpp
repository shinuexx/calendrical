#pragma once

#include "datetypes/simple_date.hpp"

namespace calendrical::calendars {
    using simple_date = datetypes::simple_date;

    class coptic_based_calendar :
    public typed_calendar_if<simple_date> {
        fixed_date_t m_epoch;

        [[nodiscard]] int32_t get_month(const fixed_date_t& date, int32_t year) const;
    public:
        // ReSharper disable once CppNonExplicitConvertingConstructor
        constexpr coptic_based_calendar(const fixed_date_t& epoch) : m_epoch(epoch) {} // NOLINT(*-explicit-constructor)

        ~coptic_based_calendar() override = default;

        [[nodiscard]] fixed_date_t epoch() const override { return m_epoch; }

        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;

        [[nodiscard]] fixed_date_t to_fixed(const simple_date& date) const override;
        [[nodiscard]] simple_date to_date(const fixed_date_t& date) const override;

        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;
        [[nodiscard]] bool is_leap_year(int32_t year) const;
    };

    extern const coptic_based_calendar coptic_calendar;
    extern const coptic_based_calendar ethiopic_calendar;
}