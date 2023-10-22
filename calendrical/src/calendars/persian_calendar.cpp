#include "calendars/persian_calendar.hpp"
#include "time_and_astronomy.hpp"
#include "season.hpp"

namespace calendrical::calendars {
    using namespace calendrical::time_and_astronomy;

    moment_t persian_calendar::midday_in_tehran(const fixed_date_t& date) const
    {
        return midday(date, tehran);
    }
    fixed_date_t persian_calendar::new_year_on_or_before(const fixed_date_t& date) const
    {
        auto approx = estimate_prior_solar_longitude(season::SPRING, midday_in_tehran(date));
        return least_passing([=](int64_t day) { return solar_longitude(midday_in_tehran(day) <= angle_t{ season::SPRING } + 2_deg); }, static_cast<int64_t>(std::floor(approx)) - 1);
    }
    fixed_date_t persian_calendar::get_new_year(int32_t year) const
    {
        int64_t beg = m_epoch + 180 + static_cast<int64_t>(std::floor(mean_tropical_year * (0 < year ? year - 1 : year)));
        return new_year_on_or_before(beg);
    }
    int32_t persian_calendar::get_year(const fixed_date_t& date) const
    {
        auto new_year = new_year_on_or_before(date);
        auto y = std::round((new_year - m_epoch) / mean_tropical_year) + 1;
        auto year = 0 < y ? y : y - 1;
        return static_cast<int32_t>(year);
    }
    fixed_date_t persian_calendar::to_fixed(const simple_date& date) const
    {
        auto [year, month, day] = date.tie();
        auto new_year = get_new_year(year);
        return new_year - 1
            + (month <= 7 ? 31 * (month - 1) : 30 * (month - 1) + 6)
            + day;
    }
}