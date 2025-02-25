#pragma once

#include "datetypes/icelandic_date.hpp"

namespace calendrical::calendars{
    using icelandic_date = datetypes::icelandic_date;

    class icelandic_calendar final:
        public typed_calendar_if<icelandic_date>{

        fixed_date_t m_epoch;

        static constexpr std::array<int64_t, 3> a{97,24,1};

        [[nodiscard]] season get_season(const fixed_date_t& date, int32_t year) const;

        [[nodiscard]] int32_t get_week(const fixed_date_t& date, int32_t year, season s) const;
        [[nodiscard]] int32_t get_month(const icelandic_date& odate, const fixed_date_t& date) const;

    public:
        icelandic_calendar();

        ~icelandic_calendar() override = default;

        [[nodiscard]] fixed_date_t summer(int32_t iyear) const;
        [[nodiscard]] fixed_date_t winter(int32_t iyear) const;

        [[nodiscard]] fixed_date_t get_new_year(int32_t year) const;
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;

        [[nodiscard]] fixed_date_t epoch() const override { return m_epoch; }

        [[nodiscard]] icelandic_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const icelandic_date& date) const override;

        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_month(const icelandic_date& date) const;

        [[nodiscard]] bool is_leap_year(int32_t year) const;

        [[nodiscard]] season get_season(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_week(const fixed_date_t& date) const;
    };
}