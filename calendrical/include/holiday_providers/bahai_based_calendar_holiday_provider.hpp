#pragma once

#include "interfaces.hpp"
#include "calendars/bahai_calendar.hpp"
#include "calendars/astro_bahai_calendar.hpp"

namespace calendrical::holiday_providers {
    
    using namespace calendrical::calendars;

    class bahai_based_calendar_holiday_provider :
        public holiday_provider_if {
        std::shared_ptr<const astro_bahai_calendar> m_astro;
        std::shared_ptr<const bahai_calendar> m_classic;
        std::shared_ptr<const gregorian_calendar> m_gregorian;

    public:
        bahai_based_calendar_holiday_provider(std::shared_ptr<const astro_bahai_calendar> astro, std::shared_ptr<const bahai_calendar> classic) {
            m_astro = astro;
            m_classic = classic;
            m_gregorian = gregorian_calendar::standard.get_shared();
        }

        virtual ~bahai_based_calendar_holiday_provider() = default;

        virtual std::shared_ptr<const calendar_if> get_calendar() const override;
        std::shared_ptr<const bahai_based_calendar> get_calendar(bool astro) const;
        virtual std::list<holiday_t> get_holiday_in_year(int32_t gyear) const override;

        fixed_date_t new_year(int32_t gyear) const;
        fixed_date_t naw_ruz(int32_t gyear) const;
        fixed_date_t feast_of_ridvan(int32_t gyear) const;
        fixed_date_t birth_of_the_bab(int32_t gyear) const;
    };
}