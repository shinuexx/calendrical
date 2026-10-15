#include <calendars/persian_based_calendar.hpp>
#include "angle.hpp"
#include "calendars/gregorian_calendar.hpp"
#include "calendars/modern_hindu_base.hpp"
#include "time_and_astronomy.hpp"
#include <array>
#include <cmath>

namespace calendrical::calendars {
    
    double modern_hindu_base::hindu_sine_table(int64_t entry)
    {
        const auto exact = 3438 * sin(static_cast<double>(entry) * angle_t::dms(0, 225));
        const auto error = 0.215 * sign(exact) * sign(std::abs(exact) - 1716);
        return std::round(exact + error) / 3438.0;
    }

    double modern_hindu_base::hindu_sine(const angle_t& O)
    {
        const auto entry = O / angle_t::dms(0, 225);
        const auto fraction = mod(entry, 1);
        return fraction * hindu_sine_table(static_cast<int64_t>(std::ceil(entry))) + (1 - fraction) * hindu_sine_table(static_cast<int64_t>(std::floor(entry)));
    }
    angle_t modern_hindu_base::hindu_arcsin(const double& amp) // NOLINT(*-no-recursion)
    {
        if (amp < 0)
            return -hindu_arcsin(-amp);
        const auto pos = least_passing([&amp](const int64_t& k) { return amp <= hindu_sine_table(k); }, 0);
        const auto below = hindu_sine_table(pos - 1);
        return angle_t{ 225 / 60.0 * (pos - 1 + (amp - below) / (hindu_sine_table(pos) - below)) };
    }
    angle_t modern_hindu_base::hindu_mean_position(const moment_t& t, const double& period)
    {
        return angle_t{ 360 * mod((static_cast<double>(t) - hindu_creation) / period, 1) };
    }
    angle_t modern_hindu_base::hindu_true_position(const moment_t& t, const double& period, const double& size, const double& anomalistic, const double& change)
    {
        const auto l = hindu_mean_position(t, period);
        const auto offset = hindu_sine(hindu_mean_position(t, anomalistic));
        const double contraction = std::abs(offset) * change * size;
        const auto equation = hindu_arcsin(offset * (size - contraction));
        return mod(l - equation, 360);
    }
    angle_t modern_hindu_base::hindu_solar_longitude(const moment_t& t)
    {
        return hindu_true_position(t, hindu_sidereal_year, 14 / 360.0, hindu_anomalistic_year, 1 / 42.0 );
    }

    int32_t modern_hindu_base::hindu_zodiac(const moment_t& t)
    {
        return static_cast<int32_t>(floor_div(hindu_solar_longitude(t), 30_deg)) + 1;
    }

    angle_t modern_hindu_base::hindu_lunar_longitude(const moment_t& t)
    {
        return hindu_true_position(t, hindu_sidereal_month,  32 / 360.0 , hindu_anomalistic_month, 1 / 96.0 );
    }

    angle_t modern_hindu_base::hindu_lunar_phase(const moment_t& t)
    {
        return mod(hindu_lunar_longitude(t) - hindu_solar_longitude(t), 360);
    }
    int32_t modern_hindu_base::hindu_lunar_day_from_moment(const moment_t& t)
    {
        return static_cast<int32_t>(floor_div(hindu_lunar_phase(t), 12_deg)) + 1;
    }

    static constexpr double hindu_E{0x1p-100}; //{std::pow(2, -100)};

    moment_t modern_hindu_base::hindu_new_moon_before(const moment_t& t)
    {
        const auto T = static_cast<double>(t) - (hindu_lunar_phase(t) / angle_t::full_circle) * hindu_synodic_month;
        auto p = [](const double &l, const double &u) {
            return hindu_zodiac(moment_t{ l }) == hindu_zodiac(moment_t{ u }) || u - l < hindu_E;
        };
        const double a = T - 1;
        const double b = std::min(static_cast<double>(t), T + 1);
        return moment_t{ binary_search([](const double& x) { return hindu_lunar_phase(moment_t{ x }) < 180_deg; }, p, a, b) };
    }
    int32_t modern_hindu_base::hindu_calendar_year(const moment_t& t)
    {
        return static_cast<int32_t>(
                std::round((static_cast<double>(t) - static_cast<int64_t>(ms_epoch)) / hindu_sidereal_year - hindu_solar_longitude(t) / 360_deg));
    }
    angle_t modern_hindu_base::hindu_ascensional_difference(const fixed_date_t& date, const location& location)
    {
        const double sin_d =  1397.0 / 3438 * hindu_sine(hindu_tropical_longitude(date));
        const auto psi = location.latitude;
        const auto diurnal_radius = hindu_sine(90_deg + hindu_arcsin(sin_d));
        const auto tan_psi = hindu_sine(psi) / hindu_sine(90_deg + psi);
        const double earth_sine = sin_d * tan_psi;
        return hindu_arcsin(-earth_sine / diurnal_radius);
    }

