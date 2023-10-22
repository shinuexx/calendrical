#include "holiday_providers/persian_based_calendar_holiday_provider.hpp"

namespace calendrical::holiday_providers {
    persian_based_calendar_holiday_provider::persian_based_calendar_holiday_provider(std::shared_ptr<const persian_based_calendar> persian) : m_persian{ persian }
    {
        m_gregorian_year_persian_epoch = gregorian_calendar::standard.get_year(m_persian->get_epoch());
    }
    std::shared_ptr<const calendar_if> persian_based_calendar_holiday_provider::get_calendar() const {
        return m_persian;
    }

    std::list<holiday_t> persian_based_calendar_holiday_provider::get_holiday_in_year(int32_t gyear) const {
        std::list<holiday_t> holidays{};
        holidays.push_back({ "nawruz", nawruz(gyear) });
        holidays.sort();
        return holidays;
    }
    fixed_date_t persian_based_calendar_holiday_provider::nawruz(int32_t year) const
    {
        auto persian_year = year - m_gregorian_year_persian_epoch + 1;
        auto y = persian_year <= 0 ? persian_year - 1 : persian_year;
        return m_persian->get_new_year(y);
    }
}