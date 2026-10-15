#pragma once

#include "calendars/gregorian_calendar.hpp"
#include "calendars/pawukon_calendar.hpp"

#include <memory>
#include <vector>

namespace calendrical::holiday_providers {
    /**
     * @brief Holiday provider for Balinese Pawukon observances (Calendrical Calculations, ch. 12).
     *
     * Both holidays recur within the 210-day Pawukon cycle, so a Gregorian year
     * holds many occurrences; every one is returned, in ascending order.
     */
    class pawukon_calendar_holiday_provider final : public holiday_provider_if {
        /** @brief Calendar the holidays are defined in (not owned). */
        const calendars::pawukon_calendar* m_pawukon;
        /** @brief Gregorian calendar used to map holidays into Gregorian years. */
        const calendars::gregorian_calendar* m_gregorian;

    public:
        /** @brief Position of Kajeng Keliwon in the 15-day subcycle: the 9th day (triwara 3, pancawara 5). */
        static constexpr int64_t kajeng_keliwon_day = 8;
        /** @brief Length of the Kajeng Keliwon subcycle (lcm of the 3- and 5-day weeks). */
        static constexpr int64_t kajeng_keliwon_period = 15;
        /** @brief Position of Tumpek in the 35-day subcycle: the 14th day (pancawara 5, saptawara 7). */
        static constexpr int64_t tumpek_day = 13;
        /** @brief Length of the Tumpek subcycle (lcm of the 5- and 7-day weeks). */
        static constexpr int64_t tumpek_period = 35;

        /** @brief Constructs a provider bound to a Pawukon calendar implementation. */
        explicit pawukon_calendar_holiday_provider(const calendars::pawukon_calendar* pawukon)
            : m_pawukon(pawukon),
              m_gregorian(std::addressof(calendars::gregorian_calendar::standard)) {}

        ~pawukon_calendar_holiday_provider() override = default;

        /** @brief Returns the backing Pawukon calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override { return m_pawukon; }
        /** @brief Returns every Kajeng Keliwon and Tumpek in Gregorian year @p gyear, sorted by date. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t gyear) const override;

        /**
         * @brief Kajeng Keliwon occurrences in Gregorian year @p gyear (CC `kajeng-keliwon`).
         *
         * Days that are both triwara 3 (Kajeng) and pancawara 5 (Keliwon); they
         * recur every 15 days.
         */
        [[nodiscard]] std::vector<fixed_date_t> kajeng_keliwon(int32_t gyear) const;
        /**
         * @brief Tumpek occurrences in Gregorian year @p gyear (CC `tumpek`).
         *
         * Days that are both pancawara 5 (Keliwon) and saptawara 7 (Saturday);
         * they recur every 35 days.
         */
        [[nodiscard]] std::vector<fixed_date_t> tumpek(int32_t gyear) const;
    };
}
