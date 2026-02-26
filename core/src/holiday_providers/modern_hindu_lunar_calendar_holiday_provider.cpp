#include "holiday_providers/modern_hindu_lunar_calendar_holiday_provider.hpp"
#include "time_and_astronomy.hpp"
#include <algorithm>
#include <cmath>

namespace calendrical::holiday_providers {
    using namespace calendrical::datetypes;
    using namespace calendrical::calendars;

    const calendar_if* modern_hindu_lunar_calendar_holiday_provider::calendar() const
    {
        return m_lunar;
    }
    std::vector<holiday_t> modern_hindu_lunar_calendar_holiday_provider::in_year(int32_t gyear) const
    {
        std::vector<holiday_t> result;
        for (const auto& x : diwali_days(gyear))
            result.emplace_back("diwali", x);
        for (const auto& x : shiva(gyear))
            result.emplace_back("maha shivaratri", x);
        for (const auto& x : rama(gyear))
            result.emplace_back("rama navami", x);
        for (const auto& x : janmashtami(gyear))
            result.emplace_back("janmashtami", x);
        for (const auto& x : ganesha_chaturthi(gyear))
            result.emplace_back("ganesha chaturthi", x);
        for (const auto& x : durga_ashtami(gyear))
            result.emplace_back("durga ashtami", x);
        for (const auto& x : sarasvati_puja(gyear))
            result.emplace_back("sarasvati puja", x);
        for (const auto& x : dasra(gyear))
            result.emplace_back("dasra", x);
        for (const auto& x : karthikai_deepam(gyear))
            result.emplace_back("karthikai deepam", x);
        for (const auto& x : vaikunta_ekadashi(gyear))
            result.emplace_back("vaikunta ekadashi", x);
        for (const auto& x : buddha_birthday(gyear))
            result.emplace_back("buddha birthday", x);
        for (const auto& x : varalakshmi_vratam(gyear))
            result.emplace_back("varalakshmi vratam", x);
        for (const auto& x : holi(gyear))
            result.emplace_back("holi", x);
        for (const auto& x : sacred_wednesdays(gyear))
            result.emplace_back("sacred wednesday", x);
        std::ranges::sort(result);
        return result;
    }
    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::in_gregorian(int32_t cmonth, int32_t day, int32_t gyear) const
    {
        return lunar_holiday(cmonth, day, gyear);
    }
    fixed_date_t modern_hindu_lunar_calendar_holiday_provider::date_occur(int32_t lyear, int32_t lmonth, int32_t lday) const
    {
        const lunar_date lunar{lyear, lmonth, false, lday, false};
        const auto try_ = m_lunar->to_fixed(lunar);
        const auto mid = m_lunar->to_date(lday > 15 ? try_.subtract(5) : try_);
        const auto expunged = lmonth != mid.month;
        const lunar_date ldate{ mid.year, mid.month, mid.leap_month, lday, false };
        if (expunged) {
            return least_passing(
                           [&](const int64_t &d) { return m_lunar->hindu_lunar_on_or_before(m_lunar->to_date(d), ldate); },
                           try_) -
                   1;
        }
        if (lday != m_lunar->to_date(try_).day) {
            return try_ - 1;
        }
        return try_;
    }
    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::lunar_holiday(int32_t lmonth, int32_t lday, int32_t gyear) const
    {
        auto range = gregorian_calendar::standard.get_year_range(gyear);
        const auto lyear = m_lunar->get_year(range.min);
        std::vector dates{
            date_occur(lyear, lmonth, lday),
            date_occur(lyear + 1, lmonth, lday),
        };
        auto r = dates | std::views::filter(range);
        return {r.begin(), r.end()};
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::diwali(int32_t gyear) const
    {
        return lunar_holiday(8, 1, gyear);
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::diwali_days(int32_t gyear) const
    {
        std::vector<fixed_date_t> result;
        auto a29 = lunar_holiday(7, 29, gyear);
        auto a30 = lunar_holiday(7, 30, gyear);
        auto k1 = diwali(gyear);
        std::ranges::move(a29, std::back_inserter(result));
        std::ranges::move(a30, std::back_inserter(result));
        std::ranges::move(k1, std::back_inserter(result));
        std::ranges::sort(result);
        result.erase(std::ranges::unique(result).begin(), result.end());
        return result;
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::janmashtami(int32_t gyear) const
    {
        return lunar_holiday(5, 23, gyear);
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::ganesha_chaturthi(int32_t gyear) const
    {
        return lunar_holiday(6, 4, gyear);
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::durga_ashtami(int32_t gyear) const
    {
        return lunar_holiday(7, 8, gyear);
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::sarasvati_puja(int32_t gyear) const
    {
        return lunar_holiday(7, 9, gyear);
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::dasra(int32_t gyear) const
    {
        return lunar_holiday(7, 10, gyear);
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::karthikai_deepam(int32_t gyear) const
    {
        return lunar_holiday(8, 15, gyear);
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::vaikunta_ekadashi(int32_t gyear) const
    {
        return lunar_holiday(9, 11, gyear);
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::buddha_birthday(int32_t gyear) const
    {
        // Convention-specific simplification: Vaiśākha 15.
        return lunar_holiday(2, 15, gyear);
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::varalakshmi_vratam(int32_t gyear) const
    {
        auto anchors = lunar_holiday(5, 15, gyear);
        auto r = anchors | std::views::transform([](const fixed_date_t& d) { return weekday::friday.before(d); });
        return {r.begin(), r.end()};
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::holi(int32_t gyear) const
    {
        // Evening observance of Phalguna 15, approximated with a sundial-time fraction near sunset.
        return lunar_event(12, 15, 0.75, gyear);
    }

    fixed_date_t modern_hindu_lunar_calendar_holiday_provider::tithi_occur(int32_t lmonth, const double& tithi, const moment_t& t, int32_t lyear) const
    {
        const auto approx = date_occur(lyear, lmonth, static_cast<int32_t>(std::floor(tithi)));
        const auto lunar = modern_hindu_lunar_calendar::hindu_lunar_day_at_or_after(
            tithi,
            moment_t{approx.subtract(2)}
        );
        const auto try_ = lunar.to_fixed_date();
        if (const auto th = time_and_astronomy::standard_from_sundial(static_cast<double>(try_) + static_cast<moment_t>(t),
                                                                modern_hindu_base::ujjain);
            lunar <= th || modern_hindu_lunar_calendar::hindu_lunar_phase(time_and_astronomy::standard_from_sundial(
                                   static_cast<double>(try_) + 1 + static_cast<moment_t>(t),
                                   modern_hindu_base::ujjain)) > angle_t{12 * tithi})
            return try_;
        return try_.add(1);
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::lunar_event(int32_t lmonth, const double& tithi, const moment_t& t, int32_t gyear) const
    {
        auto range = gregorian_calendar::standard.get_year_range(gyear);
        const auto lyear = m_lunar->get_year(range.min);
        std::vector dates{
            tithi_occur(lmonth, tithi, t, lyear),
            tithi_occur(lmonth, tithi, t, lyear + 1)
        };
        auto r = dates | std::views::filter(range);
        return {r.begin(), r.end()};
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::shiva(int32_t gyear) const
    {
        return lunar_event(11, 29, 1, gyear);
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::rama(int32_t gyear) const
    {
        return lunar_event(1, 9, 0.5, gyear);
    }

    int32_t modern_hindu_lunar_calendar_holiday_provider::karana(int32_t n)
    {
        n = static_cast<int32_t>(adjusted_remainder(n, 60));
        if (n == 1)
            return 0;
        if (n > 57)
            return n - 50;
        return static_cast<int32_t>(adjusted_remainder(n - 1, 7));
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::sacred_wednesdays(int32_t gyear) const
    {
        return sacred_wednesdays_in_range(gregorian_calendar::standard.get_year_range(gyear));
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::sacred_wednesdays_in_range(const range<fixed_date_t>& range) const
    {
        std::vector<fixed_date_t> days{};
        for (auto wed = weekday::wednesday.on_or_after(range.min); wed < range.max; wed = weekday::wednesday.on_or_after(wed.add(1)))
            if (m_lunar->get_day(wed) == 8)
                days.push_back(wed);
        return days;
    }
}
