#include "calendars/chinese_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::calendars {
    location chinese_calendar::get_location(const moment_t& t) const {
        if (const auto year = gregorian_calendar::standard.get_year(t.to_fixed_date()); year < 1929)
            return {angle_t::dms(39, 55), angle_t::dms(116, 25), 43.5, moment_t::hms(1397 / 1.8e2)};
        return {angle_t::dms(39, 55), angle_t::dms(116, 25), 43.5, moment_t::hms(8)};
    }

    chinese_marriage_augury chinese_calendar::year_marriage_augury(int32_t cycle, int32_t year) const
    {
        const auto new_year = to_fixed({ cycle, year, 1, false, 1 });
        const auto c = year == 60 ? cycle + 1 : cycle;
        const auto y = year == 60 ? 1 : year + 1;
        const auto next_new_year = to_fixed({c, y, 1, false, 1});
        const auto first_minor_term = current_major_solar_term(new_year);
        const auto next_first_minor_term = current_major_solar_term(next_new_year);
        if (first_minor_term == 1 && next_first_minor_term == 12)
            return chinese_marriage_augury::WIDOW;
        if (first_minor_term == 1)
            return chinese_marriage_augury::BLIND;
        if (next_first_minor_term == 12)
            return chinese_marriage_augury::BRIGHT;
        return chinese_marriage_augury::DOUBLE_BRIGHT;
    }
}