#pragma once

#include "moment.hpp"

namespace calendrical::calendars{
    class hindu_base{
    protected:
        static constexpr fixed_date_t ms_epoch{ -1132959 };

        static constexpr double arya_solar_year{1577917500.0 / 4320000};
        static constexpr double arya_jovian_period{1577917500.0 / 364224};

        static constexpr double arya_solar_month{arya_solar_year / 12};
        static constexpr double arya_lunar_month{1577917500.0 / 53433336};
        static constexpr double arya_lunar_day{arya_lunar_month / 30};

        static constexpr double lunar_leap_year_indicator{23902504679.0 / 1282400064.0};

        static constexpr double arya_solar_lunar_month_difference{arya_solar_month - arya_lunar_month};

    public:
        hindu_base() = default;
        virtual ~hindu_base() = default;

        [[nodiscard]] virtual int64_t day_count(const fixed_date_t& date) const;
        [[nodiscard]] virtual int32_t jovian_year(const fixed_date_t& date) const;
    };
}