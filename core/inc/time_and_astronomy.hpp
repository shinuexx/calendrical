#pragma once

#include "location.hpp"
#include "lazy.hpp"

namespace calendrical::time_and_astronomy {
    /** @brief Converts local mean solar time to universal time using longitude. */
    static constexpr moment_t universal_from_local(const moment_t& t, const location& location) {
        return t - location.get_longitude_zone();
    }
    /** @brief Converts universal time to local mean solar time using longitude. */
    static constexpr moment_t local_from_universal(const moment_t& t, const location& location) {
        return t + location.get_longitude_zone();
    }
    /** @brief Converts universal time to local standard time using time zone offset. */
    static constexpr moment_t standard_from_universal(const moment_t& t, const location& location) {
        return t + location.zone;
    }
    /** @brief Converts local standard time to universal time using time zone offset. */
    static constexpr moment_t universal_from_standard(const moment_t& t, const location& location) {
        return t - location.zone;
    }

    /** @brief Converts local mean solar time directly to local standard time. */
    static constexpr moment_t standard_from_local(const moment_t& t, const location& location) {
        return standard_from_universal(universal_from_local(t, location), location);
    }
    /** @brief Converts local standard time directly to local mean solar time. */
    static constexpr moment_t local_from_standard(const moment_t& t, const location& location) {
        return local_from_universal(universal_from_standard(t, location), location);
    }

    /** @brief Delta-T style ephemeris correction (dynamical minus universal time). */
    moment_t ephemeris_correction(const moment_t& t);

    //const extern moment_t J2000;
    /** @brief Lazily initialized J2000 epoch moment (`2000-01-01 12:00 TT`-based reference). */
    const extern lazy<moment_t> J2000;

    /** @brief Converts universal time to dynamical time. */
    moment_t dynamic_from_universal(const moment_t& t);
    /** @brief Converts dynamical time to universal time. */
    moment_t universal_from_dynamic(const moment_t& t);

    /** @brief Julian centuries elapsed since @ref J2000 in dynamical time. */
    double julian_centuries(const moment_t& t);

    /** @brief Equation of time (apparent solar time minus mean solar time). */
    moment_t equation_of_time(const moment_t& t);

    /** @brief Converts local mean solar time to apparent solar time. */
    moment_t apparent_from_local(const moment_t& t, const location& location);
    /** @brief Converts apparent solar time to local mean solar time. */
    moment_t local_from_apparent(const moment_t& t, const location& location);

    /** @brief Converts universal time to apparent solar time at a location. */
    moment_t apparent_from_universal(const moment_t& t, const location& location);
    /** @brief Converts apparent solar time at a location to universal time. */
    moment_t universal_from_apparent(const moment_t& t, const location& location);

    /** @brief Apparent midnight at a location on the given fixed date. */
    moment_t midnight(const fixed_date_t& date, const location& location);
    /** @brief Apparent midday (local apparent noon) at a location on the given fixed date. */
    moment_t midday(const fixed_date_t& date, const location& location);

    /** @brief Greenwich apparent sidereal time angle for a moment. */
    angle_t sidereal_from_moment(const moment_t& t);

    /** @brief Mean obliquity of the ecliptic. */
    angle_t obliquity(const moment_t& t);
    /** @brief Ecliptic-to-equatorial declination for latitude/longitude on date @p t. */
    angle_t declination(const moment_t& t, const angle_t& b, const angle_t& l);
    /** @brief Ecliptic-to-equatorial right ascension for latitude/longitude on date @p t. */
    angle_t right_ascension(const moment_t& t, const angle_t& b, const angle_t& l);

    /** @brief Apparent solar ecliptic longitude. */
    angle_t solar_longitude(const moment_t& t);

    /** @brief Nutation correction component used in solar longitude calculations. */
    angle_t nutation(const moment_t& t);
    /** @brief Aberration correction component used in solar longitude calculations. */
    angle_t aberration(const moment_t& t);

    /** @brief Mean tropical year length in days. */
    static constexpr auto mean_tropical_year{ 365.242189 };
    /** @brief Mean sidereal year length in days. */
    static constexpr auto mean_sidereal_year{ 365.25636 };

    /** @brief Reference sidereal solar longitude offset (degrees). */
    static constexpr angle_t sidereal_start{ 336.136058652892810 }; //148.66025426221921 // 156.13605090692624

