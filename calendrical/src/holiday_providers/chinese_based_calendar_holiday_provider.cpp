#include "holiday_providers/chinese_based_calendar_holiday_provider.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {
    std::shared_ptr<const calendar_if> chinese_based_calendar_holiday_provider::get_calendar() const
    {
        return m_chinese;
    }
    std::list<holiday_t> calendrical::holiday_providers::chinese_based_calendar_holiday_provider::get_holiday_in_year(int32_t gyear) const
    {
        std::list<holiday_t> holidays{};
        holidays.push_back({ "new year", new_year(gyear) });
        holidays.push_back({ "dragon festival", dragon_festival(gyear) });
        holidays.sort();
        return holidays;
    }
    fixed_date_t chinese_based_calendar_holiday_provider::new_year(int32_t gyear) const
    {
        return m_chinese->new_year_on_or_before(gregorian_calendar::standard.to_fixed({ gyear, month::july, 1 }));
    }
    fixed_date_t chinese_based_calendar_holiday_provider::dragon_festival(int32_t gyear) const
    {
        int64_t elasped_years = 1LL + gyear - gregorian_year_chinese_epoch;
        auto cycle = (int32_t)floor_div(elasped_years - 1, 60) + 1;
        auto year = (int32_t)adjusted_remainder(elasped_years, 60);
        return m_chinese->to_fixed({ cycle, year, 5, false, 5 });
    }
    fixed_date_t chinese_based_calendar_holiday_provider::qing_ming(int32_t gyear) const
    {
        return (int64_t)std::floor(m_chinese->minor_solar_term_on_of_after(gregorian_calendar::standard.to_fixed({ gyear, month::march, 30 })));
    }
}