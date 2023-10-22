#pragma once

#include "interfaces.hpp"
#include "calendars/islamic_based_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {

    using namespace calendrical::calendars;

    class islamic_based_calendar_holiday_provider :
        public holiday_provider_if,
        public in_gregorian_aware_provider_if<int32_t> {

        std::shared_ptr<const islamic_based_calendar> m_islamic;
        std::shared_ptr<const gregorian_calendar> m_gregorian;

    public:
        islamic_based_calendar_holiday_provider(std::shared_ptr<const islamic_based_calendar> islamic) : m_islamic{ islamic }, m_gregorian{ gregorian_calendar::standard.get_shared() } {}
        virtual ~islamic_based_calendar_holiday_provider() = default;

        virtual std::shared_ptr<const calendar_if> get_calendar() const override;
        virtual std::list<holiday_t> get_holiday_in_year(int32_t gyear) const override;

        virtual std::list<fixed_date_t> in_gregorian(int32_t imonth, int32_t iday, int32_t gyear) const override;

        std::list<fixed_date_t> mawlid(int32_t year) const;
    };
}