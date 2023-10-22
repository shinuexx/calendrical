#include "holiday_providers/hebrew_based_calendar_holiday_provider.hpp"

namespace calendrical::holiday_providers {
    const rational hebrew_based_calendar_holiday_provider::ADDA_Y{ 35975351 , 98496 }; // 365 + (5 + rational{3791, 4104}) / 24;

    hebrew_based_calendar_holiday_provider::hebrew_based_calendar_holiday_provider(std::shared_ptr<const hebrew_based_calendar> hebrew) :
        m_hebrew{ hebrew },
        m_gregorian{ gregorian_calendar::standard.get_shared() },
        m_coptic{ coptic_calendar.get_shared() }
    {
        m_coptic_provider = std::make_shared<coptic_based_calendar_holiday_provider>(m_coptic);
        m_gregorian_year_hebrew_epoch = m_gregorian->get_year(m_hebrew->get_epoch());

        start_1_adar_21 = m_hebrew->to_fixed({ 1, hebrew_month::adar, 21 });
        start_1_adar_28 = m_hebrew->to_fixed({ 1, hebrew_month::adar, 28 });
    }
    std::shared_ptr<const calendar_if> hebrew_based_calendar_holiday_provider::get_calendar() const
    {
        return m_hebrew;
    }
    std::list<holiday_t> hebrew_based_calendar_holiday_provider::get_holiday_in_year(int32_t gyear) const
    {
        std::list<holiday_t> holidays{};
        holidays.push_back({ "yom kippur", yom_kippur(gyear) });
        holidays.push_back({ "passover", passover(gyear) });
        holidays.push_back({ "purim", purim(gyear) });
        holidays.push_back({ "ta anit esther", ta_anit_esther(gyear) });
        holidays.push_back({ "tishah be av", tishah_be_av(gyear) });
        holidays.push_back({ "yom ha zikkaron", yom_ha_zikkaron(gyear) });
        for (auto& x : sh_ela(gyear))
            holidays.push_back({ "sh ela", x });
        for (auto& x : birhath_ha_hama(gyear))
            holidays.push_back({ "birhath ha hama", x });
        for (auto& x : hanukkah(gyear))
            holidays.push_back({ "hanukkah", x });
        holidays.sort();
        return holidays;
    }
    std::list<fixed_date_t> hebrew_based_calendar_holiday_provider::in_gregorian(hebrew_month hmonth, int32_t hday, int32_t gyear) const
    {
        auto range = m_gregorian->get_year_range(gyear);
        auto y = m_hebrew->get_year(range.min);
        std::list<fixed_date_t> dates{};
        for (int32_t x : std::array{ y, y + 1, y + 2 }) {
            dates.push_back(m_hebrew->to_fixed({ x, hmonth, hday }));
        }
        filter(dates, range);
        return dates;
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::yom_kippur(int32_t year) const {
        auto hyear = year - m_gregorian_year_hebrew_epoch + 1;
        return m_hebrew->to_fixed({ hyear, hebrew_month::tishri, 10 });
    }
    fixed_date_t hebrew_based_calendar_holiday_provider::passover(int32_t year) const {
        auto hyear = year - m_gregorian_year_hebrew_epoch;
        return m_hebrew->to_fixed({ hyear, hebrew_month::nisan, 15 });
    }
    fixed_date_t hebrew_based_calendar_holiday_provider::purim(int32_t year) const {
        auto hyear = year - m_gregorian_year_hebrew_epoch;
        auto last_month = m_hebrew->last_month_of_year(hyear);
        return m_hebrew->to_fixed({ hyear, last_month, 14 });
    }
    fixed_date_t hebrew_based_calendar_holiday_provider::ta_anit_esther(int32_t year) const {
        auto purim_date = purim(year);
        return purim_date.get_weekday() == weekday::sunday ? purim_date.substract(3) : purim_date.substract(1);
    }
    fixed_date_t hebrew_based_calendar_holiday_provider::tishah_be_av(int32_t year) const {
        auto hyear = year - m_gregorian_year_hebrew_epoch;
        auto av9 = m_hebrew->to_fixed({ hyear, hebrew_month::av, 9 });
        return av9.get_weekday() == weekday::saturday ? av9.add(1) : av9;
    }
    fixed_date_t hebrew_based_calendar_holiday_provider::yom_ha_zikkaron(int32_t year) const {
        auto hyear = year - m_gregorian_year_hebrew_epoch;
        auto iyyar4 = m_hebrew->to_fixed({ hyear, hebrew_month::iyyar, 4 });
        auto wd = iyyar4.get_weekday();
        if (in(std::array{ weekday::thursday, weekday::friday }, wd))
            return weekday::wednesday.before(iyyar4);
        else if (weekday::sunday == wd)
            return iyyar4.add(1);
        else
            return iyyar4;
    }

    std::list<fixed_date_t> hebrew_based_calendar_holiday_provider::sh_ela(int32_t year) const {
        return m_coptic_provider->in_gregorian(3, 26, year);
    }
    std::list<fixed_date_t> hebrew_based_calendar_holiday_provider::birhath_ha_hama(int32_t year) const {
        auto r = m_coptic_provider->in_gregorian(7, 30, year);
        std::erase_if(r, std::not_fn([=](const fixed_date_t& x) { return 17 == mod(m_coptic->get_year(x), 20ll); }));
        return r;
    }

    std::list<moment_t> hebrew_based_calendar_holiday_provider::samuel_season_in_gregorian(const angle_t& season, int32_t year) const {
        constexpr auto Y = 365.25;
        auto start = start_1_adar_21 + 0.75;
        auto offset = season / angle_t::full_circle * Y;
        return m_gregorian->cycle_in_gregorian(season, year, Y, start + offset );
    }
    std::list<rational_moment_t> hebrew_based_calendar_holiday_provider::add_season_in_gregorian(const angle_t& season, int32_t year) const {
        rational start = static_cast<int64_t>(start_1_adar_28) + rational{ 1,4 };
        rational offset = season / angle_t::full_circle * ADDA_Y;
        return m_gregorian->cycle_in_gregorian(season, year, ADDA_Y, rational_moment_t{ start + offset });
    }
    std::list<fixed_date_t> hebrew_based_calendar_holiday_provider::hanukkah(int32_t year) const
    {
        return in_gregorian(hebrew_month::kislev, 25, year);
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::birthday(const hebrew_date& birthdate, int32_t hyear) const
    {
        auto [year, month, day] = birthdate.tie();
        if (month == m_hebrew->last_month_of_year(year))
            return m_hebrew->to_fixed({ hyear, m_hebrew->last_month_of_year(hyear), day });
        else
            return m_hebrew->to_fixed({ hyear, month, 1 }).add(day - 1ll);
    }
    fixed_date_t hebrew_based_calendar_holiday_provider::yahrzeit(const hebrew_date& deathdate, int32_t hyear) const
    {
        auto [death_year, death_month, death_day] = deathdate.tie();
        if ((death_month == hebrew_month::marheshvan) && (death_day == 30) && !m_hebrew->is_long_marheshvan(death_year + 1))
            return m_hebrew->to_fixed({ hyear, hebrew_month::kislev, 1 }).substract(1);
        else if ((death_month == hebrew_month::kislev) && (death_day == 30) && m_hebrew->is_short_kislev(death_year + 1))
            return m_hebrew->to_fixed({ hyear, hebrew_month::tevet, 1 }).substract(1);
        else if (death_month == hebrew_month::adarii)
            return m_hebrew->to_fixed({ hyear, m_hebrew->last_month_of_year(hyear), death_day });
        else if ((death_day == 30) && (death_month == hebrew_month::adar) && !m_hebrew->is_leap_year(hyear))
            return m_hebrew->to_fixed({ hyear, hebrew_month::shevat, 30 });
        else
            return m_hebrew->to_fixed({ hyear, death_month, 1 }).add(death_day - 1ll);
    }
    std::list<fixed_date_t> hebrew_based_calendar_holiday_provider::birthday_in_gregorian(const hebrew_date& birthdate, int32_t gyear) const
    {
        auto range = m_gregorian->get_year_range(gyear);
        auto y = m_hebrew->get_year(range.min);
        std::list<fixed_date_t> dates{};
        for (int32_t x : std::array{ y, y + 1, y + 2 }) {
            dates.push_back(birthday(birthdate, y));
        }
        filter(dates, range);
        return dates;
    }
    std::list<fixed_date_t> hebrew_based_calendar_holiday_provider::yahrzeit_in_gregorian(const hebrew_date& deathdate, int32_t gyear) const
    {
        auto range = m_gregorian->get_year_range(gyear);
        auto y = m_hebrew->get_year(range.min);
        std::list<fixed_date_t> dates{};
        for (int32_t x : std::array{ y, y + 1, y + 2 }) {
            dates.push_back(yahrzeit(deathdate, y));
        }
        filter(dates, range);
        return dates;
    }
}