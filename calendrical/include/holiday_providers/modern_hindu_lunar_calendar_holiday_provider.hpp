#pragma once

#include "calendars/modern_hindu_lunar_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {

    using namespace calendrical::calendars;

    class modern_hindu_lunar_calendar_holiday_provider :
        public holiday_provider_if,
        public in_gregorian_aware_provider_if<int32_t> {

        std::shared_ptr<const modern_hindu_lunar_calendar> m_lunar;

    public:

        modern_hindu_lunar_calendar_holiday_provider(std::shared_ptr<const modern_hindu_lunar_calendar> lunar) : m_lunar{ lunar } {}

        virtual std::shared_ptr<const calendar_if> get_calendar() const override;
        virtual std::list<holiday_t> get_holiday_in_year(int32_t gyear) const override;

        virtual std::list<fixed_date_t> in_gregorian(int32_t cmonth, int32_t day, int32_t gyear) const override;

        fixed_date_t date_occur(int32_t lyear, int32_t lmonth, int32_t lday) const;
        std::list<fixed_date_t> lunar_holiday(int32_t lmonth, int32_t lday, int32_t gyear) const;

        std::list<fixed_date_t> diwali(int32_t gyear) const;

        fixed_date_t tithi_occur(int32_t lmonth, const rational& tithi, const rational_moment_t& t, int32_t lyear) const;
        std::list<fixed_date_t> lunar_event(int32_t lmonth, const rational& tithi, const rational_moment_t& t, int32_t gyear) const;
        
        std::list<fixed_date_t> shiva(int32_t gyear) const;
        std::list<fixed_date_t> rama(int32_t gyear) const;

        static int32_t karana(int32_t n);

        std::list<fixed_date_t> sacred_wednesdays(int32_t gyear) const;
        std::list<fixed_date_t> sacred_wednesdays_in_range(const range<fixed_date_t>& range) const;
    };
}