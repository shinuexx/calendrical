#pragma once

#include "hindu_base.hpp"
#include "location.hpp"

namespace calendrical::calendars{
    class modern_hindu_base : public hindu_base {
    public:
        static const rational_location ujjain;

        rational_location m_hindu_location;

        modern_hindu_base(const rational_location& hindu_location) : m_hindu_location(hindu_location) {}
        modern_hindu_base() : modern_hindu_base(ujjain) {}
        ~modern_hindu_base() override = default;

        static const rational hindu_sidereal_year;
        static const rational hindu_sidereal_month;
        static const rational hindu_synodic_month;

        static const rational hindu_creation;

        static const rational hindu_anomalistic_year;
        static const rational hindu_anomalistic_month;

        [[nodiscard]] static rational hindu_sine_table(int64_t entry);
        [[nodiscard]] static rational hindu_sine(const rational_angle_t& O);

        [[nodiscard]] static rational_angle_t hindu_arcsin(const rational& amp);

        [[nodiscard]] static rational_angle_t hindu_mean_position(const rational_moment_t& t, const rational& period);

        [[nodiscard]] static rational_angle_t hindu_true_position(const rational_moment_t& t, const rational& period, const rational& size, const rational& anomalistic, const rational& change);

        [[nodiscard]] static rational_angle_t hindu_solar_longitude(const rational_moment_t& t);

        [[nodiscard]] static int32_t hindu_zodiac(const rational_moment_t& t);

        [[nodiscard]] static rational_angle_t hindu_lunar_longitude(const rational_moment_t& t);
        [[nodiscard]] static rational_angle_t hindu_lunar_phase(const rational_moment_t& t);

        [[nodiscard]] static int32_t hindu_lunar_day_from_moment(const rational_moment_t& t);

        [[nodiscard]] static rational_moment_t hindu_new_moon_before(const rational_moment_t& t);

        [[nodiscard]] static int32_t hindu_calendar_year(const rational_moment_t& t);

        static constexpr auto hindu_solar_era{ 3179 };
        static constexpr auto hindu_lunar_era{ 3044 };

        [[nodiscard]] static rational_angle_t hindu_ascensional_difference(const fixed_date_t& date, const rational_location& location);
        [[nodiscard]] static rational_angle_t hindu_tropical_longitude(const fixed_date_t& date);

        [[nodiscard]] static rational hindu_solar_sidereal_difference(const fixed_date_t& date);
        [[nodiscard]] static rational hindu_daily_motion(const fixed_date_t& date);
        [[nodiscard]] static rational hindu_rising_sign(const fixed_date_t& date);

        [[nodiscard]] static rational hindu_equation_of_time(const fixed_date_t& date);

        [[nodiscard]] virtual rational_moment_t hindu_sunrise(const fixed_date_t& date) const;
        [[nodiscard]] virtual rational_moment_t hindu_sunset(const fixed_date_t& date) const;

        [[nodiscard]] virtual rational_moment_t hindu_standard_from_sundial(const rational_moment_t& t) const;

        [[nodiscard]] virtual rational_moment_t alt_hindu_sunrise(const fixed_date_t& date) const;

        [[nodiscard]] static angle_t ayanamsha(const moment_t& t);

        [[nodiscard]] virtual moment_t astro_hindu_sunset(const fixed_date_t& date) const;
        [[nodiscard]] static int32_t sidereal_zodiac(const moment_t& t);
        [[nodiscard]] static int32_t astro_hindu_calendar_year(const moment_t& t);
        [[nodiscard]] static int32_t astro_hindu_lunar_day_from_moment(const moment_t& t);

        [[nodiscard]] static rational_moment_t hindu_solar_longitude_at_or_after(const rational_angle_t& l, const rational_moment_t& t);

        [[nodiscard]] static rational_moment_t mesha_samkranti(int32_t gyear);

        [[nodiscard]] static rational_moment_t hindu_lunar_day_at_or_after(const rational& k, const rational_moment_t& t);

        [[nodiscard]] virtual fixed_date_t hindu_lunar_new_year(int32_t gyear) const;

        [[nodiscard]] virtual int32_t hindu_lunar_station(const fixed_date_t& date) const;

        [[nodiscard]] static int32_t yoga(const fixed_date_t& date);
    };
}