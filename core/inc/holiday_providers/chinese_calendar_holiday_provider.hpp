#pragma once

#include "calendars/chinese_based_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {
    /**
     * @brief Holiday provider for Chinese-family lunisolar calendars.
     *
     * Works with any @ref calendars::chinese_based_calendar, so the same rules apply to the
     * Chinese, Korean, Vietnamese and Japanese variants (each with its own location).
     */
    class chinese_calendar_holiday_provider final : public holiday_provider_if {
        const calendars::chinese_based_calendar* m_chinese;
        const calendars::gregorian_calendar* m_gregorian;

    public:
        /** @brief Constructs a provider bound to a Chinese-family calendar instance. */
        explicit chinese_calendar_holiday_provider(const calendars::chinese_based_calendar* chinese)
            : m_chinese(chinese),
              m_gregorian(std::addressof(calendars::gregorian_calendar::standard)) {}

        ~chinese_calendar_holiday_provider() override = default;

        /** @brief Returns the backing Chinese-family calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override { return m_chinese; }
        /** @brief Returns Chinese observances that fall in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;

        /** @brief Chinese New Year (first day of month 1) in Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t new_year(int32_t gyear) const;
        /** @brief Qingming (day of the minor solar term at 15 degrees) in Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t qing_ming(int32_t gyear) const;
        /** @brief Dragon Festival (month 5, day 5) in Gregorian year @p gyear. */
        [[nodiscard]] fixed_date_t dragon_festival(int32_t gyear) const;
    };
}
