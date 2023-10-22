#pragma once

#include "interfaces.hpp"
#include "calendars/persian_based_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {

    using namespace calendrical::calendars;

    class persian_based_calendar_holiday_provider :
        public holiday_provider_if {

        std::shared_ptr<const persian_based_calendar> m_persian;

        int32_t m_gregorian_year_persian_epoch;

    public:
        persian_based_calendar_holiday_provider(std::shared_ptr<const persian_based_calendar> persian);
        virtual ~persian_based_calendar_holiday_provider() = default;

        virtual std::shared_ptr<const calendar_if> get_calendar() const override;
        virtual std::list<holiday_t> get_holiday_in_year(int32_t gyear) const override;

        fixed_date_t nawruz(int32_t year) const;
    };
}