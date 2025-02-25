#include "calendars/arithmetic_persian_calendar.hpp"

namespace calendrical::calendars {
    bool arithmetic_persian_calendar::is_leap_year(int32_t pyear) const {
        const int64_t y = 0 < pyear ? pyear - 474ll : pyear - 473ll;
        const auto year = mod(y, 2820) + 474;
        return mod(31 * (year + 38), 128) < 31;
    }
    fixed_date_t arithmetic_persian_calendar::get_new_year(int32_t pyear) const
    {
        const int64_t y = 0 < pyear ? pyear - 474ll : pyear - 473ll;
        const auto year = mod(y, 2820) + 474;
        return ms_epoch
            + 1029983 * floor_div(y, 2820)
            + 365 * (year - 1)
            + floor_div(31 * year - 5, 128);
    }
    int32_t arithmetic_persian_calendar::get_year(const fixed_date_t& date) const
    {
        static const auto begin = to_fixed({475, 1, 1});
        const auto d0 = date - begin;
        const auto n2820 = floor_div(d0, 1029983);
        const auto d1 = mod(d0, 1029983);
        const auto y2820 = d1 == 1029982 ? 2820 : floor_div(128 * d1 + 46878, 46751);
        const auto year = 474 + 2820 * n2820 + y2820;
        return static_cast<int32_t>(0 < year ? year : year - 1);
    }
    fixed_date_t arithmetic_persian_calendar::to_fixed(const simple_date& date) const
    {
        auto &[pyear, month, day] = date;
        const int64_t y = 0 < pyear ? pyear - 474ll : pyear - 473ll;
        const auto year = mod(y, 2820) + 474;
        return ms_epoch - 1
            + 1029983 * floor_div(y, 2820)
            + 365 * (year - 1)
            + floor_div(31 * year - 5, 128)
            + (month <= 7 ? 31 * (month - 1ll) : 30 * (month - 1ll) + 6)
            + day;
    }
}