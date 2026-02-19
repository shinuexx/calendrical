#pragma once

namespace calendrical::datetypes {
    struct chinese_date{
        int32_t cycle;
        int32_t year;
        int32_t month;
        bool leap_month;
        int32_t day;

        constexpr chinese_date() = default;
        constexpr chinese_date(int32_t cycle, int32_t year, int32_t month, bool leap_month, int32_t day)
            : cycle(cycle), year(year), month(month), leap_month(leap_month), day(day) {}

        constexpr bool operator==(const chinese_date &other) const = default;

        [[nodiscard]] std::string to_string() const;

        virtual ~chinese_date() = default;
    };
}