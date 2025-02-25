#pragma once

#include "moment.hpp"

namespace calendrical::calendars{
    class hindu_base{
    protected:
        static constexpr fixed_date_t ms_epoch{ -1132959 };

        static const rational arya_solar_year;
        static const rational arya_jovian_period;

        static const rational arya_solar_month;
        static const rational arya_lunar_month;
        static const rational arya_lunar_day;

        static const rational lunar_leap_year_indicator;

        static const rational arya_solar_lunar_month_difference;

    public:
        hindu_base() = default;
        virtual ~hindu_base() = default;

        [[nodiscard]] virtual int64_t day_count(const fixed_date_t& date) const;
        [[nodiscard]] virtual int32_t jovian_year(const fixed_date_t& date) const;
    };
}