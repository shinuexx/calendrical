#pragma once

#include "hebrew_based_calendar.hpp"
#include "gregorian_calendar.hpp"
#include "location.hpp"
#include "hebrew_calendar.hpp"

namespace calendrical::calendars{

    class observational_hebrew_calendar: public hebrew_based_calendar{
        const gregorian_calendar* m_gregorian;
        std::unique_ptr<const hebrew_calendar> m_hebrew;

        [[nodiscard]] static fixed_date_t first_of_nisan(int32_t gyear);

        [[nodiscard]] fixed_date_t get_crescent(const fixed_date_t& date) const;

    public:
        static constexpr location hebrew_location{ 32.82, 35, 0, moment_t::hms(2) };

        observational_hebrew_calendar();
        ~observational_hebrew_calendar() override = default;

        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const override;
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const override;

        [[nodiscard]] hebrew_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const hebrew_date& date) const override;

        [[nodiscard]] hebrew_month get_month(const fixed_date_t& date) const override;

        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const override;

        [[nodiscard]] bool is_leap_year(int32_t year) const override;

        [[nodiscard]] fixed_date_t classical_passover_eve(int32_t gyear) const;
    };
}