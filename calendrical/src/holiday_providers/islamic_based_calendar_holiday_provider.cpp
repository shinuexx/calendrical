#include "holiday_providers/islamic_based_calendar_holiday_provider.hpp"

namespace calendrical::holiday_providers {
    std::shared_ptr<const calendar_if> islamic_based_calendar_holiday_provider::get_calendar() const {
        return m_islamic;
    }

    std::list<holiday_t> islamic_based_calendar_holiday_provider::get_holiday_in_year(int32_t gyear) const {
        std::list<holiday_t> holidays{};
        for (auto x : mawlid(gyear))
            holidays.push_back({ "Mawlid", x });
        holidays.sort();
        return holidays;
    }

    std::list<fixed_date_t> islamic_based_calendar_holiday_provider::in_gregorian(int32_t imonth, int32_t iday, int32_t gyear) const {
        auto range = m_gregorian->get_year_range(gyear);
        auto y = m_islamic->get_year(range.min);
        std::list<fixed_date_t> dates{};
        for (int32_t x : std::array{ y, y + 1, y + 2 }) {
            dates.push_back(m_islamic->to_fixed({ x, imonth, iday }));
        }
        filter(dates, range);
        return dates;
    }

    std::list<fixed_date_t> islamic_based_calendar_holiday_provider::mawlid(int32_t year) const {
        return in_gregorian(3, 12, year);
    }
}