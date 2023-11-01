#include "calendars/modern_hindu_base.hpp"
#include "calendars/gregorian_calendar.hpp"
#include "angle.hpp"
#include "time_and_astronomy.hpp"

namespace calendrical::calendars {
    const rational_location modern_hindu_base::ujjain{
        rational_angle_t::dms(23,9),
        rational_angle_t::dms(75,46,6),
        0,
        rational_moment_t::hms(5 + rational{461,9000})
    };

    const rational modern_hindu_base::hindu_sidereal_year{ 365 * 1080000LL + 276457, 1080000LL };
    const rational modern_hindu_base::hindu_sidereal_month{ 27 * 14438334LL + 4644439, 14438334LL };
    const rational modern_hindu_base::hindu_synodic_month{ 29 * 13358334LL + 7087771, 13358334LL };
    const rational modern_hindu_base::hindu_creation{ (int64_t)hindu_base::m_epoch - 1955880000 * modern_hindu_base::hindu_sidereal_year };

    const rational modern_hindu_base::hindu_anomalistic_year{ 1577917828000LL, 4320000000LL - 387 };
    const rational modern_hindu_base::hindu_anomalistic_month{ 1577917828LL, 57753336LL - 488199 };

    rational modern_hindu_base::hindu_sine_table(int64_t entry)
    {
        auto exact = 3438 * sin(entry * angle_t::dms(0, 225));
        auto error = 0.215 * sign(exact) * sign(std::abs(exact) - 1716);
        return rational{ (int64_t)std::round(exact + error), 3438 };
    }

    rational modern_hindu_base::hindu_sine(const rational_angle_t& O)
    {
        auto entry = O / rational{ 226, 60 };
        auto fraction = mod(entry, 1);
        return fraction * hindu_sine_table((int64_t)ceil(entry)) + (1 - fraction) * hindu_sine_table((int64_t)floor(entry));
    }
    rational_angle_t modern_hindu_base::hindu_arcsin(const rational& amp)
    {
        auto pos = least_passing([&amp](int64_t k) { return amp <= hindu_sine_table(k); }, 0);
        auto below = hindu_sine_table(pos - 1);
        if (amp < 0)
            return -hindu_arcsin(-amp);
        else
            return rational{ 225, 60 } * (pos - 1 + (amp - below) / (hindu_sine_table(pos) - below));
    }
    rational_angle_t modern_hindu_base::hindu_mean_position(const rational_moment_t& t, const rational& period)
    {
        return 360 * mod((t - hindu_creation) / period, 1);
    }
    rational_angle_t modern_hindu_base::hindu_true_position(const rational_moment_t& t, const rational& period, const rational& size, const rational& anomalistic, const rational& change)
    {
        auto l = hindu_mean_position(t, period);
        auto offset = hindu_sine(hindu_mean_position(t, anomalistic));
        rational contraction = boost::multiprecision::abs(offset) * change * size;
        auto equation = hindu_arcsin(offset * (size - contraction));
        return mod(l - equation, 360);
    }
    rational_angle_t modern_hindu_base::hindu_solar_longitude(const rational_moment_t& t)
    {
        return hindu_true_position(t, hindu_sidereal_year, { 14,360 }, hindu_anomalistic_year, { 1,42 });
    }

    int32_t modern_hindu_base::hindu_zodiac(const rational_moment_t& t)
    {
        return (int32_t)floor_div(hindu_solar_longitude(t), 30) + 1;
    }

    rational_angle_t modern_hindu_base::hindu_lunar_longitude(const rational_moment_t& t)
    {
        return hindu_true_position(t, hindu_sidereal_month, { 32, 360 }, hindu_anomalistic_month, { 1,96 });
    }

    rational_angle_t modern_hindu_base::hindu_lunar_phase(const rational_moment_t& t)
    {
        return mod(hindu_lunar_longitude(t) - hindu_solar_longitude(t), 360);
    }
    int32_t modern_hindu_base::hindu_lunar_day_from_moment(const rational_moment_t& t)
    {
        return (int32_t)floor_div(hindu_lunar_phase(t), 12) + 1;
    }

    static const rational hindu_E{ 1, boost::multiprecision::pow(bigint{2}, 1000)};

