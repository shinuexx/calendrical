#pragma once

#include "hindu_base.hpp"
#include "location.hpp"

namespace calendrical::calendars{
    class modern_hindu_base : public hindu_base {
    public:
        static constexpr location ujjain{
            angle_t::dms(23,9),
            angle_t::dms(75,46,6),
            0,
            moment_t::hms(5 + 461/9e3)
        };

        location m_hindu_location;

        explicit modern_hindu_base(const location& hindu_location) : m_hindu_location(hindu_location) {}
        modern_hindu_base() : modern_hindu_base(ujjain) {}
        ~modern_hindu_base() override = default;

        static constexpr double hindu_sidereal_year{(365 * 108e4 + 279457) / 108e4};
        static constexpr double hindu_sidereal_month{(27 * 14438334.0 + 4644439) / 14438334};
        static constexpr double hindu_synodic_month{(29 * 13358334.0 + 7087771)/ 13358334};

        static constexpr double hindu_creation{static_cast<int64_t>(ms_epoch) - 1955880000 * hindu_sidereal_year};

        static constexpr double hindu_anomalistic_year{1577917828000.0 / (4320000000.0 - 387)};
        static constexpr double hindu_anomalistic_month{1577917828.0 / (57753336.0 - 488199)};

        [[nodiscard]] static double hindu_sine_table(int64_t entry);
        [[nodiscard]] static double hindu_sine(const angle_t& O);

        [[nodiscard]] static angle_t hindu_arcsin(const double& amp);

        [[nodiscard]] static angle_t hindu_mean_position(const moment_t& t, const double& period);

        [[nodiscard]] static angle_t hindu_true_position(const moment_t& t, const double& period, const double& size, const double& anomalistic, const double& change);

        [[nodiscard]] static angle_t hindu_solar_longitude(const moment_t& t);

        [[nodiscard]] static int32_t hindu_zodiac(const moment_t& t);

        [[nodiscard]] static angle_t hindu_lunar_longitude(const moment_t& t);
        [[nodiscard]] static angle_t hindu_lunar_phase(const moment_t& t);

        [[nodiscard]] static int32_t hindu_lunar_day_from_moment(const moment_t& t);

        [[nodiscard]] static moment_t hindu_new_moon_before(const moment_t& t);

        [[nodiscard]] static int32_t hindu_calendar_year(const moment_t& t);

        static constexpr auto hindu_solar_era{ 3179 };
        static constexpr auto hindu_lunar_era{ 3044 };

        [[nodiscard]] static angle_t hindu_ascensional_difference(const fixed_date_t& date, const location& location);
        [[nodiscard]] static angle_t hindu_tropical_longitude(const fixed_date_t& date);

        [[nodiscard]] static double hindu_solar_sidereal_difference(const fixed_date_t& date);
        [[nodiscard]] static double hindu_daily_motion(const fixed_date_t& date);
        [[nodiscard]] static double hindu_rising_sign(const fixed_date_t& date);

        [[nodiscard]] static double hindu_equation_of_time(const fixed_date_t& date);

        [[nodiscard]] virtual moment_t hindu_sunrise(const fixed_date_t& date) const;
        [[nodiscard]] virtual moment_t hindu_sunset(const fixed_date_t& date) const;

        [[nodiscard]] virtual moment_t hindu_standard_from_sundial(const moment_t& t) const;

        [[nodiscard]] virtual moment_t alt_hindu_sunrise(const fixed_date_t& date) const;

        [[nodiscard]] static angle_t ayanamsha(const moment_t& t);

        [[nodiscard]] virtual moment_t astro_hindu_sunset(const fixed_date_t& date) const;
        [[nodiscard]] static int32_t sidereal_zodiac(const moment_t& t);
        [[nodiscard]] static int32_t astro_hindu_calendar_year(const moment_t& t);
        [[nodiscard]] static int32_t astro_hindu_lunar_day_from_moment(const moment_t& t);

        [[nodiscard]] static moment_t hindu_solar_longitude_at_or_after(const angle_t& l, const moment_t& t);

        [[nodiscard]] static moment_t mesha_samkranti(int32_t gyear);

        [[nodiscard]] static moment_t hindu_lunar_day_at_or_after(const double& k, const moment_t& t);

        [[nodiscard]] virtual fixed_date_t hindu_lunar_new_year(int32_t gyear) const;

        [[nodiscard]] virtual int32_t hindu_lunar_station(const fixed_date_t& date) const;

        [[nodiscard]] static int32_t yoga(const fixed_date_t& date);
    };
}