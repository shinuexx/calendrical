#include "calendars/icelandic_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"
#include <numeric>

namespace calendrical::calendars {
    icelandic_calendar::icelandic_calendar() {
        m_epoch = gregorian_calendar::standard.to_fixed({1, month::april, 19});
    }

    fixed_date_t icelandic_calendar::summer(int32_t iyear) const {
        const std::array y {
            floor_div(iyear, 400),
            mod(floor_div(iyear, 100), 4),
            mod(floor_div(iyear, 4), 25)
        };

        const auto apr19 = m_epoch + 365 * (iyear - 1LL) + std::inner_product(a.begin(), a.end(), y.begin(), int64_t{0});
        return weekday::thursday.on_or_after(apr19);
    }

    fixed_date_t icelandic_calendar::winter(int32_t iyear) const {
        return summer(iyear + 1).subtract(180);
    }

    fixed_date_t icelandic_calendar::get_new_year(int32_t year) const {
        return summer(year);
    }

    int32_t icelandic_calendar::get_year(const fixed_date_t& date) const {
        const auto approx = static_cast<int32_t>(floor_div(400 * (date - m_epoch + 369), 146097));
        const auto year = date >= summer(approx) ? approx : approx - 1;
        return year;
    }

    icelandic_date icelandic_calendar::to_date(const fixed_date_t &date) const {
        const auto year = get_year(date);
        const auto season = get_season(date, year);
        const auto week = get_week(date, year, season);
        const auto wd = date.get_weekday();
        return { year, season, week, wd };
    }

    fixed_date_t icelandic_calendar::to_fixed(const icelandic_date &date) const {
        fixed_date_t start;
        weekday shift;
        if (date.season == season::summer) {
            start = summer(date.year);
            shift = weekday::thursday;
        }else {
            start = winter(date.year);
            shift = weekday::saturday;
        }
        return start + 7 * (date.week - 1) + mod(date.weekday - shift, 7);
    }

    int32_t icelandic_calendar::get_month(const icelandic_date &odate, const fixed_date_t &date) const {
        const auto year = odate.year;
        const auto season = odate.season;
        const auto midsummer = winter(year).subtract(90);
        const auto t1 = summer(year);
        fixed_date_t start;
        if (season == season::winter)
            start = midsummer + 90;
        else if (date >= midsummer)
            start = midsummer - 90;
        else if (date < t1.add(90))
            start = t1;
        else
            start = midsummer;
        return static_cast<int32_t>(floor_div(date - start, 30)) + 1;
    }

    int32_t icelandic_calendar::get_month(const fixed_date_t &date) const {
        return get_month(to_date(date), date);
    }

    bool icelandic_calendar::is_leap_year(int32_t year) const {
        return summer(year + 1) - summer(year) != 364;
    }

    season icelandic_calendar::get_season(const fixed_date_t &date) const {
        return get_season(date, get_year(date));
    }

    int32_t icelandic_calendar::get_week(const fixed_date_t &date) const {
        const auto year = get_year(date);
        return get_week(date, year, get_season(date, year));
    }

    season icelandic_calendar::get_season(const fixed_date_t &date, int32_t year) const {
        return date < winter(year) ? season::summer : season::winter;
    }

    int32_t icelandic_calendar::get_week(const fixed_date_t &date, int32_t year, season s) const {
        const auto start = s == season::summer ? summer(year) : winter(year);
        return static_cast<int32_t>(floor_div(date - start, 7)) + 1;
    }

    int32_t icelandic_calendar::get_month(const icelandic_date &date) const {
        return get_month(date, to_fixed(date));
    }

}