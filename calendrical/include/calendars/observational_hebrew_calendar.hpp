#pragma once

#include "hebrew_based_calendar.hpp"
#include "gregorian_calendar.hpp"
#include "location.hpp"
#include "hebrew_calendar.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    class observational_hebrew_calendar :
        public hebrew_based_calendar {

        std::shared_ptr<const gregorian_calendar> m_gregorian;
        std::shared_ptr<const hebrew_calendar> m_hebrew;

        static fixed_date_t first_of_nisan(int32_t gyear);

        fixed_date_t get_crescent(const fixed_date_t& date) const;

    public:
        static constexpr location hebrew_location{ 32.82, 35, 0, moment_t::hms(2) };

        observational_hebrew_calendar() : m_gregorian{ gregorian_calendar::standard.get_shared() }, m_hebrew{ new hebrew_calendar() } {}
        virtual ~observational_hebrew_calendar() = default;

        virtual fixed_date_t get_new_year(int32_t year) const override;
        virtual int32_t get_year(const fixed_date_t& date) const override;

        virtual hebrew_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const hebrew_date& date) const override;

        virtual hebrew_month get_month(const fixed_date_t& date) const override;

        virtual int32_t get_day(const fixed_date_t& date) const override;

        virtual bool is_leap_year(int32_t year) const override;

        fixed_date_t classical_passover_eve(int32_t gyear) const;
    };
}