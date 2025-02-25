#pragma once

#include "datetypes/gregorian_date.hpp"
#include "range.hpp"
#include "moment.hpp"
#include "angle.hpp"


namespace calendrical::calendars{
    using gregorian_date = datetypes::gregorian_date;
    class gregorian_calendar final :
    public typed_calendar_if<gregorian_date>
    {
        fixed_date_t m_epoch;

        [[nodiscard]] month get_month(const fixed_date_t &date, int32_t year) const;

    public:
        constexpr gregorian_calendar() : m_epoch{1} {}
        constexpr gregorian_calendar(const fixed_date_t &date) : m_epoch{date} {} // NOLINT(*-explicit-constructor)

        ~gregorian_calendar() override = default;

        [[nodiscard]] constexpr fixed_date_t epoch() const override { return m_epoch; }

        [[nodiscard]] gregorian_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const gregorian_date& date) const override;

        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;
        [[nodiscard]] int32_t get_year(const fixed_date_t &date) const;

        [[nodiscard]] month get_month(const fixed_date_t &date) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t &date) const;
        [[nodiscard]] bool is_leap_year(int32_t year) const;

        [[nodiscard]] fixed_date_t get_year_end(int32_t year) const;
        [[nodiscard]] range<fixed_date_t> get_year_range(int32_t year) const;

        [[nodiscard]] int32_t get_remaining_days(const gregorian_date &date) const;

        [[nodiscard]] fixed_date_t nth_weekday(int64_t n, weekday k, const gregorian_date &date) const;

        [[nodiscard]] fixed_date_t first_weekday(weekday k, const gregorian_date &date) const;
        [[nodiscard]] fixed_date_t last_weekday(weekday k, const gregorian_date &date) const;

        [[nodiscard]] std::ranges::transform_view<std::ranges::owning_view<std::vector<double>>, bind_constructor<moment_t>> cycle_in_gregorian(const angle_t &season, int32_t year, double L, const moment_t& start) const;
        [[nodiscard]] std::ranges::transform_view<std::ranges::owning_view<std::vector<rational>>, bind_constructor<rational_moment_t>> cycle_in_gregorian(const angle_t &season, int32_t year, const rational& L, const rational_moment_t& start) const;

        static const gregorian_calendar standard;
    };
}