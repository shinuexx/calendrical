#include "holiday_providers/samaritan_calendar_holiday_provider.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>

namespace calendrical::holiday_providers {
    std::vector<fixed_date_t> samaritan_calendar_holiday_provider::in_gregorian(
        int32_t smonth,
        int32_t sday,
        int32_t gyear) const {
        const auto range = m_gregorian->get_year_range(gyear);
        const auto y = m_samaritan->to_date(range.min).year;
        auto r = std::ranges::iota_view(y - 1, y + 2) |
            std::views::transform([&](int32_t y2) { return m_samaritan->to_fixed({y2, smonth, sday}); }) |
            std::views::filter(range);
        return {r.begin(), r.end()};
    }

    std::vector<fixed_date_t> samaritan_calendar_holiday_provider::fixed_range_in_gregorian(
        int32_t smonth,
        int32_t start_day,
        int32_t end_day,
        int32_t gyear) const {
        std::vector<fixed_date_t> result;
        for (int32_t day = start_day; day <= end_day; ++day) {
            auto dates = in_gregorian(smonth, day, gyear);
            std::ranges::move(dates, std::back_inserter(result));
        }
        std::ranges::sort(result);
        result.erase(std::unique(result.begin(), result.end()), result.end());
        return result;
    }

    std::vector<holiday_t> samaritan_calendar_holiday_provider::in_year(int32_t gyear) const {
        std::vector<holiday_t> result;
        for (const auto& x : passover(gyear))
            result.emplace_back("passover", x);
        for (const auto& x : festival_of_unleavened_bread(gyear))
            result.emplace_back("festival of the unleavened bread", x);
        result.emplace_back("festival of pentecost", festival_of_pentecost(gyear));
        for (const auto& x : festival_of_the_seventh_month(gyear))
            result.emplace_back("festival of the seventh month", x);
        for (const auto& x : day_of_atonement(gyear))
            result.emplace_back("day of atonement", x);
        for (const auto& x : festival_of_tabernacles(gyear))
            result.emplace_back("festival of tabernacles", x);
        for (const auto& x : eighth_day(gyear))
            result.emplace_back("the eighth day", x);
        result.emplace_back("simmut of passover", simmut_of_passover(gyear));
        result.emplace_back("simmut of tabernacles", simmut_of_tabernacles(gyear));
        std::ranges::sort(result);
        return result;
    }

    std::vector<fixed_date_t> samaritan_calendar_holiday_provider::passover(int32_t gyear) const {
        return in_gregorian(1, 14, gyear);
    }

    std::vector<fixed_date_t> samaritan_calendar_holiday_provider::festival_of_unleavened_bread(int32_t gyear) const {
        return fixed_range_in_gregorian(1, 15, 21, gyear);
    }

    std::vector<fixed_date_t> samaritan_calendar_holiday_provider::festival_of_the_seventh_month(int32_t gyear) const {
        return in_gregorian(7, 1, gyear);
    }

    std::vector<fixed_date_t> samaritan_calendar_holiday_provider::day_of_atonement(int32_t gyear) const {
        return in_gregorian(7, 10, gyear);
    }

    std::vector<fixed_date_t> samaritan_calendar_holiday_provider::festival_of_tabernacles(int32_t gyear) const {
        return fixed_range_in_gregorian(7, 15, 21, gyear);
    }

    std::vector<fixed_date_t> samaritan_calendar_holiday_provider::eighth_day(int32_t gyear) const {
        return in_gregorian(7, 22, gyear);
    }

    fixed_date_t samaritan_calendar_holiday_provider::festival_of_pentecost(int32_t gyear) const {
        const auto p = passover(gyear);
        if (p.empty()) {
            throw std::runtime_error("No Samaritan Passover found in Gregorian year " + std::to_string(gyear));
        }
        const auto first_sunday_after_passover = weekday::sunday.after(p.front());
        return first_sunday_after_passover.add(49);
    }

    fixed_date_t samaritan_calendar_holiday_provider::simmut_of_passover(int32_t gyear) const {
        const auto p = passover(gyear);
        if (p.empty()) {
            throw std::runtime_error("No Samaritan Passover found in Gregorian year " + std::to_string(gyear));
        }
        return weekday::saturday.on_or_before(p.front().subtract(49));
    }

    fixed_date_t samaritan_calendar_holiday_provider::simmut_of_tabernacles(int32_t gyear) const {
        const auto t = festival_of_tabernacles(gyear);
        if (t.empty()) {
            throw std::runtime_error("No Samaritan Tabernacles found in Gregorian year " + std::to_string(gyear));
        }
        return weekday::saturday.on_or_before(t.front().subtract(49));
    }
}
