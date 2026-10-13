#include "calendars/observational_hebrew_calendar.hpp"
#include "time_and_astronomy.hpp"
#include "season.hpp"
#include <cmath>

namespace calendrical::calendars{
    using namespace calendrical::time_and_astronomy;

    observational_hebrew_calendar::observational_hebrew_calendar() {
        m_gregorian = std::addressof(gregorian_calendar::standard);
        m_hebrew = std::make_unique<hebrew_calendar>();
    }

    fixed_date_t observational_hebrew_calendar::first_of_nisan(int32_t gyear) {
        const auto equinox = season_in_gregorian(season::spring, gyear);
        const auto set = universal_from_standard(sunset(fixed_date_t{ static_cast<int64_t>(std::floor(equinox)) }, hebrew_location),
                                           hebrew_location);
        return phasis_on_or_after(fixed_date_t{ static_cast<int64_t>(std::floor(equinox) - (equinox < set ? 14 : 13)) },
                                  hebrew_location);
    }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    fixed_date_t observational_hebrew_calendar::get_crescent(const fixed_date_t& date) const // NOLINT(*-convert-member-functions-to-static)
    {
        return phasis_on_or_before(date, hebrew_location);
    }
    fixed_date_t observational_hebrew_calendar::get_new_year(int32_t year) const
    {
        return to_fixed({ year, hebrew_month::nisan, 1 });
    }
    hebrew_date observational_hebrew_calendar::to_date(const fixed_date_t& date) const
    {
        const auto crescent = get_crescent(date);
        const auto gyear = m_gregorian->get_year(date);
        const auto ny = first_of_nisan(gyear);
        const auto new_year = date < ny ? first_of_nisan(gyear - 1) : ny;
        auto month = static_cast<hebrew_month>(static_cast<int32_t>(std::round(static_cast<double>(crescent - new_year) / 29.5) + 1)) ;
        auto year = m_hebrew->get_year(new_year) + (month >= hebrew_month::tishri ? 1 : 0);
        const auto day = date - crescent + 1;
        return { year, month, static_cast<int32_t>(day) };
    }
    fixed_date_t observational_hebrew_calendar::to_fixed(const hebrew_date& date) const
    {
        auto [year, month, day] = date;
        auto year1 = month >= hebrew_month::tishri ? year - 1 : year;
        const auto start = m_hebrew->to_fixed({year1, hebrew_month::nisan, 1});
        const auto gyear = m_gregorian->get_year(start.add(60));
        const auto new_year = first_of_nisan(gyear);
        const auto midmonth = new_year + static_cast<int64_t>(std::round(29.5 * (static_cast<int32_t>(month) - 1))) + 15;
        return phasis_on_or_before(midmonth, hebrew_location) + day - 1;
    }
    int32_t observational_hebrew_calendar::get_year(const fixed_date_t& date) const
    {
        return to_date(date).year;
    }
    hebrew_month observational_hebrew_calendar::get_month(const fixed_date_t& date) const
    {
        return to_date(date).month;
    }
    int32_t observational_hebrew_calendar::get_day(const fixed_date_t& date) const
    {
        return to_date(date).day;
    }
    bool observational_hebrew_calendar::is_leap_year(int32_t year) const { return m_hebrew->is_leap_year(year); }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    fixed_date_t observational_hebrew_calendar::classical_passover_eve(int32_t gyear) const // NOLINT(*-convert-member-functions-to-static)
    {
        return first_of_nisan(gyear).add(13);
    }
}