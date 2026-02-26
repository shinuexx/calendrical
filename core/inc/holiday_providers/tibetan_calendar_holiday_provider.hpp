#pragma once

#include "calendars/gregorian_calendar.hpp"
#include "calendars/tibetan_calendar.hpp"

namespace calendrical::holiday_providers {
    /**
     * @brief Holiday provider for major Tibetan calendar observances.
     */
    class tibetan_calendar_holiday_provider final : public holiday_provider_if {
        const calendars::tibetan_calendar* m_tibetan;
        const calendars::gregorian_calendar* m_gregorian;

        /** @brief Helper for inclusive fixed-date ranges in a Tibetan month. */
        [[nodiscard]] std::vector<fixed_date_t> fixed_range_in_gregorian(int32_t tmonth, int32_t start_day, int32_t end_day, int32_t gyear) const;

    public:
        /** @brief Constructs a provider bound to a Tibetan calendar instance. */
        explicit tibetan_calendar_holiday_provider(const calendars::tibetan_calendar* tibetan)
            : m_tibetan(tibetan),
              m_gregorian(std::addressof(calendars::gregorian_calendar::standard)) {}

        ~tibetan_calendar_holiday_provider() override = default;

        /** @brief Returns the backing Tibetan calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override { return m_tibetan; }
        /** @brief Returns Tibetan observances that fall in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;

        /**
         * @brief Maps a Tibetan month/day occurrence into Gregorian year @p gyear.
         *
         * This provider uses the non-leap month/day convention (`leap_month=false`, `leap_day=false`).
         */
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(int32_t tmonth, int32_t tday, int32_t gyear) const;

        /** @brief Losar (Tibetan New Year) dates in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> losar(int32_t gyear) const;
        /** @brief Monlam Chenmo (days 4-11 of month 1) in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> monlam_chenmo(int32_t gyear) const;
        /** @brief Chotrul Duchen (month 1, day 15) in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> chotrul_duchen(int32_t gyear) const;
        /** @brief Saga Dawa Duchen (month 4, day 15) in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> saga_dawa_duchen(int32_t gyear) const;
        /** @brief Chokhor Duchen (month 6, day 4) in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> chokhor_duchen(int32_t gyear) const;
        /** @brief Lhabab Duchen (month 9, day 22) in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> lhabab_duchen(int32_t gyear) const;
        /** @brief Ganden Ngamcho (month 10, day 25) in Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> ganden_ngamcho(int32_t gyear) const;
    };
}
