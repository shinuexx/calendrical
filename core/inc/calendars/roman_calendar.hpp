#pragma once

#include "datetypes/roman_date.hpp"
#include "julian_calendar.hpp"

namespace calendrical::calendars {
    using roman_date = datetypes::roman_date;
    using olympiad = datetypes::olympiad;

    class roman_calendar :
    public typed_calendar_if<roman_date>
    {
        julian_calendar m_julian;

    public:
        roman_calendar();

        [[nodiscard]] int32_t get_year(const fixed_date_t& date) const;
        [[nodiscard]] bool is_leap_year(int32_t year) const;

        [[nodiscard]] fixed_date_t epoch() const override { return m_julian.epoch(); }

        [[nodiscard]] roman_event get_event(const fixed_date_t& date) const;
        [[nodiscard]] int32_t get_count(const fixed_date_t& date) const;
        [[nodiscard]] bool get_leap(const fixed_date_t& date) const;

        [[nodiscard]] roman_date to_date(const fixed_date_t& date) const override;
        [[nodiscard]] fixed_date_t to_fixed(const roman_date& date) const override;

        [[nodiscard]] month get_month(const fixed_date_t& date) const;

        static constexpr int32_t year_rome_founded = -753;

        [[nodiscard]] int32_t julian_year_from_auc(int32_t year) const;
        [[nodiscard]] int32_t auc_year_from_julian(int32_t year) const;

        static constexpr int32_t olympiad_start = -776;

        [[nodiscard]] int32_t julian_year_from_olympiad(const olympiad& odate) const;
        [[nodiscard]] olympiad olympiad_from_julian_year(int32_t jyear) const;

    private:
        [[nodiscard]] int32_t ides_of_month(month m) const;
        [[nodiscard]] int32_t nones_of_month(month m) const;
    };
}