    /** @brief Mean synodic month length in days. */
    static constexpr auto mean_synodic_month{ 29.530588861 };

    /** @brief First moment after @p t when solar longitude reaches @p l. */
    moment_t solar_longitude_after(const angle_t& l, const moment_t& t);
    /** @brief Moment of a seasonal solar longitude crossing in Gregorian year @p year. */
    moment_t season_in_gregorian(const angle_t& season, int32_t year);
    /** @brief Convenience helper returning the Urbana winter solstice moment for a Gregorian year. */
    moment_t urbana_winter(int32_t gyear);
    /** @brief General precession angle. */
    angle_t precession(const moment_t& t);
    /** @brief Sidereal solar longitude (tropical longitude adjusted by ayanamsha). */
    angle_t sidereal_solar_longitude(const moment_t& t);
    /** @brief Solar altitude above horizon for a location and moment. */
    angle_t solar_altitude(const moment_t& t, const location& location);
    /** @brief Backward estimate for when solar longitude last equaled @p l before @p t. */
    moment_t estimate_prior_solar_longitude(const angle_t& l, const moment_t& t);

    /** @brief n-th mean/true new moon moment used by the library's lunar model. */
    moment_t nth_new_moon(int64_t n);

    /** @brief Latest new moon strictly before @p t. */
    moment_t new_moon_before(const moment_t& t);
    /** @brief Earliest new moon on or after @p t. */
    moment_t new_moon_on_or_after(const moment_t& t);

    /** @brief Apparent lunar ecliptic longitude. */
    angle_t lunar_longitude(const moment_t& t);

    /** @brief Mean longitude of the lunar node as a function of Julian centuries. */
    angle_t moon_node(double c);
    /** @brief Mean lunar anomaly as a function of Julian centuries. */
    angle_t lunar_anomaly(double c);
    /** @brief Mean solar anomaly as a function of Julian centuries. */
    angle_t solar_anomaly(double c);
    /** @brief Mean lunar elongation as a function of Julian centuries. */
    angle_t lunar_elongation(double c);
    /** @brief Mean lunar longitude as a function of Julian centuries. */
    angle_t mean_lunar_longitude(double c);

    /** @brief Lunar node longitude on the given fixed date. */
    angle_t lunar_node(const fixed_date_t& date);

    /** @brief Sidereal lunar longitude. */
    angle_t sidereal_lunar_longitude(const moment_t& t);

    /** @brief Lunar phase angle (Moon-Sun elongation). */
    angle_t lunar_phase(const moment_t& t);

    /** @brief Most recent time at or before @p t when lunar phase equals @p psi. */
    moment_t lunar_phase_at_or_before(const angle_t& psi, const moment_t& t);
    /** @brief Earliest time at or after @p t when lunar phase equals @p psi. */
    moment_t lunar_phase_at_or_after(const angle_t& psi, const moment_t& t);

    /** @brief Apparent lunar ecliptic latitude. */
    angle_t lunar_latitude(const moment_t& t);
    /** @brief Geocentric lunar altitude for a location and moment. */
    angle_t lunar_altitude(const moment_t& t, const location& location);

    /** @brief Earth-Moon distance in kilometers (library lunar model output). */
    double lunar_distance(const moment_t& t);

    /** @brief Lunar parallax angle for a location and moment. */
    angle_t lunar_parallax(const moment_t& t, const location& location);

    /** @brief Topocentric lunar altitude corrected for parallax. */
    angle_t topocentric_lunar_altitude(const moment_t& t, const location& location);

    /** @brief Approximate time when the Sun reaches depression angle @p a. */
    moment_t approx_moment_of_depression(const moment_t& t, const location& location, const angle_t& a, bool early);

    /** @brief Auxiliary sine expression used in depression-time calculations. */
    double sine_offset(const moment_t& t, const location& location, const angle_t& a);

    /** @brief Refined time when the Sun reaches depression angle @p a. */
    moment_t moment_of_depression(const moment_t& approx, const location& location, const angle_t& a, bool early);

    /** @brief Boolean flag selecting morning branch of rise/depression calculations. */
    static constexpr auto morning{ true };
    /** @brief Boolean flag selecting evening branch of rise/depression calculations. */
    static constexpr auto evening{ false };