    rational_moment_t modern_hindu_base::hindu_new_moon_before(const rational_moment_t& t)
    {
        auto T = t - hindu_lunar_phase(t) * hindu_synodic_month / 360;
        auto p = [](const rational& l, const rational& u) { return hindu_zodiac(l) == hindu_zodiac(u) || (u - l) < hindu_E; };
        rational a = T - 1;
        rational b = boost::multiprecision::min((rational)t, T + 1);
        return binary_search([](const rational& x) { return hindu_lunar_phase(x) < 180; }, p, a, b);
    }
    int32_t modern_hindu_base::hindu_calendar_year(const rational_moment_t& t)
    {
        return (int32_t)round(((rational)t - (int64_t)m_epoch) / hindu_sidereal_year - hindu_solar_longitude(t) / 360);
    }
    rational_angle_t modern_hindu_base::hindu_ascensional_difference(const fixed_date_t& date, const rational_location& location)
    {
        rational sin_d = rational{ 1397, 3438 } * hindu_sine(hindu_tropical_longitude(date));
        auto psi = location.latitude;
        auto diurnal_radius = hindu_sine(90_rdeg + hindu_arcsin(sin_d));
        auto tan_psi = hindu_sine(psi) / hindu_sine(90_rdeg + psi);
        rational earth_sine = sin_d * tan_psi;
        return hindu_arcsin(-earth_sine / diurnal_radius);
    }

    static const rational half{ 1,2 };
    static const rational quarter{ 1,4 };

    rational_angle_t modern_hindu_base::hindu_tropical_longitude(const fixed_date_t& date)
    {
        static const rational a{ 600, 1577917828 };
        auto days = date - m_epoch;
        rational precession = 27 - boost::multiprecision::abs(108 * interval_modulus(a * days - quarter, -half, half));
        return mod(hindu_solar_longitude(date) - precession, 360);
    }
    rational modern_hindu_base::hindu_solar_sidereal_difference(const fixed_date_t& date)
    {
        return hindu_daily_motion(date) * hindu_rising_sign(date);
    }
    rational modern_hindu_base::hindu_daily_motion(const fixed_date_t& date)
    {
        static const rational mean_motion = 360 / hindu_sidereal_year;
        auto anomaly = hindu_mean_position(date, hindu_anomalistic_year);
        auto epicycle = rational{ 14,360 } - boost::multiprecision::abs(hindu_sine(anomaly)) / 1080;
        auto entry = (int64_t)floor_div(anomaly, rational{225,60});
        auto sine_table_step = hindu_sine_table(entry + 1) - hindu_sine_table(entry);
        auto factor = -rational{ 3438,225 } *sine_table_step * epicycle;
        return mean_motion * (factor + 1);
    }
    rational modern_hindu_base::hindu_rising_sign(const fixed_date_t& date)
    {
        static const std::array<rational, 6> table{
            rational{1670,1800},
            rational{1795,1800},
            rational{1935,1800},
            rational{1935,1800},
            rational{1795,1800},
            rational{1670,1800},
        };

        auto i = floor(hindu_tropical_longitude(date) / 30_rdeg);
        return table[(std::size_t)mod(i, 6)];
    }

    rational modern_hindu_base::hindu_equation_of_time(const fixed_date_t& date)
    {
        auto offset = hindu_sine(hindu_mean_position(date, hindu_anomalistic_year));
        auto equation_sun = offset * rational_angle_t::dms(57, 18) * (rational{ 14,360 } - boost::multiprecision::abs(offset) / 180);
        return (hindu_daily_motion(date) / 360) * (equation_sun / 360) * hindu_sidereal_year;
    }

    rational_moment_t modern_hindu_base::hindu_sunrise(const fixed_date_t& date) const
    {
        static const rational a{ 1577917828, 1582237828 * 360LL };
        static const rational quarter{ 1,4 };
        return (int64_t)date + quarter + (ujjain.longitude - m_hindu_location.longitude) / 360
            - hindu_equation_of_time(date)
            + a * (hindu_ascensional_difference(date, m_hindu_location) + quarter * hindu_solar_sidereal_difference(date));
    }

    rational_moment_t modern_hindu_base::hindu_sunset(const fixed_date_t& date) const
    {
        static const rational a{ 1577917828, 1582237828 * 360LL };
        static const rational quarter{ 3,4 };
        return (int64_t)date + quarter + (ujjain.longitude - m_hindu_location.longitude) / 360
            - hindu_equation_of_time(date)
            + a * (-hindu_ascensional_difference(date, m_hindu_location) + quarter * hindu_solar_sidereal_difference(date));
    }

