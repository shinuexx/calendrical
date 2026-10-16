#pragma once

#include "hindu_base.hpp"
#include "location.hpp"

namespace calendrical::calendars{
    /**
     * @brief Shared astronomical algorithms for modern/astronomical Hindu calendars.
     */
    class modern_hindu_base : public hindu_base {
    public:
        /** @brief Ujjain reference location traditionally used in Hindu astronomy. */
        static constexpr location ujjain{
            angle_t::dms(23,9),
            angle_t::dms(75,46,6),
            0,
            moment_t::hms(5 + 461/9e3)
        };

        /** @brief Active location used by this calendar instance. */
        location m_hindu_location;

        /** @brief Constructs with an explicit reference location. */
        explicit modern_hindu_base(const location& hindu_location) : m_hindu_location(hindu_location) {}
        /** @brief Constructs using the default Ujjain reference location. */
        modern_hindu_base() : modern_hindu_base(ujjain) {}
        ~modern_hindu_base() override = default;

        /** @brief Hindu sidereal year length (days). */
        static constexpr double hindu_sidereal_year{(365 * 108e4 + 279457) / 108e4};
        /** @brief Hindu sidereal month length (days). */
        static constexpr double hindu_sidereal_month{(27 * 14438334.0 + 4644439) / 14438334};
        /** @brief Hindu synodic month length (days). */
        static constexpr double hindu_synodic_month{(29 * 13358334.0 + 7087771)/ 13358334};

        /** @brief Theoretical creation epoch used in traditional calculations. */
        static constexpr double hindu_creation{ms_epoch.value() - 1955880000 * hindu_sidereal_year};

        /** @brief Hindu anomalistic year length (days). */
        static constexpr double hindu_anomalistic_year{1577917828000.0 / (4320000000.0 - 387)};
        /** @brief Hindu anomalistic month length (days). */
        static constexpr double hindu_anomalistic_month{1577917828.0 / (57753336.0 - 488199)};

        /** @brief Entry from the traditional Hindu sine table. */
        [[nodiscard]] static double hindu_sine_table(int64_t entry);
        /** @brief Interpolated Hindu sine for an angle. */
        [[nodiscard]] static double hindu_sine(const angle_t& O);

        /** @brief Inverse of the Hindu sine approximation. */
        [[nodiscard]] static angle_t hindu_arcsin(const double& amp);

        /** @brief Mean position on a cycle with period @p period. */
        [[nodiscard]] static angle_t hindu_mean_position(const moment_t& t, const double& period);

        /** @brief True position with anomaly correction in the Hindu model. */
        [[nodiscard]] static angle_t hindu_true_position(const moment_t& t, const double& period, const double& size, const double& anomalistic, const double& change);

        /** @brief Hindu model solar longitude. */
        [[nodiscard]] static angle_t hindu_solar_longitude(const moment_t& t);

        /** @brief Zodiac sign number from Hindu solar longitude. */
        [[nodiscard]] static int32_t hindu_zodiac(const moment_t& t);

        /** @brief Hindu model lunar longitude. */
        [[nodiscard]] static angle_t hindu_lunar_longitude(const moment_t& t);
        /** @brief Hindu model lunar phase angle. */
        [[nodiscard]] static angle_t hindu_lunar_phase(const moment_t& t);

        /** @brief Tithi number from moment in the Hindu model. */
        [[nodiscard]] static int32_t hindu_lunar_day_from_moment(const moment_t& t);

        /** @brief New moon before a moment in the Hindu model. */
        [[nodiscard]] static moment_t hindu_new_moon_before(const moment_t& t);

        /** @brief Hindu calendar year derived from solar position at moment @p t. */
        [[nodiscard]] static int32_t hindu_calendar_year(const moment_t& t);

        /** @brief Saka-era offset used by Hindu solar calendars. */
        static constexpr auto hindu_solar_era{ 3179 };
        /** @brief Vikrama-like lunar-era offset used by Hindu lunar calendars. */
        static constexpr auto hindu_lunar_era{ 3044 };

        /** @brief Ascensional difference for date/location in Hindu astronomy. */
        [[nodiscard]] static angle_t hindu_ascensional_difference(const fixed_date_t& date, const location& location);
        /** @brief Tropical solar longitude on a fixed date in the Hindu model. */
        [[nodiscard]] static angle_t hindu_tropical_longitude(const fixed_date_t& date);

        /** @brief Tropical-vs-sidereal solar longitude difference. */
        [[nodiscard]] static double hindu_solar_sidereal_difference(const fixed_date_t& date);
        /** @brief Daily solar motion approximation. */
        [[nodiscard]] static double hindu_daily_motion(const fixed_date_t& date);
        /** @brief Rising-sign factor used in sunrise/sunset approximations. */
        [[nodiscard]] static double hindu_rising_sign(const fixed_date_t& date);

        /** @brief Hindu equation-of-time approximation. */
        [[nodiscard]] static double hindu_equation_of_time(const fixed_date_t& date);

        /** @brief Standard Hindu sunrise for the configured location. */
        [[nodiscard]] virtual moment_t hindu_sunrise(const fixed_date_t& date) const;
        /** @brief Standard Hindu sunset for the configured location. */
        [[nodiscard]] virtual moment_t hindu_sunset(const fixed_date_t& date) const;

        /** @brief Converts sundial-based local time to standard time. */
        [[nodiscard]] virtual moment_t hindu_standard_from_sundial(const moment_t& t) const;

        /** @brief Alternate sunrise definition used by some variants. */
        [[nodiscard]] virtual moment_t alt_hindu_sunrise(const fixed_date_t& date) const;

        /** @brief Ayanamsha (precession offset) used for sidereal calculations. */
        [[nodiscard]] static angle_t ayanamsha(const moment_t& t);

        /** @brief Astronomical Hindu sunset using the library astronomy module. */
        [[nodiscard]] virtual moment_t astro_hindu_sunset(const fixed_date_t& date) const;
        /** @brief Sidereal zodiac sign number at moment @p t. */
        [[nodiscard]] static int32_t sidereal_zodiac(const moment_t& t);
        /** @brief Astronomical Hindu calendar year at moment @p t. */
        [[nodiscard]] static int32_t astro_hindu_calendar_year(const moment_t& t);
        /** @brief Astronomical tithi number from moment @p t. */
        [[nodiscard]] static int32_t astro_hindu_lunar_day_from_moment(const moment_t& t);

        /** @brief First moment on or after @p t with solar longitude @p l. */
        [[nodiscard]] static moment_t hindu_solar_longitude_at_or_after(const angle_t& l, const moment_t& t);

        /** @brief Mesha samkranti moment in Gregorian year @p gyear. */
        [[nodiscard]] static moment_t mesha_samkranti(int32_t gyear);

        /** @brief First moment on or after @p t with lunar day index @p k. */
        [[nodiscard]] static moment_t hindu_lunar_day_at_or_after(const double& k, const moment_t& t);

        /** @brief Hindu lunar New Year date in Gregorian year @p gyear. */
        [[nodiscard]] virtual fixed_date_t hindu_lunar_new_year(int32_t gyear) const;

        /** @brief Nakshatra/station index for a fixed date. */
        [[nodiscard]] virtual int32_t hindu_lunar_station(const fixed_date_t& date) const;

        /** @brief Yoga index for a fixed date. */
        [[nodiscard]] static int32_t yoga(const fixed_date_t& date);
    };
}
