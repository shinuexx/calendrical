#include "holiday_providers/persian_based_calendar_holiday_provider.hpp"

namespace calendrical::holiday_providers {
    persian_based_calendar_holiday_provider::persian_based_calendar_holiday_provider(const calendars::persian_based_calendar* persian) : m_persian(persian) {
        m_gregorian_year_persian_epoch = calendars::gregorian_calendar::standard.get_year(m_persian->epoch());
    }

    const calendar_if* persian_based_calendar_holiday_provider::calendar() const {
        return m_persian;
    }

    std::vector<holiday_t> persian_based_calendar_holiday_provider::in_year(int32_t gyear) const {
        std::vector<holiday_t> result;
        result.emplace_back("nawruz", nawruz(gyear));
        std::ranges::sort(result);
        return result;
    }

    fixed_date_t persian_based_calendar_holiday_provider::nawruz(int32_t year) const {
        const auto persian_year = year - m_gregorian_year_persian_epoch;
        const auto y = persian_year <= 0 ? persian_year - 1 : persian_year;
        return m_persian->get_new_year(y);
    }
}