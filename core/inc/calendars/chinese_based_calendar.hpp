#pragma once

#include "chinese_base.hpp"
#include "datetypes/chinese_date.hpp"
#include "datetypes/sexagesimal_name.hpp"
#include "types.hpp"

namespace calendrical::calendars {
    using chinese_date = datetypes::chinese_date;
    using sexagesimal_name = datetypes::sexagesimal_name;

    class chinese_based_calendar :
        public typed_calendar_if<chinese_date>,
        public chinese_base{
        fixed_date_t m_epoch;
    public:
        explicit chinese_based_calendar(const fixed_date_t& epoch) : m_epoch{ epoch } {}
        chinese_based_calendar();

        ~chinese_based_calendar() override = default;

        [[nodiscard]] fixed_date_t epoch() const override { return m_epoch; }

        [[nodiscard]] location get_location(const moment_t& t) const override = 0;

        [[nodiscard]] chinese_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const chinese_date& date) const override;

        [[nodiscard]] int32_t get_cycle(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_month(const fixed_date_t& date) const;
        [[nodiscard]] bool is_leap_month(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_day(const fixed_date_t& date) const;

        [[nodiscard]] static sexagesimal_name year_name(int32_t n);

        static constexpr int32_t month_name_epoch = 57;

        [[nodiscard]] static sexagesimal_name month_name(int32_t month, int32_t year);

        static constexpr fixed_date_t day_name_epoch{ 45 };

        [[nodiscard]] static sexagesimal_name day_name(const fixed_date_t& date);

        static const sexagesimal_name day_name_zero;

        [[nodiscard]] static fixed_date_t day_name_on_or_before(const sexagesimal_name& name, const fixed_date_t& date);
    };
}