    angle_t modern_hindu_base::hindu_tropical_longitude(const fixed_date_t& date)
    {
        constexpr double a{6e2 / 1577917828};
        const auto days = date - ms_epoch;
        const double precession = 27 - std::abs(108 * interval_modulus(a * days - 0.25, -0.5, 0.5));
        return mod(hindu_solar_longitude(moment_t{ date }) - angle_t{ precession }, 360);
    }
    double modern_hindu_base::hindu_solar_sidereal_difference(const fixed_date_t& date)
    {
        return hindu_daily_motion(date) * hindu_rising_sign(date);
    }
    double modern_hindu_base::hindu_daily_motion(const fixed_date_t& date)
    {
        constexpr double mean_motion = 360 / hindu_sidereal_year;
        const auto anomaly = hindu_mean_position(moment_t{ date }, hindu_anomalistic_year);
        const auto epicycle = 14/360.0 - std::abs(hindu_sine(anomaly)) / 1080;
        const auto entry = static_cast<int64_t>(floor_div(anomaly, angle_t{ 225 / 6e1 }));
        const auto sine_table_step = hindu_sine_table(entry + 1) - hindu_sine_table(entry);
        const auto factor = -3438 / 225.0 * sine_table_step * epicycle;
        return mean_motion * (factor + 1);
    }
    double modern_hindu_base::hindu_rising_sign(const fixed_date_t& date)
    {
        constexpr std::array table{
                1670 / 1800.0, 1795 / 1800.0, 1935 / 1800.0,
                1935 / 1800.0, 1795 / 1800.0, 1670 / 1800.0,
        };

        const auto i = std::floor(hindu_tropical_longitude(date) / 30_deg);
        return table[static_cast<std::size_t>(mod(i, 6))];
    }

    double modern_hindu_base::hindu_equation_of_time(const fixed_date_t& date)
    {
        const auto offset = hindu_sine(hindu_mean_position(moment_t{ date }, hindu_anomalistic_year));
        const double equation_sun = offset * angle_t::dms(57, 18).degrees() * ( 14/360.0 - std::abs(offset) / 1080);
        return hindu_daily_motion(date) / 360 * (equation_sun / 360) * hindu_sidereal_year;
    }

    moment_t modern_hindu_base::hindu_sunrise(const fixed_date_t& date) const
    {
        constexpr double a{ 1577917828 / (1582237828 * 360.0) };
        constexpr double quarter{ 0.25 };
        return moment_t{ static_cast<int64_t>(date) + quarter + day_fraction(ujjain.longitude - m_hindu_location.longitude)
            - hindu_equation_of_time(date)
            + a * (hindu_ascensional_difference(date, m_hindu_location).degrees() + quarter * hindu_solar_sidereal_difference(date)) };
    }

    moment_t modern_hindu_base::hindu_sunset(const fixed_date_t& date) const
    {
        constexpr double a{ 1577917828 / (1582237828 * 360.0) };
        constexpr double quarter{ 0.25 };
        return moment_t{ static_cast<int64_t>(date) + quarter + day_fraction(ujjain.longitude - m_hindu_location.longitude)
            - hindu_equation_of_time(date)
            + a * ((-hindu_ascensional_difference(date, m_hindu_location)).degrees() + quarter * hindu_solar_sidereal_difference(date)) };
    }

    moment_t modern_hindu_base::hindu_standard_from_sundial(const moment_t& t) const
    {
        const auto date = t.to_fixed_date();
        const auto time = t.time();
        const auto q = static_cast<int32_t>(std::floor(4 * time));
        double a, b, h;
        if (q == 0)
        {
            a = static_cast<double>(hindu_sunset(date.subtract(1)));
            b = static_cast<double>(hindu_sunrise(date));
            h = -0.25;
        }
        else if (q == 3)
        {
            a = static_cast<double>(hindu_sunset(date));
            b = static_cast<double>(hindu_sunrise(date.add(1)));
            h = 0.75;
        }
        else
        {
            a = static_cast<double>(hindu_sunrise(date));
            b = static_cast<double>(hindu_sunset(date));
            h = 0.25;
        }
        return moment_t{ a + 2 * (b - a) * (time - h) };
    }

