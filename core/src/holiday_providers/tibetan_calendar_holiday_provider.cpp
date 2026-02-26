#include "holiday_providers/tibetan_calendar_holiday_provider.hpp"

#include <algorithm>

namespace calendrical::holiday_providers {
    std::vector<holiday_t> tibetan_calendar_holiday_provider::in_year(int32_t gyear) const {
        std::vector<holiday_t> result;
        for (const auto& x : losar(gyear))
            result.emplace_back("losar", x);
        for (const auto& x : monlam_chenmo(gyear))
            result.emplace_back("monlam chenmo", x);
        for (const auto& x : chotrul_duchen(gyear))
            result.emplace_back("chotrul duchen", x);
        for (const auto& x : saga_dawa_duchen(gyear))
            result.emplace_back("saga dawa duchen", x);
        for (const auto& x : chokhor_duchen(gyear))
            result.emplace_back("chokhor duchen", x);
        for (const auto& x : lhabab_duchen(gyear))
            result.emplace_back("lhabab duchen", x);
        for (const auto& x : ganden_ngamcho(gyear))
            result.emplace_back("ganden ngamcho", x);
        std::ranges::sort(result);
        return result;
    }

    std::vector<fixed_date_t> tibetan_calendar_holiday_provider::in_gregorian(int32_t tmonth, int32_t tday, int32_t gyear) const {
        const auto range = m_gregorian->get_year_range(gyear);
        const auto tyear = m_tibetan->to_date(range.min).year;
        auto r = std::ranges::iota_view(tyear - 1, tyear + 3) |
            std::views::transform([&](int32_t y2) { return m_tibetan->to_fixed({y2, tmonth, false, tday, false}); }) |
            std::views::filter(range);
        std::vector<fixed_date_t> result{r.begin(), r.end()};
        std::ranges::sort(result);
        result.erase(std::unique(result.begin(), result.end()), result.end());
        return result;
    }

    std::vector<fixed_date_t> tibetan_calendar_holiday_provider::fixed_range_in_gregorian(
        int32_t tmonth,
        int32_t start_day,
        int32_t end_day,
        int32_t gyear) const {
        std::vector<fixed_date_t> result;
        for (int32_t day = start_day; day <= end_day; ++day) {
            auto dates = in_gregorian(tmonth, day, gyear);
            std::ranges::move(dates, std::back_inserter(result));
        }
        std::ranges::sort(result);
        result.erase(std::unique(result.begin(), result.end()), result.end());
        return result;
    }

    std::vector<fixed_date_t> tibetan_calendar_holiday_provider::losar(int32_t gyear) const {
        return m_tibetan->new_year(gyear);
    }

    std::vector<fixed_date_t> tibetan_calendar_holiday_provider::monlam_chenmo(int32_t gyear) const {
        return fixed_range_in_gregorian(1, 4, 11, gyear);
    }

    std::vector<fixed_date_t> tibetan_calendar_holiday_provider::chotrul_duchen(int32_t gyear) const {
        return in_gregorian(1, 15, gyear);
    }

    std::vector<fixed_date_t> tibetan_calendar_holiday_provider::saga_dawa_duchen(int32_t gyear) const {
        return in_gregorian(4, 15, gyear);
    }

    std::vector<fixed_date_t> tibetan_calendar_holiday_provider::chokhor_duchen(int32_t gyear) const {
        return in_gregorian(6, 4, gyear);
    }

    std::vector<fixed_date_t> tibetan_calendar_holiday_provider::lhabab_duchen(int32_t gyear) const {
        return in_gregorian(9, 22, gyear);
    }

    std::vector<fixed_date_t> tibetan_calendar_holiday_provider::ganden_ngamcho(int32_t gyear) const {
        return in_gregorian(10, 25, gyear);
    }
}
