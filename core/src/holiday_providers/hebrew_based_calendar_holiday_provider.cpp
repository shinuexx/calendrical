#include "holiday_providers/hebrew_based_calendar_holiday_provider.hpp"

#include <algorithm>
#include <array>

namespace calendrical::holiday_providers {
    hebrew_based_calendar_holiday_provider::hebrew_based_calendar_holiday_provider(
        const calendars::hebrew_based_calendar *hebrew) :
        m_hebrew(hebrew),
        m_gregorian(std::addressof(calendars::gregorian_calendar::standard)),
        m_coptic{std::addressof(calendars::coptic_calendar)}
    {
        m_coptic_provider = std::make_unique<coptic_based_calendar_holiday_provider>(m_coptic);
        m_gregorian_year_hebrew_epoch = m_gregorian->get_year(m_hebrew->epoch());

        m_start_1_adar_21 = m_hebrew->to_fixed({1, hebrew_month::adar, 21});
        m_start_1_adar_28 = m_hebrew->to_fixed({1, hebrew_month::adar, 28});
    }

    std::vector<holiday_t> hebrew_based_calendar_holiday_provider::in_year(int32_t gyear) const {
        std::vector<holiday_t> result;
        result.emplace_back( "rosh ha shanah", rosh_ha_shanah(gyear) );
        result.emplace_back( "rosh ha shanah (day 2)", rosh_ha_shanah_day_2(gyear) );
        result.emplace_back( "yom kippur", yom_kippur(gyear) );
        result.emplace_back( "sukkot", sukkot(gyear) );
        result.emplace_back( "sukkot (diaspora day 2)", sukkot_day_2_diaspora(gyear) );
        result.emplace_back( "hoshana rabba", hoshana_rabba(gyear) );
        result.emplace_back( "shemini atzeret", shemini_atzeret(gyear) );
        result.emplace_back( "simhat torah", simhat_torah(gyear) );
        result.emplace_back( "passover", passover(gyear) );
        result.emplace_back( "passover (diaspora day 2)", passover_day_2_diaspora(gyear) );
        result.emplace_back( "end of passover", end_of_passover(gyear) );
        result.emplace_back( "passover (diaspora day 8)", passover_day_8_diaspora(gyear) );
        result.emplace_back( "shavuot", shavuot(gyear) );
        result.emplace_back( "shavuot (diaspora day 2)", shavuot_day_2_diaspora(gyear) );
        result.emplace_back( "purim", purim(gyear) );
        result.emplace_back( "shushan purim", shushan_purim(gyear) );
        result.emplace_back( "ta anit esther", ta_anit_esther(gyear) );
        result.emplace_back( "tzom gedaliah", tzom_gedaliah(gyear) );
        result.emplace_back( "tzom tammuz", tzom_tammuz(gyear) );
        result.emplace_back( "tishah be av", tishah_be_av(gyear) );
        result.emplace_back( "yom ha shoah", yom_ha_shoah(gyear) );
        result.emplace_back( "yom ha zikkaron", yom_ha_zikkaron(gyear) );
        for (auto& x : sh_ela(gyear))
            result.emplace_back( "sh ela", x );
        for (auto& x : birhath_ha_hama(gyear))
            result.emplace_back( "birhath ha hama", x );
        for (auto& x : tzom_tevet(gyear))
            result.emplace_back( "tzom tevet", x );
        for (auto& x : rosh_hodesh(gyear))
            result.emplace_back( "rosh hodesh", x );
        for (auto& x : intermediate_days_of_sukkot(gyear))
            result.emplace_back( "intermediate days of sukkot", x );
        for (auto& x : intermediate_days_of_passover(gyear))
            result.emplace_back( "intermediate days of passover", x );
        for (auto& x : hanukkah_days(gyear))
            result.emplace_back( "hanukkah", x );
        std::ranges::sort(result, holiday_t::date_only_less_t{});
        return result;
    }