    moment_t modern_hindu_base::alt_hindu_sunrise(const fixed_date_t& date) const
    {
        static constexpr auto prim = angle_t::dms(0, 47);
        const auto rise = time_and_astronomy::dawn(date, m_hindu_location, prim);
        return moment_t{ std::round(rise.days() * 24 * 60) / (24 * 60) };
    }

    angle_t modern_hindu_base::ayanamsha(const moment_t& t)
    {
        return time_and_astronomy::solar_longitude(t) - time_and_astronomy::sidereal_solar_longitude(t);
    }

    moment_t modern_hindu_base::astro_hindu_sunset(const fixed_date_t& date) const
    {
        return time_and_astronomy::dusk(date, m_hindu_location, 0_deg);
    }

    int32_t modern_hindu_base::sidereal_zodiac(const moment_t& t)
    {
        return static_cast<int32_t>(std::floor(time_and_astronomy::sidereal_solar_longitude(t) / 30_deg)) + 1;
    }

    int32_t modern_hindu_base::astro_hindu_calendar_year(const moment_t& t)
    {
        return static_cast<int32_t>(std::round((t - ms_epoch).days() / time_and_astronomy::mean_sidereal_year -
                                               time_and_astronomy::sidereal_solar_longitude(t) / 360_deg));
    }

    int32_t modern_hindu_base::astro_hindu_lunar_day_from_moment(const moment_t& t)
    {
        return static_cast<int32_t>(floor_div(time_and_astronomy::lunar_phase(t), 12_deg)) + 1;
    }

    moment_t modern_hindu_base::hindu_solar_longitude_at_or_after(const angle_t& l, const moment_t& t)
    {
        const auto T = static_cast<double>(t) + hindu_sidereal_year / 360 * mod(l - hindu_solar_longitude(t), 360).degrees();
        const auto a = std::max(static_cast<double>(t), T - 5);
        const double b = T + 5;
        return moment_t{ inverse_angular(make_value_function(hindu_solar_longitude), l.degrees(), a, b) };
    }

    moment_t modern_hindu_base::mesha_samkranti(int32_t gyear)
    {
        const auto jan1 = gregorian_calendar::standard.get_new_year(gyear);
        return hindu_solar_longitude_at_or_after(0_deg, moment_t{ jan1 });
    }

    moment_t modern_hindu_base::hindu_lunar_day_at_or_after(const double& k, const moment_t& t)
    {
        const angle_t phase = (k - 1) * 12_deg;
        const double T = static_cast<double>(t) + mod(phase - hindu_lunar_phase(t), 360).degrees() * hindu_synodic_month / 360;
        const auto a = std::max(static_cast<double>(t), T - 2);
        const double b = T + 2;
        return moment_t{ inverse_angular(make_value_function(hindu_lunar_phase), phase.degrees(), a, b) };
    }

    fixed_date_t modern_hindu_base::hindu_lunar_new_year(int32_t gyear) const
    {
        const auto jan1 = gregorian_calendar::standard.get_new_year(gyear);
        const auto mina = hindu_solar_longitude_at_or_after(330_deg, moment_t{ jan1 });
        const auto new_moon = hindu_lunar_day_at_or_after(1, mina);
        const auto hday = floor(new_moon);
        if (const auto critical = hindu_sunrise(hday);
            new_moon < critical || 2 == hindu_lunar_day_from_moment(hindu_sunrise(hday + 1)))
            return hday;
        return hday + 1;
    }

    int32_t modern_hindu_base::hindu_lunar_station(const fixed_date_t& date) const
    {
        const auto critical = hindu_sunrise(date);
        return static_cast<int32_t>(floor_div(hindu_lunar_longitude(critical), angle_t::dms(0, 800))) + 1;
    }

    int32_t modern_hindu_base::yoga(const fixed_date_t& date)
    {
        return 1 + static_cast<int32_t>(mod(floor_div(hindu_solar_longitude(moment_t{ date }) + hindu_lunar_longitude(moment_t{ date }),
                                                             angle_t::dms(0, 800)),
                                        27));
    }
}