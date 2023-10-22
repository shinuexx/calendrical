#pragma once

#include "location.hpp"

namespace calendrical::time_and_astronomy {
    static constexpr moment_t universal_from_local(const moment_t& t, const location& location) {
        return t - location.get_longitude_zone();
    }
    static constexpr moment_t local_from_universal(const moment_t& t, const location& location) {
        return t + location.get_longitude_zone();
    }
    static constexpr moment_t standard_from_universal(const moment_t& t, const location& location) {
        return t + location.zone;
    }
    static constexpr moment_t universal_from_standard(const moment_t& t, const location& location) {
        return t - location.zone;
    }

    static constexpr moment_t standard_from_local(const moment_t& t, const location& location) {
        return standard_from_universal(universal_from_local(t, location), location);
    }
    static constexpr moment_t local_from_standard(const moment_t& t, const location& location) {
        return local_from_universal(universal_from_standard(t, location), location);
    }

    moment_t ephemeris_correction(const moment_t& t);

    const extern moment_t J2000;

    moment_t dynamic_from_universal(const moment_t& t);
    moment_t universal_from_dynamic(const moment_t& t);

    double julian_centuries(const moment_t& t);

    moment_t equation_of_time(const moment_t& t);

    moment_t apparent_from_local(const moment_t& t, const location& location);
    moment_t local_from_apparent(const moment_t& t, const location& location);

    moment_t apparent_from_universal(const moment_t& t, const location& location);
    moment_t universal_from_apparent(const moment_t& t, const location& location);

    moment_t midnight(const fixed_date_t& date, const location& location);
    moment_t midday(const fixed_date_t& date, const location& location);

    angle_t sidereal_from_moment(const moment_t& t);

    angle_t obliquity(const moment_t& t);
    angle_t declination(const moment_t& t, const angle_t& b, const angle_t& l);
    angle_t right_ascension(const moment_t& t, const angle_t& b, const angle_t& l);

    angle_t solar_longitude(const moment_t& t);

    angle_t nutation(const moment_t& t);
    angle_t abberation(const moment_t& t);
    angle_t abberation(const moment_t& t);

    static constexpr auto mean_tropical_year{ 365.242189 };
    static constexpr auto mean_sidereal_year{ 365.25636 };

    static constexpr angle_t sidereal_start{ 156.13605090692624 };

    static constexpr auto mean_synodic_month{ 29.530588861 };

    moment_t solar_longitude_after(const angle_t& l, const moment_t& t);
    moment_t season_in_gregorian(const angle_t& season, int32_t year);
    moment_t urbana_winter(int32_t gyear);
    angle_t precession(const moment_t& t);
    angle_t sidereal_solar_longitude(const moment_t& t);
    angle_t solar_altitude(const moment_t& t, const location& location);
    moment_t estimate_prior_solar_longitude(const angle_t& l, const moment_t& t);

    moment_t nth_new_moon(int64_t n);

    moment_t new_moon_before(const moment_t& t);
    moment_t new_moon_on_or_after(const moment_t& t);

    angle_t lunar_longitude(const moment_t& t);

    angle_t moon_node(double c);
    angle_t lunar_anomaly(double c);
    angle_t solar_anomaly(double c);
    angle_t lunar_elongation(double c);
    angle_t mean_lunar_longitude(double c);

    angle_t lunar_node(const fixed_date_t& date);

    angle_t sidereal_lunar_longitude(const moment_t& t);

    angle_t lunar_phase(const moment_t& t);

    moment_t lunar_phase_at_or_before(const angle_t& psi, const moment_t& t);
    moment_t lunar_phase_at_or_after(const angle_t& psi, const moment_t& t);

    angle_t lunar_latitude(const moment_t& t);
    angle_t lunar_altitude(const moment_t& t, const location& location);

    double lunar_distance(const moment_t& t);

    angle_t lunar_parallax(const moment_t& t, const location& location);

    angle_t topocentric_lunar_altitude(const moment_t& t, const location& location);

    moment_t approx_moment_of_depression(const moment_t& t, const location& location, const angle_t& a, bool early);
    
    double sine_offset(const moment_t& t, const location& location, const angle_t& a);

    moment_t moment_of_depression(const moment_t& approx, const location& location, const angle_t& a, bool early);

    static constexpr auto morning{ true };
    static constexpr auto evening{ false };

    moment_t dawn(const fixed_date_t& date, const location& location, const angle_t& a);
    moment_t dusk(const fixed_date_t& date, const location& location, const angle_t& a);

    angle_t refraction(const moment_t& t, const location& location);

    moment_t sunrise(const fixed_date_t& date, const location& location);
    moment_t sunset(const fixed_date_t& date, const location& location);

    moment_t jewish_sabbath_ends(const fixed_date_t& date, const location& location);
    moment_t jewish_dusk(const fixed_date_t& date, const location& location);

    angle_t observed_lunar_altitude(const moment_t& t, const location& location);

    moment_t moonrise(const fixed_date_t& date, const location& location);
    moment_t moonset(const fixed_date_t& date, const location& location);

    moment_t local_zero_hour(const moment_t& t);
    moment_t local_from_italian(const moment_t& t);
    moment_t italian_from_local(const moment_t& t);

    moment_t daytime_temporal_hour(const fixed_date_t& date, const location& location);
    moment_t nighttime_temporal_hour(const fixed_date_t& date, const location& location);

    moment_t standard_from_sundial(const moment_t& t, const location& location);

    moment_t jewish_morning_end(const fixed_date_t& date, const location& location);

    moment_t asr(const fixed_date_t& date, const location& location);
    moment_t alt_asr(const fixed_date_t& date, const location& location);

    angle_t arc_of_light(const moment_t& t);

    moment_t simple_best_view(const fixed_date_t& date, const location& location);
    bool shaukat_criterion(const fixed_date_t& date, const location& location);

    angle_t arg_of_vision(const moment_t& t, const location& location);
    moment_t bruin_best_view(const fixed_date_t& date, const location& location);

    bool yallop_criterion(const fixed_date_t& date, const location& location);

    double lunar_semi_diameter(const moment_t& t, const location& location);

    angle_t lunar_diameter(const moment_t& t);

    static constexpr auto visible_crescent = shaukat_criterion;

    fixed_date_t phasis_on_or_before(const fixed_date_t& date, const location& location);
    fixed_date_t phasis_on_or_after(const fixed_date_t& date, const location& location);

    moment_t moonlag(const fixed_date_t& date, const location& location);
    int32_t month_length(const fixed_date_t& date, const location& location);
    bool is_early_month(const fixed_date_t& date, const location& location);
}