    rational_moment_t modern_hindu_base::hindu_standard_from_sundial(const rational_moment_t& t) const
    {
        auto date = t.to_fixed_date();
        auto time = t.time();
        auto q = (int32_t)floor(4 * time);
        rational a, b, h;
        if (q == 0)
        {
            a = hindu_sunset(date.subtract(1));
            b = hindu_sunrise(date);
            h = rational{ -1,4 };
        }
        else if (q == 3)
        {
            a = hindu_sunset(date);
            b = hindu_sunrise(date.add(1));
            h = rational{ 3,4 };
        }
        else 
        {
            a = hindu_sunrise(date);
            b = hindu_sunset(date);
            h = rational{ 1,4 };
        }
        return rational{ a + 2 * (b - a) * (time - h) };
    }

    rational_moment_t modern_hindu_base::alt_hindu_sunrise(const fixed_date_t& date) const
    {
        static const auto prim = angle_t::dms(0, 47);
        auto rise = time_and_astronomy::dawn(date, (location)m_hindu_location, prim);
        return rational{
            (int32_t)std::round(rise * 24 * 60),
            24 * 60
        };
    }

    angle_t modern_hindu_base::ayanamsha(const moment_t& t)
    {
        return time_and_astronomy::solar_longitude(t) - time_and_astronomy::sidereal_solar_longitude(t);
    }

    moment_t modern_hindu_base::astro_hindu_sunset(const fixed_date_t& date) const
    {
        return time_and_astronomy::dusk(date, (location)m_hindu_location, 0_deg);
    }

    int32_t modern_hindu_base::sidereal_zodiac(const moment_t& t)
    {
        return (int32_t)std::floor(time_and_astronomy::sidereal_solar_longitude(t) / 30_deg) + 1;
    }

    int32_t modern_hindu_base::astro_hindu_calendar_year(const moment_t& t)
    {
        return (int32_t)std::round((t - m_epoch) / time_and_astronomy::mean_sidereal_year - time_and_astronomy::sidereal_solar_longitude(t) / 360_deg);
    }

    int32_t modern_hindu_base::astro_hindu_lunar_day_from_moment(const moment_t& t)
    {
        return (int32_t)floor_div(time_and_astronomy::lunar_phase(t), 12) + 1;
    }

    rational_moment_t modern_hindu_base::hindu_solar_longitude_at_or_after(const rational_angle_t& l, const rational_moment_t& t)
    {
        auto T = t + hindu_sidereal_year / 360 * mod(l - hindu_solar_longitude(t), 360);
        auto a = boost::multiprecision::max((rational)t, T - 5);
        rational b = T + 5;
        return inverse_angular(make_rational_value_function(hindu_solar_longitude), l, a, b);
    }

    rational_moment_t modern_hindu_base::mesha_samkranti(int32_t gyear)
    {
        auto jan1 = gregorian_calendar::standard.get_new_year(gyear);
        return hindu_solar_longitude_at_or_after(0_rdeg, jan1);
    }

    rational_moment_t modern_hindu_base::hindu_lunar_day_at_or_after(const rational& k, const rational_moment_t& t)
    {
        rational_angle_t phase = (k - 1) * rational{ 12 };
        rational T = t + mod(phase - hindu_lunar_phase(t), 360);
        auto a = boost::multiprecision::max((rational)t, T - 2);
        rational b = T + 2;
        return inverse_angular(make_rational_value_function(hindu_lunar_phase), phase, a, b);
    }

    fixed_date_t modern_hindu_base::hindu_lunar_new_year(int32_t gyear) const
    {
        auto jan1 = gregorian_calendar::standard.get_new_year(gyear);
        auto mina = hindu_solar_longitude_at_or_after(330, jan1);
        auto new_moon = hindu_lunar_day_at_or_after(1, mina);
        auto hday = (int64_t)floor(new_moon);
        auto critical = hindu_sunrise(hday);
        if (new_moon < critical || (2 == hindu_lunar_day_from_moment(hindu_sunrise(hday + 1))))
            return hday;
        else
            return hday + 1;
    }

    int32_t modern_hindu_base::hindu_lunar_station(const fixed_date_t& date) const
    {
        auto critical = hindu_sunrise(date);
        return (int32_t)floor_div(hindu_lunar_longitude(critical), rational_angle_t::dms(0, 800)) + 1;
    }

    int32_t modern_hindu_base::yoga(const fixed_date_t& date)
    {
        return 1 + (int32_t)mod((bigint)floor_div(hindu_solar_longitude(date) + hindu_lunar_longitude(date), rational_angle_t::dms(0, 800)), 27);
    }

}