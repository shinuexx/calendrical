#pragma once

#include "interfaces.hpp"
#include "calendars/simple_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {

    using namespace calendrical::calendars;

    class simple_calendar_holiday_provider :
        public holiday_provider_if,
        public in_gregorian_aware_provider_if<int32_t> {

        std::shared_ptr<const simple_calendar> m_simple;
        std::shared_ptr<const gregorian_calendar> m_gregorian;

    public:
        simple_calendar_holiday_provider(std::shared_ptr<const simple_calendar> simple) : m_simple{ simple }, m_gregorian{ gregorian_calendar::standard.get_shared() } {}

        virtual std::shared_ptr<const calendar_if> get_calendar() const override;
        virtual std::list<holiday_t> get_holiday_in_year(int32_t gyear) const override;

        virtual std::list<fixed_date_t> in_gregorian(int32_t cmonth, int32_t day, int32_t gyear) const override;
    };
}