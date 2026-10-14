#pragma once

#include "types.hpp"
#include "calendars/islamic_based_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {

    /**
     * @brief Holiday provider for Islamic-calendar holidays.
     */
    class islamic_based_calendar_holiday_provider:
    public holiday_provider_if {
        /** @brief Calendar the holidays are defined in (not owned). */
        const calendars::islamic_based_calendar *m_islamic;
        /** @brief Gregorian calendar used to map holidays into Gregorian years. */
        const calendars::gregorian_calendar *m_gregorian;

    public:
        /** @brief Constructs a provider bound to a specific Islamic calendar implementation. */
        explicit islamic_based_calendar_holiday_provider(const calendars::islamic_based_calendar *islamic) :
            m_islamic(islamic),
            m_gregorian(std::addressof(calendars::gregorian_calendar::standard)){}

        ~islamic_based_calendar_holiday_provider() override = default;

        /** @brief Returns the backing Islamic calendar interface. */
        [[nodiscard]] const calendar_if* calendar() const override { return m_islamic; }
        /** @brief Returns the provider's holiday set for Gregorian year @p year. */
        [[nodiscard]] std::vector<holiday_t> in_year(int32_t year) const override;
        /** @brief Maps Islamic month/day occurrences into Gregorian year @p gyear. */
        [[nodiscard]] std::vector<fixed_date_t> in_gregorian(int32_t imonth, int32_t iday, int32_t gyear) const;

        /** @brief Mawlid dates in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> mawlid(int32_t year) const;
        /** @brief Islamic New Year (Muharram 1) dates in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> islamic_new_year(int32_t year) const;
        /** @brief Ashura (Muharram 10) dates in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> ashura(int32_t year) const;
        /** @brief Lailat al-Miraj (Rajab 27) dates in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> lailat_al_miraj(int32_t year) const;
        /** @brief Lailat al-Baraa (Shaban 15) dates in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> lailat_al_baraa(int32_t year) const;
        /** @brief Ramadan start (Ramadan 1) dates in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> ramadan(int32_t year) const;
        /** @brief Lailat al-Qadr (Ramadan 27) dates in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> lailat_al_qadr(int32_t year) const;
        /** @brief Eid ul-Fitr (Shawwal 1) dates in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> eid_ul_fitr(int32_t year) const;
        /** @brief Eid ul-Adha (Dhu al-Hijja 10) dates in Gregorian year @p year. */
        [[nodiscard]] std::vector<fixed_date_t> eid_ul_adha(int32_t year) const;
    };
}
