#include "holiday_providers/modern_hindu_lunar_calendar_holiday_provider.hpp"
#include "time_and_astronomy.hpp"

namespace calendrical::holiday_providers {
    std::shared_ptr<const calendar_if> modern_hindu_lunar_calendar_holiday_provider::get_calendar() const
    {
        return m_lunar;
    }
    std::list<holiday_t> modern_hindu_lunar_calendar_holiday_provider::get_holiday_in_year(int32_t gyear) const
    {
        return std::list<holiday_t>();
    }
    std::list<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::in_gregorian(int32_t cmonth, int32_t day, int32_t gyear) const
    {
        return lunar_holiday(cmonth, day, gyear);
    }
    fixed_date_t modern_hindu_lunar_calendar_holiday_provider::date_occur(int32_t lyear, int32_t lmonth, int32_t lday) const
    {
        lunar_date lunar{ lyear, lmonth, false, lday, false };
        auto try_ = m_lunar->to_fixed(lunar);
        auto mid = m_lunar->to_date(lday > 15 ? try_.subtract(5) : try_);
        auto expunged = lmonth != mid.month;
        lunar_date ldate{ mid.year, mid.month, mid.leap_month, lday, false };
        if (expunged) {
            return least_passing([=](int64_t d) { return m_lunar->hindu_lunar_on_or_before(m_lunar->to_date(d), ldate); }, try_) - 1;
        }
        else if (lday != m_lunar->to_date(try_).day) {
            return try_ - 1;
        }
        else {
            return try_;
        }
    }
    std::list<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::lunar_holiday(int32_t lmonth, int32_t lday, int32_t gyear) const
    {
        auto range = gregorian_calendar::standard.get_year_range(gyear);
        auto lyear = m_lunar->get_year(range.min);
        std::list<fixed_date_t> dates{
            date_occur(lyear, lmonth, lday),
            date_occur(lyear + 1, lmonth, lday),
        };
        filter(dates, range);
        return dates;
    }

    std::list<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::diwali(int32_t gyear) const
    {
        return lunar_holiday(8, 1, gyear);
    }

    fixed_date_t modern_hindu_lunar_calendar_holiday_provider::tithi_occur(int32_t lmonth, const rational& tithi, const rational_moment_t& t, int32_t lyear) const
    {
        auto approx = date_occur(lyear, lmonth, (int32_t)floor(tithi));
        auto lunar = m_lunar->hindu_lunar_day_at_or_after(tithi, approx - 2);
        auto try_ = lunar.to_fixed_date();
        auto th = time_and_astronomy::standard_from_sundial(try_ + (moment_t)t, (location)modern_hindu_base::ujjain);
        if (lunar <= th || (m_lunar->hindu_lunar_phase(time_and_astronomy::standard_from_sundial(try_ + 1 + (moment_t)t, (location)modern_hindu_base::ujjain))) > (12 * tithi))
            return try_;
        else
            return try_.add(1);
    }

    std::list<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::lunar_event(int32_t lmonth, const rational& tithi, const rational_moment_t& t, int32_t gyear) const
    {
        auto range = gregorian_calendar::standard.get_year_range(gyear);
        auto lyear = m_lunar->get_year(range.min);
        std::list<fixed_date_t> dates{
            tithi_occur(lmonth, tithi, t, lyear),
            tithi_occur(lmonth, tithi, t, lyear + 1)
        };
        filter(dates, range);
        return dates;
    }

    std::list<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::shiva(int32_t gyear) const
    {
        return lunar_event(11, 29, 1LL, gyear);
    }

    std::list<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::rama(int32_t gyear) const
    {
        return lunar_event(1, 9, rational{ 1,2 }, gyear);
    }

    int32_t modern_hindu_lunar_calendar_holiday_provider::karana(int32_t n)
    {
        n = (int32_t)adjusted_remainder(n, 60);
        if (n == 1)
            return 0;
        else if (n > 57)
            return (n - 50);
        else
            return (int32_t)adjusted_remainder(n - 1LL, 7);
    }

    std::list<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::sacred_wednesdays(int32_t gyear) const
    {
        return sacred_wednesdays_in_range(gregorian_calendar::standard.get_year_range(gyear));
    }

    std::list<fixed_date_t> modern_hindu_lunar_calendar_holiday_provider::sacred_wednesdays_in_range(const range<fixed_date_t>& range) const
    {
        std::list<fixed_date_t> days{};
        for (auto wed = weekday::wednesday.on_or_after(range.min); wed < range.max; wed = weekday::wednesday.on_or_after(wed.add(1)))
            if (m_lunar->get_day(wed) == 8)
                days.push_back(wed);
        return days;
    }
}