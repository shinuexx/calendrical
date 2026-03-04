#include "calendars/gregorian_calendar.hpp"

namespace calendrical::calendars{
    const gregorian_calendar gregorian_calendar::standard{};

    month gregorian_calendar::get_month(const fixed_date_t& date, int32_t year) const {
        const auto prior_days = date - get_new_year(year);
        const auto correction = date < to_fixed({year, month::march, 1}) ? 0 : is_leap_year(year) ? 1 : 2;
        return static_cast<month>(floor_div(12 * (prior_days + correction) + 373, 367));
    }

    fixed_date_t gregorian_calendar::get_new_year(int32_t year) const {
        return to_fixed({year, month::january, 1});
    }

    int32_t gregorian_calendar::get_year(const fixed_date_t& date) const {
        const auto d0 = date - m_epoch;
        const auto n400 = floor_div(d0, 146097);
        const auto d1 = mod(d0, 146097);
        const auto n100 = floor_div(d1, 36524);
        const auto d2 = mod(d1, 36524);
        const auto n4 = floor_div(d2, 1461);
        const auto d3 = mod(d2, 1461);
        const auto n1 = floor_div(d3, 365);
        const auto year = 400 * n400 + 100 * n100 + n4 * 4 + n1;
        return static_cast<int32_t>(
            n100 == 4 || n1 == 4 ?
            year :
            year + 1
        );
    }

    gregorian_date gregorian_calendar::to_date(const fixed_date_t& date) const {
        auto year = get_year(date);
        auto month = get_month(date, year);
        const auto day = date - to_fixed({year, month, 1}) + 1;
        return {year, month, static_cast<int32_t>(day)};
    }

    fixed_date_t gregorian_calendar::to_fixed(const gregorian_date& date) const {
        const auto& [year, month, day] = date;
        const auto m = static_cast<int64_t>(month);
        const int64_t y1 = year - 1LL;
        return m_epoch - 1
            + 365 * y1
            + floor_div(y1, 4)
            - floor_div(y1, 100)
            + floor_div(y1, 400)
            + floor_div(367 * m - 362, 12)
            + (month <= month::february ? 0 : is_leap_year(year) ? -1 : -2)
            + day;
    }

    month gregorian_calendar::get_month(const fixed_date_t &date) const {
        const auto year = get_year(date);
        return get_month(date, year);
    }

    int32_t gregorian_calendar::get_day(const fixed_date_t &date) const { return to_date(date).day; }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    bool gregorian_calendar::is_leap_year(int32_t year) const { // NOLINT(*-convert-member-functions-to-static)
        constexpr std::array<int64_t, 3> non_leap_years = {100, 200, 300};
        return 0 == mod(year, 4) && !in(non_leap_years, mod(year, 400));
    }

    fixed_date_t gregorian_calendar::get_year_end(int32_t year) const {
        return to_fixed({year, month::december, 31});
    }

    interval<fixed_date_t> gregorian_calendar::get_year_range(int32_t year) const {
        return interval_left_closed_between(get_new_year(year), get_new_year(year + 1));
    }

    int32_t gregorian_calendar::get_remaining_days(const gregorian_date &date) const {
        return static_cast<int32_t>(to_fixed({date.year, month::december, 31}) - to_fixed(date));
    }

    fixed_date_t gregorian_calendar::nth_weekday(int64_t n, weekday k, const gregorian_date &date) const {
        if (n > 0)
            return 7 * n + k.before(to_fixed(date));
        if (n < 0)
            return 7 * n + k.after(to_fixed(date));
        throw std::range_error("n == 0");
    }

    fixed_date_t gregorian_calendar::first_weekday(weekday k, const gregorian_date &date) const {
        return nth_weekday(1, k, date);
    }

    fixed_date_t gregorian_calendar::last_weekday(weekday k, const gregorian_date &date) const {
        return nth_weekday(-1, k, date);
    }

    std::ranges::transform_view<std::ranges::owning_view<std::vector<double>>, bind_constructor<moment_t>> gregorian_calendar::cycle_in_gregorian(const angle_t &season, int32_t year, double L,
                                                               const moment_t &start) const {
        const auto year_range = get_year_range(year);
        const auto pos = season / 360_deg * L;
        const auto D = pos - mod(start, L);
        auto t = positions_in_range<double>(pos, L, D, year_range);
        return std::move(t) | std::views::transform(bind_constructor<moment_t>{});
    }
}