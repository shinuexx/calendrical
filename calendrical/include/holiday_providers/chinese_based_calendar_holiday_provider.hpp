#pragma once

#include "interfaces.hpp"
#include "calendars/chinese_based_calendar.hpp"


namespace calendrical::holiday_providers {

    using namespace calendrical::calendars;

    class chinese_based_calendar_holiday_provider :
        public holiday_provider_if {

        std::shared_ptr<const chinese_based_calendar> m_chinese;

        static constexpr int32_t gregorian_year_chinese_epoch{ -2636 };

    public:
        chinese_based_calendar_holiday_provider(std::shared_ptr<const chinese_based_calendar> chinese) : m_chinese{ chinese } {}

        virtual std::shared_ptr<const calendar_if> get_calendar() const override;
        virtual std::list<holiday_t> get_holiday_in_year(int32_t gyear) const override;

        fixed_date_t new_year(int32_t gyear) const;
        fixed_date_t dragon_festival(int32_t gyear) const;
        fixed_date_t qing_ming(int32_t gyear) const;
    };
}