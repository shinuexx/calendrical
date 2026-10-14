#pragma once

#include "datetypes/roman_date.hpp"
#include "julian_calendar.hpp"

namespace calendrical::calendars {
    /** @brief Alias for the Roman date type handled by @ref roman_calendar. */
    using roman_date = datetypes::roman_date;
    /** @brief Alias for Olympiad cycle/year notation. */
    using olympiad = datetypes::olympiad;

    /**
     * @brief Roman calendar conversion API built on the Julian calendar.
     */
    class roman_calendar :
    public typed_calendar_if<roman_date>
    {
        /** @brief Julian calendar the Roman nomenclature is layered on. */
        julian_calendar m_julian;

    public:
        /** @brief Constructs a Roman calendar instance. */
        roman_calendar();

        /** @brief Returns the Julian/Roman year containing @p date. */
        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        /** @brief Returns whether the year is leap under Julian rules. */
        [[nodiscard]] bool is_leap_year(int32_t year) const;

        /** @brief Returns the Roman calendar epoch (delegates to Julian epoch). */
        [[nodiscard]] fixed_date_t epoch() const override { return m_julian.epoch(); }

        /** @brief Returns the Roman event (Kalends/Nones/Ides) for @p date. */
        [[nodiscard]] roman_event get_event(const fixed_date_t& date) const;
        /** @brief Returns the inclusive backward count to the Roman event for @p date. */
        [[nodiscard]] int32_t get_count(const fixed_date_t& date) const;
        /** @brief Returns the Roman leap-day flag for @p date. */
        [[nodiscard]] bool get_leap(const fixed_date_t& date) const;

        /** @brief Converts a fixed date to Roman calendar notation. */
        [[nodiscard]] roman_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts Roman calendar notation to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const roman_date& date) const override;

        /** @brief Returns the Roman/Julian month containing @p date. */
        [[nodiscard]] month get_month(const fixed_date_t& date) const;

        /** @brief AUC offset: founding year of Rome in Julian year numbering. */
        static constexpr int32_t year_rome_founded = -753;

        /** @brief Converts AUC year to Julian year numbering. */
        [[nodiscard]] int32_t julian_year_from_auc(int32_t year) const;
        /** @brief Converts Julian year numbering to AUC year. */
        [[nodiscard]] int32_t auc_year_from_julian(int32_t year) const;

        /** @brief First Olympiad year in Julian year numbering. */
        static constexpr int32_t olympiad_start = -776;

        /** @brief Converts an Olympiad designation to Julian year numbering. */
        [[nodiscard]] int32_t julian_year_from_olympiad(const olympiad& odate) const;
        /** @brief Converts Julian year numbering to Olympiad designation. */
        [[nodiscard]] olympiad olympiad_from_julian_year(int32_t jyear) const;

    private:
        /** @brief Ides day number for a given month. */
        [[nodiscard]] int32_t ides_of_month(month m) const;
        /** @brief Nones day number for a given month. */
        [[nodiscard]] int32_t nones_of_month(month m) const;
    };
}
