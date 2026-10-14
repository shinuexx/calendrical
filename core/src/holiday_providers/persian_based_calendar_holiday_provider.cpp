#include "holiday_providers/persian_based_calendar_holiday_provider.hpp"

#include <algorithm>

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
        for (const auto& x : sizdah_bedar(gyear))
            result.emplace_back("sizdah bedar", x);
        result.emplace_back("chaharshanbe suri", chaharshanbe_suri(gyear));
        for (const auto& x : tirgan(gyear))
            result.emplace_back("tirgan", x);
        for (const auto& x : mehregan(gyear))
            result.emplace_back("mehregan", x);
        for (const auto& x : yalda_night(gyear))
            result.emplace_back("yalda night", x);
        for (const auto& x : sepandarmazgan(gyear))
            result.emplace_back("sepandarmazgan", x);
        std::ranges::sort(result, holiday_t::date_only_less_t{});
        return result;
    }

    std::vector<fixed_date_t> persian_based_calendar_holiday_provider::in_gregorian(int32_t pmonth, int32_t pday, int32_t gyear) const {
        const auto range = calendars::gregorian_calendar::standard.get_year_range(gyear);
        const auto y = m_persian->to_date(range.min).year;
        auto r = std::ranges::iota_view(y - 1, y + 2) |
            std::views::transform([&](int32_t y2) { return m_persian->to_fixed({y2, pmonth, pday}); }) |
            std::views::filter(range);
        return {r.begin(), r.end()};
    }

    fixed_date_t persian_based_calendar_holiday_provider::nawruz(int32_t year) const {
        const auto persian_year = year - m_gregorian_year_persian_epoch + 1;
        const auto y = persian_year <= 0 ? persian_year - 1 : persian_year;
        return m_persian->get_new_year(y);
    }

    std::vector<fixed_date_t> persian_based_calendar_holiday_provider::sizdah_bedar(int32_t year) const {
        return in_gregorian(1, 13, year);
    }

    std::vector<fixed_date_t> persian_based_calendar_holiday_provider::tirgan(int32_t year) const {
        return in_gregorian(4, 13, year);
    }

    std::vector<fixed_date_t> persian_based_calendar_holiday_provider::mehregan(int32_t year) const {
        return in_gregorian(7, 16, year);
    }

    std::vector<fixed_date_t> persian_based_calendar_holiday_provider::yalda_night(int32_t year) const {
        return in_gregorian(9, 30, year);
    }

    std::vector<fixed_date_t> persian_based_calendar_holiday_provider::sepandarmazgan(int32_t year) const {
        return in_gregorian(12, 5, year);
    }

    fixed_date_t persian_based_calendar_holiday_provider::chaharshanbe_suri(int32_t year) const {
        return weekday::wednesday.before(nawruz(year));
    }
}