    /** @brief Dawn time when solar depression equals @p a. */
    moment_t dawn(const fixed_date_t& date, const location& location, const angle_t& a);
    /** @brief Dusk time when solar depression equals @p a. */
    moment_t dusk(const fixed_date_t& date, const location& location, const angle_t& a);

    /** @brief Atmospheric refraction correction near the horizon. */
    angle_t refraction(const moment_t& t, const location& location);

    /** @brief Sunrise time for a date and location. */
    moment_t sunrise(const fixed_date_t& date, const location& location);
    /** @brief Sunset time for a date and location. */
    moment_t sunset(const fixed_date_t& date, const location& location);

    /** @brief End of Jewish Sabbath based on twilight criterion used by the library. */
    moment_t jewish_sabbath_ends(const fixed_date_t& date, const location& location);
    /** @brief Jewish dusk/twilight helper used by calendar rules. */
    moment_t jewish_dusk(const fixed_date_t& date, const location& location);

    /** @brief Observed lunar altitude (topocentric altitude with observational adjustments). */
    angle_t observed_lunar_altitude(const moment_t& t, const location& location);

    /** @brief Moonrise time for a date and location. */
    moment_t moonrise(const fixed_date_t& date, const location& location);
    /** @brief Moonset time for a date and location. */
    moment_t moonset(const fixed_date_t& date, const location& location);

    /** @brief Local zero-hour reference used by Italian-hour conversions. */
    moment_t local_zero_hour(const moment_t& t);
    /** @brief Converts Italian hours to local time. */
    moment_t local_from_italian(const moment_t& t);
    /** @brief Converts local time to Italian-hour time. */
    moment_t italian_from_local(const moment_t& t);

    /** @brief Daytime temporal hour length (sunrise to sunset divided by 12). */
    moment_t daytime_temporal_hour(const fixed_date_t& date, const location& location);
    /** @brief Nighttime temporal hour length (sunset to sunrise divided by 12). */
    moment_t nighttime_temporal_hour(const fixed_date_t& date, const location& location);

    /** @brief Converts a sundial-style local time to standard time. */
    moment_t standard_from_sundial(const moment_t& t, const location& location);

    /** @brief End of the Jewish morning prayer window (library criterion). */
    moment_t jewish_morning_end(const fixed_date_t& date, const location& location);

    /** @brief Asr prayer time (standard juristic rule used by the library). */
    moment_t asr(const fixed_date_t& date, const location& location);
    /** @brief Alternative Asr prayer time (alternate juristic rule). */
    moment_t alt_asr(const fixed_date_t& date, const location& location);

    /** @brief Arc of light visibility parameter for crescent visibility criteria. */
    angle_t arc_of_light(const moment_t& t);

    /** @brief Simple best-view time for crescent visibility observations. */
    moment_t simple_best_view(const fixed_date_t& date, const location& location);
    /** @brief Shaukat crescent-visibility criterion. */
    bool shaukat_criterion(const fixed_date_t& date, const location& location);

    /** @brief Argument of vision for crescent visibility models. */
    angle_t arg_of_vision(const moment_t& t, const location& location);
    /** @brief Bruin best-view time for crescent visibility observations. */
    moment_t bruin_best_view(const fixed_date_t& date, const location& location);

    /** @brief Yallop crescent-visibility criterion. */
    bool yallop_criterion(const fixed_date_t& date, const location& location);

    /** @brief Apparent lunar semidiameter. */
    double lunar_semi_diameter(const moment_t& t, const location& location);

    /** @brief Apparent angular lunar diameter. */
    angle_t lunar_diameter(const moment_t& t);

    /** @brief Default crescent-visibility predicate alias used by observational calendars. */
    static constexpr auto visible_crescent = shaukat_criterion;

    /** @brief Most recent date on or before @p date with first visible crescent (phasis). */
    fixed_date_t phasis_on_or_before(const fixed_date_t& date, const location& location);
    /** @brief Earliest date on or after @p date with first visible crescent (phasis). */
    fixed_date_t phasis_on_or_after(const fixed_date_t& date, const location& location);

    /** @brief Delay between sunset and moonset on the given date. */
    moment_t moonlag(const fixed_date_t& date, const location& location);
    /** @brief Observed lunar month length around the given date/location context. */
    int32_t month_length(const fixed_date_t& date, const location& location);
    /** @brief Returns whether the observed month is classified as “early”. */
    bool is_early_month(const fixed_date_t& date, const location& location);
}
