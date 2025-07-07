#include "holiday_providers/modern_hindu_lunar_calendar_holiday_provider.hpp"
#include "time_and_astronomy.hpp"

namespace calendrical::holiday_providers {
    using namespace calendrical::datetypes;
    using namespace calendrical::calendars;

    const calendar_if* modern_hindu_lunar_calendar_holiday_provider::calendar() const
    {
        return m_lunar;
    }
    std::vector<holiday_t> modern_hindu_lunar_calendar_holiday_provider::in_year(int32_t gyear) const
    {
        return {};
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

    fixed_date_t modern_hindu_lunar_calendar_holiday_provider::tithi_occur(int32_t lmonth, const rational& tithi, const rational_moment_t& t, int32_t lyear) const
    {
        const auto approx = date_occur(lyear, lmonth, static_cast<int32_t>(floor(tithi)));
        const auto lunar = modern_hindu_lunar_calendar::hindu_lunar_day_at_or_after(tithi, approx - 2);
        const auto try_ = lunar.to_fixed_date();
        if (const auto th = time_and_astronomy::standard_from_sundial(static_cast<double>(try_) + static_cast<moment_t>(t),
                                                                static_cast<location>(modern_hindu_base::ujjain));
            lunar <= th || modern_hindu_lunar_calendar::hindu_lunar_phase(time_and_astronomy::standard_from_sundial(
                                   static_cast<double>(try_) + 1 + static_cast<moment_t>(t),
                                   static_cast<location>(modern_hindu_base::ujjain))) > 12 * tithi)
            return try_;
        return try_.add(1);
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::lunar_event(int32_t lmonth, const rational& tithi, const rational_moment_t& t, int32_t gyear) const
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
        return lunar_event(11, 29, rational{1}, gyear);
    }

    std::vector<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::rama(int32_t gyear) const
    {
        return lunar_event(1, 9, rational{ 1,2 }, gyear);
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