    std::vector<fixed_date_t> hebrew_based_calendar_holiday_provider::in_gregorian(hebrew_month hmonth, int32_t hday,
                                                                                   int32_t gyear) const {
        const auto range = m_gregorian->get_year_range(gyear);
        const auto y = m_hebrew->get_year(range.min);
        auto r = std::views::iota(y, y + 3) |
            std::views::transform([&](int32_t y2) { return m_hebrew->to_fixed({y2, hmonth, hday});}) |
            std::views::filter(range);
        return {r.begin(), r.end()};
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::rosh_ha_shanah(int32_t year) const {
        const auto hyear = year - m_gregorian_year_hebrew_epoch + 1;
        return m_hebrew->to_fixed({hyear, hebrew_month::tishri, 1});
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::yom_kippur(int32_t year) const {
        auto hyear = year - m_gregorian_year_hebrew_epoch + 1;
        return m_hebrew->to_fixed({hyear, hebrew_month::tishri, 10});
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::rosh_ha_shanah_day_2(int32_t year) const {
        return rosh_ha_shanah(year).add(1);
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::sukkot(int32_t year) const {
        const auto hyear = year - m_gregorian_year_hebrew_epoch + 1;
        return m_hebrew->to_fixed({hyear, hebrew_month::tishri, 15});
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::hoshana_rabba(int32_t year) const {
        const auto hyear = year - m_gregorian_year_hebrew_epoch + 1;
        return m_hebrew->to_fixed({hyear, hebrew_month::tishri, 21});
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::shemini_atzeret(int32_t year) const {
        const auto hyear = year - m_gregorian_year_hebrew_epoch + 1;
        return m_hebrew->to_fixed({hyear, hebrew_month::tishri, 22});
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::simhat_torah(int32_t year) const {
        const auto hyear = year - m_gregorian_year_hebrew_epoch + 1;
        return m_hebrew->to_fixed({hyear, hebrew_month::tishri, 23});
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::sukkot_day_2_diaspora(int32_t year) const {
        return sukkot(year).add(1);
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::passover(int32_t year) const {
        auto hyear = year - m_gregorian_year_hebrew_epoch;
        return m_hebrew->to_fixed({hyear, hebrew_month::nisan, 15});
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::end_of_passover(int32_t year) const {
        const auto hyear = year - m_gregorian_year_hebrew_epoch;
        return m_hebrew->to_fixed({hyear, hebrew_month::nisan, 21});
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::shavuot(int32_t year) const {
        const auto hyear = year - m_gregorian_year_hebrew_epoch;
        return m_hebrew->to_fixed({hyear, hebrew_month::sivan, 6});
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::passover_day_2_diaspora(int32_t year) const {
        return passover(year).add(1);
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::passover_day_8_diaspora(int32_t year) const {
        return passover(year).add(7);
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::shavuot_day_2_diaspora(int32_t year) const {
        return shavuot(year).add(1);
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::purim(int32_t year) const {
        auto hyear = year - m_gregorian_year_hebrew_epoch;
        auto last_month = m_hebrew->last_month_of_year(hyear);
        return m_hebrew->to_fixed({hyear, last_month, 14});
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::shushan_purim(int32_t year) const {
        const auto date = purim(year).add(1);
        return date.get_weekday() == weekday::saturday ? date.add(1) : date;
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::ta_anit_esther(int32_t year) const {
        const auto purim_date = purim(year);
        return purim_date.get_weekday() == weekday::sunday ? purim_date.subtract(3) : purim_date.subtract(1);
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::tzom_gedaliah(int32_t year) const {
        const auto hyear = year - m_gregorian_year_hebrew_epoch + 1;
        const auto date = m_hebrew->to_fixed({hyear, hebrew_month::tishri, 3});
        return date.get_weekday() == weekday::saturday ? date.add(1) : date;
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::tzom_tammuz(int32_t year) const {
        const auto hyear = year - m_gregorian_year_hebrew_epoch;
        const auto date = m_hebrew->to_fixed({hyear, hebrew_month::tammuz, 17});
        return date.get_weekday() == weekday::saturday ? date.add(1) : date;
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::tishah_be_av(int32_t year) const {
        auto hyear = year - m_gregorian_year_hebrew_epoch;
        const auto av9 = m_hebrew->to_fixed({hyear, hebrew_month::av, 9});
        return av9.get_weekday() == weekday::saturday ? av9.add(1) : av9;
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::yom_ha_shoah(int32_t year) const {
        const auto hyear = year - m_gregorian_year_hebrew_epoch;
        const auto date = m_hebrew->to_fixed({hyear, hebrew_month::nisan, 27});
        return date.get_weekday() == weekday::sunday ? date.add(1) : date;
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::yom_ha_zikkaron(int32_t year) const {
        auto hyear = year - m_gregorian_year_hebrew_epoch;
        const auto iyyar4 = m_hebrew->to_fixed({hyear, hebrew_month::iyyar, 4});
        const auto wd = iyyar4.get_weekday();
        if (in(std::array{weekday::thursday, weekday::friday}, wd))
            return weekday::wednesday.before(iyyar4);
        if (weekday::sunday == wd)
            return iyyar4.add(1);
        return iyyar4;
    }

    std::vector<fixed_date_t> hebrew_based_calendar_holiday_provider::sh_ela(int32_t year) const {
        return m_coptic_provider->in_gregorian(3, 26, year);
    }

    std::vector<fixed_date_t> hebrew_based_calendar_holiday_provider::birhath_ha_hama(int32_t year) const {
        auto r = m_coptic_provider->in_gregorian(7,30,year);
        auto e = std::ranges::remove_if(
                r, std::not_fn([&](const fixed_date_t& date) {
            return 17 == mod(m_coptic->get_year(date), 28);
        }));
        r.erase(e.begin(), e.end());
        return r;
    }

    std::vector<fixed_date_t> hebrew_based_calendar_holiday_provider::tzom_tevet(int32_t year) const {
        return in_gregorian(hebrew_month::tevet, 10, year);
    }

    std::vector<fixed_date_t> hebrew_based_calendar_holiday_provider::rosh_hodesh(int32_t year) const {
        const auto range = m_gregorian->get_year_range(year);
        const auto hy = m_hebrew->get_year(range.min);
        std::vector<fixed_date_t> result;

        for (const auto hyear : std::views::iota(hy - 1, hy + 3)) {
            const auto last_month = static_cast<int32_t>(m_hebrew->last_month_of_year(hyear));
            for (int32_t month_num = static_cast<int32_t>(hebrew_month::nisan); month_num <= last_month; ++month_num) {
                const auto month = static_cast<hebrew_month>(month_num);
                const auto day1 = m_hebrew->to_fixed({hyear, month, 1});
                if (range(day1)) {
                    result.push_back(day1);
                }

                const hebrew_month prev_month =
                    (month == hebrew_month::nisan)
                        ? m_hebrew->last_month_of_year(hyear)
                        : static_cast<hebrew_month>(month_num - 1);
                if (m_hebrew->last_day_of_month(hyear, prev_month) == 30) {
                    const auto day0 = day1.subtract(1);
                    if (range(day0)) {
                        result.push_back(day0);
                    }
                }
            }
        }

        std::ranges::sort(result);
        result.erase(std::unique(result.begin(), result.end()), result.end());
        return result;
    }

    std::vector<fixed_date_t> hebrew_based_calendar_holiday_provider::intermediate_days_of_sukkot(int32_t year) const {
        std::vector<fixed_date_t> result;
        const auto start = sukkot(year).add(1); // Tishri 16
        for (int i = 0; i < 6; ++i) {
            result.push_back(start.add(i));
        }
        return result;
    }

    std::vector<fixed_date_t> hebrew_based_calendar_holiday_provider::intermediate_days_of_passover(int32_t year) const {
        std::vector<fixed_date_t> result;
        const auto start = passover(year).add(1); // Nisan 16
        for (int i = 0; i < 5; ++i) {
            result.push_back(start.add(i));
        }
        return result;
    }

    std::vector<moment_t> hebrew_based_calendar_holiday_provider::samuel_season_in_gregorian(const angle_t &season,
                                                                                             int32_t year) const {
        constexpr auto Y = 365.25;
        const auto start = static_cast<double>(m_start_1_adar_21) + 0.75;
        const auto offset = season / angle_t::full_circle * Y;
        auto r = m_gregorian->cycle_in_gregorian(season, year, Y, moment_t{ start + offset });
        return {r.begin(), r.end()};
    }

    std::vector<moment_t>
    hebrew_based_calendar_holiday_provider::add_season_in_gregorian(const angle_t &season, int32_t year) const {
        const auto start = static_cast<double>(static_cast<int64_t>(m_start_1_adar_28)) + 0.25;
        const auto offset = season / angle_t::full_circle * ADDA_Y;
        auto r = m_gregorian->cycle_in_gregorian(season, year, ADDA_Y, moment_t{start + offset});
        return {r.begin(), r.end()};
    }

    std::vector<fixed_date_t> hebrew_based_calendar_holiday_provider::hanukkah(int32_t year) const {
        return in_gregorian(hebrew_month::kislev, 25, year);
    }

    std::vector<fixed_date_t> hebrew_based_calendar_holiday_provider::hanukkah_days(int32_t year) const {
        const auto range = m_gregorian->get_year_range(year);
        std::vector<fixed_date_t> result;
        for (const auto& start : hanukkah(year)) {
            for (int i = 0; i < 8; ++i) {
                if (const auto d = start.add(i); range(d)) {
                    result.push_back(d);
                }
            }
        }
        std::ranges::sort(result);
        result.erase(std::ranges::unique(result).begin(), result.end());
        return result;
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::birthday(const datetypes::hebrew_date &birthdate,
                                                                  int32_t hyear) const {
        auto& [year, month, day] = birthdate;
        if (month == m_hebrew->last_month_of_year(year))
            return m_hebrew->to_fixed({hyear, m_hebrew->last_month_of_year(hyear), day});
        return m_hebrew->to_fixed({hyear, month, 1}).add(day - 1LL);
    }

    fixed_date_t hebrew_based_calendar_holiday_provider::yahrzeit(const datetypes::hebrew_date &deathdate,
                                                                  int32_t hyear) const {
        auto& [death_year, death_month, death_day] = deathdate;
        if (death_month == hebrew_month::marheshvan && death_day == 30 && !m_hebrew->is_long_marheshvan(death_year + 1))
            return m_hebrew->to_fixed({hyear, hebrew_month::kislev, 1}).subtract(1);
        if (death_month == hebrew_month::kislev && death_day == 30 && m_hebrew->is_short_kislev(death_year + 1))
            return m_hebrew->to_fixed({hyear, hebrew_month::tevet, 1}).subtract(1);
        if (death_month == hebrew_month::adarii)
            return m_hebrew->to_fixed({hyear, m_hebrew->last_month_of_year(hyear), death_day});
        if (death_day == 30 && death_month == hebrew_month::adar && !m_hebrew->is_leap_year(hyear))
            return m_hebrew->to_fixed({hyear, hebrew_month::shevat, 30});
        return m_hebrew->to_fixed({hyear, death_month, 1}).add(death_day - 1ll);
    }

    std::vector<fixed_date_t>
    hebrew_based_calendar_holiday_provider::birthday_in_gregorian(const datetypes::hebrew_date &birthdate,
                                                                  int32_t gyear) const {
        const auto range = m_gregorian->get_year_range(gyear);
        const auto y = m_hebrew->get_year(range.min);
        auto r = std::views::iota(y, y + 3) |
            std::views::transform([&](int32_t y2) { return birthday(birthdate, y2);}) |
            std::views::filter(range);
        return {r.begin(), r.end()};
    }

    std::vector<fixed_date_t>
    hebrew_based_calendar_holiday_provider::yahrzeit_in_gregorian(const datetypes::hebrew_date &deathdate,
                                                                  int32_t gyear) const {
        const auto range = m_gregorian->get_year_range(gyear);
        const auto y = m_hebrew->get_year(range.min);
        auto r = std::views::iota(y, y + 3) |
            std::views::transform([&](int32_t y2) { return yahrzeit(deathdate, y2);}) |
            std::views::filter(range);
        return {r.begin(), r.end()};
    }

}
