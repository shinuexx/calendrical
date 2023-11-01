#include "time_and_astronomy.hpp"
#include "calendars/gregorian_calendar.hpp"
#include "polynomial.hpp"
#include "location.hpp"
#include "season.hpp"
#include "lunar_phase.hpp"

namespace calendrical::time_and_astronomy {

    static const fixed_date_t ephemeris_correction_start = calendars::gregorian_calendar::standard.to_fixed({ 1900, month::january, 1 });

    static constexpr auto r2051 = range_closed_between<int32_t>(2051, 2150);
    static constexpr auto r2006 = range_closed_between<int32_t>(2006, 2050);
    static constexpr auto r1987 = range_closed_between<int32_t>(1987, 2005);
    static constexpr auto r1900 = range_closed_between<int32_t>(1900, 1986);
    static constexpr auto r1800 = range_closed_between<int32_t>(1800, 1899);
    static constexpr auto r1700 = range_closed_between<int32_t>(1700, 1799);
    static constexpr auto r1600 = range_closed_between<int32_t>(1600, 1699);
    static constexpr auto r500 = range_closed_between<int32_t>(500, 1599);
    static constexpr auto r0 = range_opened_between<int32_t>(-500, 500);

    static constexpr auto p2006 = make_polynomial(62.92, 0.32217, 0.005589);
    static constexpr auto p1987 = make_polynomial(63.86, 0.3345, -0.060374, 0.0017275, 0.000651814, 0.00002373599);
    static constexpr auto p1900 = make_polynomial(-0.00002, 0.000297, 0.025184, -0.181133, 0.553040, -0.861938, 0.677066, -0.212591);
    static constexpr auto p1800 = make_polynomial(-0.000009, 0.003844, 0.083563, 0.865736, 4.867575, 15.845535, 31.332267, 38.291999, 28.316289, 11.636204, 2.043794);
    static constexpr auto p1700 = make_polynomial(8.118780842, -0.005092142, 0.003336121, -0.0000266484);
    static constexpr auto p1600 = make_polynomial(120 - 0.9808, -0.01532, 0.000140272128);
    static constexpr auto p500 = make_polynomial(1574.2, -556.01, 71.23472, 0.319781, -0.8503463, -0.005050998, 0.0083572073);
    static constexpr auto p0 = make_polynomial(10583.6, -1014.41, 33.78311, -5.952053, -0.1798452, 0.022174192, 0.0090316521);

    moment_t ephemeris_correction(const moment_t& t)
    {
        auto year = calendars::gregorian_calendar::standard.get_year(t.to_fixed_date());
        auto c = (calendars::gregorian_calendar::standard.to_fixed({ year, month::july, 1 }) - ephemeris_correction_start) / 36525.0;
        auto y2000 = year - 2000;
        if (in(r2051, year)) {
            auto c2051 = -20 + 32 * std::pow((year - 1820.0) / 1e2, 2) + 0.5628 * (2150.0 - year);
            return c2051 / 86400;
        }
        else if (in(r2006, year)) {
            auto c2006 = p2006(y2000) / 86400;
            return c2006;
        }
        else if (in(r1987, year)) {
            auto c1987 = p1987(y2000) / 86400;
            return c1987;
        }
        else if (in(r1900, year))
        {
            auto c1900 = p1900(c);
            return c1900;
        }
        else if (in(r1800, year))
        {
            auto c1800 = p1800(c);
            return c1800;
        }
        else if (in(r1700, year))
        {
            auto y1700 = year - 1700;
            auto c1700 = p1700(y1700) / 86400;
            return c1700;
        }
        else if (in(r1600, year))
        {
            auto y1600 = year - 1600;
            auto c1800 = p1600(y1600) / 86400;
            return c1800;
        }
        else if (in(r500, year))
        {
            auto y1000 = (year - 1000.0) / 1e2;
            auto c500 = p500(y1000) / 86400;
            return c500;
        }
        else if (in(r0, year))
        {
            auto y0 = year / 1e2;
            auto c0 = p0(y0) / 86400;
            return c0;
        }
        else
        {
            auto y1820 = (year - 1820.0) / 1e2;
            auto other = -20 + 32 * y1820 * y1820;
            return other / 86400;
        }
    }

    const moment_t J2000 = calendars::gregorian_calendar::standard.get_new_year(2000) + 0.5;
    
    moment_t dynamic_from_universal(const moment_t& t)
    {
        return t + ephemeris_correction(t);
    }
    moment_t universal_from_dynamic(const moment_t& t)
    {
        return t - ephemeris_correction(t);
    }
    double julian_centuries(const moment_t& t)
    {
        return (dynamic_from_universal(t) - J2000) / 36525;
    }

    static constexpr auto eot_lambda_p = make_polynomial(280.46645, 36000.76983, 0.0003032);
    static constexpr auto eot_anomaly_p = make_polynomial(357.52910, 35999.05030, -0.0001559, -0.00000048);
    static constexpr auto eot_eccentricity_p = make_polynomial(0.016708617, -0.00042037, -0.0000001236);

    moment_t equation_of_time(const moment_t& t)
    {
        auto c = julian_centuries(t);
        angle_t l = eot_lambda_p(c);
        angle_t anomaly = eot_anomaly_p(c);
        auto eccentricity = eot_eccentricity_p(c);
        auto E = obliquity(t);
        auto y = std::pow(static_cast<double>(E) * 2, 2);
        auto equation = y * sin(2 * l) - 2 * eccentricity * sin(anomaly)
            + 4 * eccentricity * y * sin(anomaly) * cos(2 * l)
            - 0.5 * y * y * sin(4 * l)
            - 1.25 * eccentricity * eccentricity * sin(2 * anomaly);
        equation /= 2 * std::numbers::pi;
        return sign(equation) * std::min(std::abs(equation), 0.5);
    }

    moment_t apparent_from_local(const moment_t& t, const location& location)
    {
        return t + equation_of_time(universal_from_local(t, location));
    }

    moment_t local_from_apparent(const moment_t& t, const location& location)
    {
        return t - equation_of_time(universal_from_local(t, location));
    }

    moment_t apparent_from_universal(const moment_t& t, const location& location)
    {
        return apparent_from_local(local_from_universal(t, location), location);
    }

    moment_t universal_from_apparent(const moment_t& t, const location& location)
    {
        return universal_from_local(local_from_apparent(t, location), location);
    }

    moment_t midnight(const fixed_date_t& date, const location& location)
    {
        return universal_from_apparent(date, location);
    }

    moment_t midday(const fixed_date_t& date, const location& location)
    {
        return universal_from_apparent(date + 0.5, location);
    }

    static constexpr auto sidereal_p = make_polynomial(280.46061837, 36525 * 360.98564736629, 0.000387933, -1 / 3871e4);

    angle_t sidereal_from_moment(const moment_t& t)
    {
        auto c = (t - J2000) / 36525;
        return mod(sidereal_p(c), 360);
    }

    static constexpr auto obliquity_begin = angle_t::dms(23, 26, 21.448);
    static constexpr auto obliquity_p = make_polynomial(0, -46.5150, -0.00059, 0.001813);

    angle_t obliquity(const moment_t& t)
    {
        auto c = julian_centuries(t);
        angle_t a = obliquity_p(c) / 3600;
        return obliquity_begin + a;
    }

    angle_t declination(const moment_t& t, const angle_t& b, const angle_t& l)
    {
        auto E = obliquity(t);
        return asin(sin(b) * cos(E) + cos(b) * sin(E) * sin(l));
    }

    angle_t right_ascension(const moment_t& t, const angle_t& b, const angle_t& l)
    {
        auto E = obliquity(t);
        return arctan(sin(l) * cos(E) - tan(b) * sin(E), cos(l));
    }

    static constexpr auto solar_longitude_xyz = std::array{
        std::make_tuple<int, double, double>(403406, 270.54861, 0.9287892),
        std::make_tuple<int, double, double>(195207, 340.19128, 35999.1376958),
        std::make_tuple<int, double, double>(119433, 63.91854, 35999.4089666),
        std::make_tuple<int, double, double>(112392, 331.26220, 35998.7287385),
        std::make_tuple<int, double, double>(3891, 317.843, 71998.20261),
        std::make_tuple<int, double, double>(2819, 86.631, 71998.4403),
        std::make_tuple<int, double, double>(1721, 240.052, 36000.35726),
        std::make_tuple<int, double, double>(660, 310.26, 71997.4812),
        std::make_tuple<int, double, double>(350, 247.23, 32964.4678),
        std::make_tuple<int, double, double>(334, 260.87, -19.4410),
        std::make_tuple<int, double, double>(314, 297.82, 445267.1117),
        std::make_tuple<int, double, double>(268, 343.14, 45036.8840),
        std::make_tuple<int, double, double>(242, 166.79, 3.1008),
        std::make_tuple<int, double, double>(234, 81.53, 22518.4434),
        std::make_tuple<int, double, double>(158, 3.50, -19.9739),
        std::make_tuple<int, double, double>(132, 132.75, 65928.9345),
        std::make_tuple<int, double, double>(129, 182.95, 9038.0293),
        std::make_tuple<int, double, double>(114, 162.03, 3034.7684),
        std::make_tuple<int, double, double>(99, 29.8, 33718.148),
        std::make_tuple<int, double, double>(93, 266.4, 3034.448),
        std::make_tuple<int, double, double>(86, 249.2, -2280.773),
        std::make_tuple<int, double, double>(78, 157.6, 29929.992),
        std::make_tuple<int, double, double>(72, 257.8, 31556.493),
        std::make_tuple<int, double, double>(68, 185.1, 149.588),
        std::make_tuple<int, double, double>(64, 69.9, 9037.750),
        std::make_tuple<int, double, double>(46, 8, 107997.405),
        std::make_tuple<int, double, double>(38, 197.1, -4444.176),
        std::make_tuple<int, double, double>(37, 250.4, 151.771),
        std::make_tuple<int, double, double>(32, 65.3, 67555.316),
        std::make_tuple<int, double, double>(29, 162.7, 31556.080),
        std::make_tuple<int, double, double>(28, 341.5, -4561.540),
        std::make_tuple<int, double, double>(27, 291.6, 107996.706),
        std::make_tuple<int, double, double>(27, 98.5, 1221.655),
        std::make_tuple<int, double, double>(25, 146.7, 62894.167),
        std::make_tuple<int, double, double>(24, 110, 31437.369),
        std::make_tuple<int, double, double>(21, 5.2, 14578.298),
        std::make_tuple<int, double, double>(21, 342.6, -31931.757),
        std::make_tuple<int, double, double>(20, 230.9, 34777.243),
        std::make_tuple<int, double, double>(18, 256.1, 1221.999),
        std::make_tuple<int, double, double>(17, 45.3, 62894.511),
        std::make_tuple<int, double, double>(14, 242.9, -4442.039),
        std::make_tuple<int, double, double>(13, 115.2, 107997.909),
        std::make_tuple<int, double, double>(13, 151.8, 119.066),
        std::make_tuple<int, double, double>(13, 285.3, 16859.071),
        std::make_tuple<int, double, double>(12, 53.3, -4.578),
        std::make_tuple<int, double, double>(10, 126.6, 26895.292),
        std::make_tuple<int, double, double>(10, 205.7, -39.127),
        std::make_tuple<int, double, double>(10, 85.9, 12297.536),
        std::make_tuple<int, double, double>(10, 146.1, 90073.778),
    };

    angle_t solar_longitude(const moment_t& t)
    {
        auto c = julian_centuries(t);
        double a{0};
        for (auto& [x, y, z] : solar_longitude_xyz)
            a += x * sin(y + z * c);
        auto l = 282.7771834 + 36000.7695374 * c + 0.000005729577951308232 * a;
        return mod(l + abberation(t) + nutation(t), 360);
    }

    static constexpr auto A_p = make_polynomial(124.9, -1934.134, 0.002063);
    static constexpr auto B_p = make_polynomial(201.11, 72001.5377, 0.00057);

    angle_t nutation(const moment_t& t)
    {
        auto c = julian_centuries(t);
        auto A = A_p(c);
        auto B = B_p(c);
        return -0.004778 * sin(A) - 0.0003667 * sin(B);
    }

    angle_t abberation(const moment_t& t) {
        auto c = julian_centuries(t);
        return 0.0000974 * cos(177.63 + 35999.01848 * c) - 0.005575;
    }
    moment_t solar_longitude_after(const angle_t& l, const moment_t& t)
    {
        constexpr auto rate = mean_tropical_year / 360;
        auto T = t + rate * mod(l - solar_longitude(t), 360);
        auto a = std::max<double>(t, T - 5);
        auto b = T + 5;
        return inverse_angular(make_value_function(solar_longitude), l, a, b);
    }
    moment_t season_in_gregorian(const angle_t& season, int32_t year)
    {
        auto jan1 = calendars::gregorian_calendar::standard.get_new_year(year);
        return solar_longitude_after(season, jan1);
    }

    moment_t urbana_winter(int32_t gyear)
    {
        return standard_from_universal(season_in_gregorian(season::WINTER, gyear), locations::urbana);
    }

    static constexpr auto precession_n_p = make_polynomial(47.0029, -0.03302, 0.00006);
    static constexpr auto precession_P_p = make_polynomial(174.876384, -869.8089 / 3600, 0.03536 / 3600);
    static constexpr auto precession_p_p = make_polynomial(5029.0966, 1.11113, 0.000006);

    angle_t precession(const moment_t& t)
    {
        auto c = julian_centuries(t);
        auto n = mod(precession_n_p(c) / 3600, 360);
        auto P = mod(precession_P_p(c), 360);
        auto p = mod(precession_p_p(c) / 3600, 360);
        auto A = cos(n) * sin(P);
        auto B = cos(P);
        auto arg = arctan(A, B);
        return mod(p + P - arg, 360);
    }

    angle_t sidereal_solar_longitude(const moment_t& t)
    {
        return mod(solar_longitude(t) - precession(t) + sidereal_start, 360);
    }

    angle_t solar_altitude(const moment_t& t, const location& location)
    {
        auto psi = location.latitude;
        auto phi = location.longitude;
        auto l = solar_longitude(t);
        auto a = right_ascension(t, 0, l);
        auto d = declination(t, 0, l);
        auto O0 = sidereal_from_moment(t);
        auto H = mod(O0 + phi - a, 360);
        auto altitude = asin(sin(psi) * sin(d) + cos(psi) * cos(d) * cos(H));
        return interval_modulus(altitude, -180, 180);
    }
    moment_t estimate_prior_solar_longitude(const angle_t& l, const moment_t& t)
    {
        constexpr auto rate = mean_tropical_year / 360;
        auto T = t - rate * mod(solar_longitude(t) - l, 360);
        auto D = interval_modulus(solar_longitude(t) - l, -180, 180);
        return std::min<double>(t, T - rate * D);
    }

    static constexpr auto nth_new_moon_table_a = std::array{
        std::make_tuple<double, int, int, int, int>(-0.40720, 0, 0, 1, 0),
        std::make_tuple<double, int, int, int, int>(0.17241, 1, 1, 0, 0),
        std::make_tuple<double, int, int, int, int>(0.01608, 0, 0, 2, 0),
        std::make_tuple<double, int, int, int, int>(0.01039, 0, 0, 0, 2),
        std::make_tuple<double, int, int, int, int>(0.00739, 1, -1, 1, 0),
        std::make_tuple<double, int, int, int, int>(-0.00514, 1, 1, 1, 0),
        std::make_tuple<double, int, int, int, int>(0.00208, 2, 2, 0, 0),
        std::make_tuple<double, int, int, int, int>(-0.00111, 0, 0, 1, -2),
        std::make_tuple<double, int, int, int, int>(-0.00057, 0, 0, 1, 2),
        std::make_tuple<double, int, int, int, int>(0.00056, 1, 1, 2, 0),
        std::make_tuple<double, int, int, int, int>(-0.00042, 0, 0, 3, 0),
        std::make_tuple<double, int, int, int, int>(0.00042, 1, 1, 0, 2),
        std::make_tuple<double, int, int, int, int>(0.00038, 1, 1, 0, -2),
        std::make_tuple<double, int, int, int, int>(-0.00024, 1, -1, 2, 0),
        std::make_tuple<double, int, int, int, int>(-0.00007, 0, 2, 1, 0),
        std::make_tuple<double, int, int, int, int>(0.00004, 0, 0, 2, -2),
        std::make_tuple<double, int, int, int, int>(0.00004, 0, 3, 0, 0),
        std::make_tuple<double, int, int, int, int>(0.00003, 0, 1, 1, -2),
        std::make_tuple<double, int, int, int, int>(0.00003, 0, 0, 2, 2),
        std::make_tuple<double, int, int, int, int>(-0.00003, 0, 1, 1, 2),
        std::make_tuple<double, int, int, int, int>(0.00003, 0, -1, 1, 2),
        std::make_tuple<double, int, int, int, int>(-0.00002, 0, -1, 1, -2),
        std::make_tuple<double, int, int, int, int>(-0.00002, 0, 1, 3, 0),
        std::make_tuple<double, int, int, int, int>(0.00002, 0, 0, 4, 0),
    };

    static constexpr auto nth_new_moon_table_b = std::array{
        std::make_tuple<double, double, double>(251.88, 0.016321, 0.000165),
        std::make_tuple<double, double, double>(251.83, 26.641886, 0.000164),
        std::make_tuple<double, double, double>(349.42, 36.412478, 0.000126),
        std::make_tuple<double, double, double>(84.66, 18.206239, 0.000110),
        std::make_tuple<double, double, double>(141.74, 53.303771, 0.000062),
        std::make_tuple<double, double, double>(207.14, 2.453732, 0.000060),
        std::make_tuple<double, double, double>(154.84, 7.306860, 0.000056),
        std::make_tuple<double, double, double>(34.52, 27.261239, 0.000047),
        std::make_tuple<double, double, double>(207.19, 0.121824, 0.000042),
        std::make_tuple<double, double, double>(291.34, 1.844379, 0.000040),
        std::make_tuple<double, double, double>(161.72, 24.198154, 0.000037),
        std::make_tuple<double, double, double>(239.56, 25.513099, 0.000035),
        std::make_tuple<double, double, double>(331.55, 3.592518, 0.000023),
    };

    static constexpr auto nth_new_moon_approx_p = make_polynomial(5.09766, mean_synodic_month * 1236.85, 0.00015437, -0.00000015, 0.00000000073);
    static constexpr auto nth_new_moon_E_p = make_polynomial(1, -0.002516, -0.0000074);
    static constexpr auto nth_new_moon_solar_anomaly_p = make_polynomial(2.5534, 1236.84 * 29.1053567, -0.0000014, -0.00000011);
    static constexpr auto nth_new_moon_lunar_anomaly_p = make_polynomial(201.5643, 385.81693528 * 1236.85, 0.0107582, 0.00001238, -0.000000058);
    static constexpr auto nth_new_moon_moon_argument_p = make_polynomial(160.7108, 390.67050284 * 1236.85, -0.0016118, -0.00000227, 0.000000011);
    static constexpr auto nth_new_moon_omega_p = make_polynomial(124.7746, -156375588 * 1236.85, 0.0020672, 0.00000215);
    static constexpr auto nth_new_moon_extra_p = make_polynomial(299.77, 132.8475848, -0.009173);

    moment_t nth_new_moon(int64_t n) {
        constexpr int64_t n0 = 24724;
        auto k = n - n0;
        auto c = k / 1236.85;
        auto approx = J2000 + nth_new_moon_approx_p(c);
        auto E = nth_new_moon_E_p(c);
        auto solar_anomaly = nth_new_moon_solar_anomaly_p(c);
        auto lunar_anomaly = nth_new_moon_lunar_anomaly_p(c);
        auto moon_argument = nth_new_moon_moon_argument_p(c);
        auto O = nth_new_moon_omega_p(c);
        auto extra = 0.00325 * sin(nth_new_moon_extra_p(c));
        double additional{ 0 };
        for (auto& [i, j, l] : nth_new_moon_table_b)
            additional += l * sin(i + j * k);
        double correction = -0.00017 * sin(O);
        for (auto& [v, w, x, y, z] : nth_new_moon_table_a)
            correction += v * std::pow(E, w) * sin(x * solar_anomaly + y * lunar_anomaly + z * moon_argument);
        
        return universal_from_dynamic(approx + correction + extra + additional);
    }

    static const moment_t zero_new_moon = nth_new_moon(0);

    moment_t new_moon_before(const moment_t& t) {
        auto t0 = zero_new_moon;
        auto psi = lunar_phase(t);
        auto n = static_cast<int64_t>(std::round((t - t0) / mean_synodic_month - psi / angle_t::full_circle));
        auto a = last_passing([&t](int64_t k) { return nth_new_moon(k) < t; }, n - 1);
        return nth_new_moon(a);
    }
    moment_t new_moon_on_or_after(const moment_t& t) {
        auto t0 = zero_new_moon;
        auto psi = lunar_phase(t);
        auto n = static_cast<int64_t>(std::round((t - t0) / mean_synodic_month - psi / angle_t::full_circle));
        auto a = least_passing([&t](int64_t k) { return nth_new_moon(k) >= t; }, n - 1);
        return nth_new_moon(a);
    }

    static constexpr auto lunar_longitude_table = std::array{
        std::make_tuple<int,int,int,int,int>(6288774, 0, 0, 1, 0),
        std::make_tuple<int,int,int,int,int>(1274027, 2, 0, -1, 0),
        std::make_tuple<int,int,int,int,int>(658314, 2, 0, 0, 0),
        std::make_tuple<int,int,int,int,int>(213618, 0, 0, 2, 0),
        std::make_tuple<int,int,int,int,int>(-185116, 0, 1, 0, 0),
        std::make_tuple<int,int,int,int,int>(-114332, 0, 0, 0, 2),
        std::make_tuple<int,int,int,int,int>(58793, 2, 0, -2, 0),
        std::make_tuple<int,int,int,int,int>(57066, 2, -1, -1, 0),
        std::make_tuple<int,int,int,int,int>(53322, 2, 0, 1, 0),
        std::make_tuple<int,int,int,int,int>(45758, 2, -1, 0, 0),
        std::make_tuple<int,int,int,int,int>(-40923, 0, 1, -1, 0),
        std::make_tuple<int,int,int,int,int>(-34720, 1, 0, 0, 0),
        std::make_tuple<int,int,int,int,int>(-30383, 0, 1, 1, 0),
        std::make_tuple<int,int,int,int,int>(15327, 2, 0, 0, -2),
        std::make_tuple<int,int,int,int,int>(-12528, 0, 0, 1, 2),
        std::make_tuple<int,int,int,int,int>(10980, 0, 0, 1, -2),
        std::make_tuple<int,int,int,int,int>(10675, 4, 0, -1, 0),
        std::make_tuple<int,int,int,int,int>(10034, 0, 0, 3, 0),
        std::make_tuple<int,int,int,int,int>(8548, 4, 0, -2, 0),
        std::make_tuple<int,int,int,int,int>(-7888, 2, 1, -1, 0),
        std::make_tuple<int,int,int,int,int>(-6766, 2, 1, 0, 0),
        std::make_tuple<int,int,int,int,int>(-5163, 1, 0, -1, 0),
        std::make_tuple<int,int,int,int,int>(4987, 1, 1, 0, 0),
        std::make_tuple<int,int,int,int,int>(4036, 2, -1, 1, 0),
        std::make_tuple<int,int,int,int,int>(3994, 2, 0, 2, 0),
        std::make_tuple<int,int,int,int,int>(3861, 4, 0, 0, 0),
        std::make_tuple<int,int,int,int,int>(3665, 2, 0, -3, 0),
        std::make_tuple<int,int,int,int,int>(-2689, 0, 1, -2, 0),
        std::make_tuple<int,int,int,int,int>(-2602, 2, 0, -1, 2),
        std::make_tuple<int,int,int,int,int>(2390, 2, -1, -2, 0),
        std::make_tuple<int,int,int,int,int>(-2348, 1, 0, 1, 0),
        std::make_tuple<int,int,int,int,int>(2236, 2, -2, 0, 0),
        std::make_tuple<int,int,int,int,int>(-2120, 0, 1, 2, 0),
        std::make_tuple<int,int,int,int,int>(-2069, 0, 2, 0, 0),
        std::make_tuple<int,int,int,int,int>(2048, 2, -2, -1, 0),
        std::make_tuple<int,int,int,int,int>(-1773, 2, 0, 1, -2),
        std::make_tuple<int,int,int,int,int>(-1595, 2, 0, 0, 2),
        std::make_tuple<int,int,int,int,int>(1215, 4, -1, -1, 0),
        std::make_tuple<int,int,int,int,int>(-1110, 0, 0, 2, 2),
        std::make_tuple<int,int,int,int,int>(-892, 3, 0, -1, 0),
        std::make_tuple<int,int,int,int,int>(-810, 2, 1, 1, 0),
        std::make_tuple<int,int,int,int,int>(759, 4, -1, -2, 0),
        std::make_tuple<int,int,int,int,int>(-713, 0, 2, -1, 0),
        std::make_tuple<int,int,int,int,int>(-700, 2, 2, -1, 0),
        std::make_tuple<int,int,int,int,int>(691, 2, 1, -2, 0),
        std::make_tuple<int,int,int,int,int>(596, 2, -1, 0, -2),
        std::make_tuple<int,int,int,int,int>(549, 4, 0, 1, 0),
        std::make_tuple<int,int,int,int,int>(537, 0, 0, 4, 0),
        std::make_tuple<int,int,int,int,int>(520, 4, -1, 0, 0),
        std::make_tuple<int,int,int,int,int>(-487, 1, 0, -2, 0),
        std::make_tuple<int,int,int,int,int>(-399, 2, 1, 0, -2),
        std::make_tuple<int,int,int,int,int>(-381, 0, 0, 2, -2),
        std::make_tuple<int,int,int,int,int>(351, 1, 1, 1, 0),
        std::make_tuple<int,int,int,int,int>(-340, 3, 0, -2, 0),
        std::make_tuple<int,int,int,int,int>(330, 4, 0, -3, 0),
        std::make_tuple<int,int,int,int,int>(327, 2, -1, 2, 0),
        std::make_tuple<int,int,int,int,int>(-323, 0, 2, 1, 0),
        std::make_tuple<int,int,int,int,int>(299, 1, 1, -1, 0),
        std::make_tuple<int,int,int,int,int>(294, 2, 0, 3, 0),
    };

    static constexpr auto lunar_E_p = make_polynomial(1, -0.002516, -0.0000074);

    angle_t lunar_longitude(const moment_t& t) {
        auto c = julian_centuries(t);
        auto L1 = mean_lunar_longitude(c);
        auto D = lunar_elongation(c);
        auto M = solar_anomaly(c);
        auto M1 = lunar_anomaly(c);
        auto F = moon_node(c);
        auto E = lunar_E_p(c);
        double correction{};
        for (auto& [v, w, x, y, z] : lunar_longitude_table) {
            correction += v * std::pow(E, std::abs(x)) * sin(w * D + x * M + y * M1 + z * F);
        }
        auto venus = 395 * sin(119.75 + c * 131.849);
        auto jupiter = 318 * sin(53.09 + c * 479264.29);
        auto flat_earth = 1962 * sin(L1 - F);
        return mod(static_cast<double>(L1) + (correction + venus + jupiter + flat_earth) / 1e6 + nutation(t), 360);
    }

    static constexpr auto moon_node_p = make_polynomial(93.2720950, 483202.0175233, -0.0036539, -1 / 3526000.0, 1 / 863310000.0);
    static constexpr auto lunar_anomaly_p = make_polynomial(134.9633964, 477198.8675055, 0.0087414, 1 / 69699.0, -1 / 14712000.0);
    static constexpr auto solar_anomaly_p = make_polynomial(357.5291092, 35999.0502909, -0.0001536, 1 / 24490000.0);
    static constexpr auto lunar_elongation_p = make_polynomial(297.8501921, 445267.1114034, -0.0018819, 1 / 545868.0, -1 / 113065000.0);
    static constexpr auto mean_lunar_longitude_p = make_polynomial(218.3164477, 481267.88123421, -0.0015786, 1 / 538841.0, -1 / 65194000.0);

    angle_t moon_node(double c) {
        return mod(moon_node_p(c), 360);
    }
    angle_t lunar_anomaly(double c) {
        return mod(lunar_anomaly_p(c), 360);
    }
    angle_t solar_anomaly(double c) {
        return mod(solar_anomaly_p(c), 360);
    }
    angle_t lunar_elongation(double c) {
        return mod(lunar_elongation_p(c), 360);
    }
    angle_t mean_lunar_longitude(double c) {
        return mod(mean_lunar_longitude_p(c), 360);
    }

    angle_t lunar_node(const fixed_date_t& date) {
        return interval_modulus(moon_node(julian_centuries(date)), -90, 90);
    }

    angle_t sidereal_lunar_longitude(const moment_t& t) {
        return mod(lunar_longitude(t) - precession(t) + sidereal_start, 360);
    }

    angle_t lunar_phase(const moment_t& t) {
        auto psi = mod(lunar_longitude(t) - solar_longitude(t), 360);
        auto t0 = zero_new_moon;
        auto n = static_cast<int64_t>(std::round((t - t0) / mean_synodic_month));
        auto psi1 = 360 * mod((t - nth_new_moon(n)) / mean_synodic_month, 1);
        if (std::abs(psi - psi1) > 180)
            return psi1;
        else
            return psi;
    }

    moment_t lunar_phase_at_or_before(const angle_t& psi, const moment_t& t) {
        auto T = t - mean_synodic_month * mod(lunar_phase(t) - psi, 360) / 360;
        auto a = T - 2;
        auto b = std::min<double>(t, T + 2);
        return inverse_angular(make_value_function(lunar_phase), psi, a, b);
    }
    moment_t lunar_phase_at_or_after(const angle_t& psi, const moment_t& t) {
        auto T = t - mean_synodic_month * mod(psi - lunar_phase(t), 360) / 360;
        auto a = std::max<double>(t, T - 2);
        auto b = T + 2;
        return inverse_angular(make_value_function(lunar_phase), psi, a, b);
    }

    static constexpr auto lunar_latitude_table = std::array{
        std::make_tuple<int,int,int,int,int>(5128122, 0, 0, 0, 1),
        std::make_tuple<int,int,int,int,int>(280602, 0, 0, 1, 1),
        std::make_tuple<int,int,int,int,int>(277693, 0, 0, 1, -1),
        std::make_tuple<int,int,int,int,int>(173237, 2, 0, 0, -1),
        std::make_tuple<int,int,int,int,int>(55413, 2, 0, -1, 1),
        std::make_tuple<int,int,int,int,int>(46271, 2, 0, -1, -1),
        std::make_tuple<int,int,int,int,int>(32573, 2, 0, 0, 1),
        std::make_tuple<int,int,int,int,int>(17198, 0, 0, 2, 1),
        std::make_tuple<int,int,int,int,int>(9266, 2, 0, 1, -1),
        std::make_tuple<int,int,int,int,int>(8822, 0, 0, 2, -1),
        std::make_tuple<int,int,int,int,int>(8216, 2, -1, 0, -1),
        std::make_tuple<int,int,int,int,int>(4324, 2, 0, -2, -1),
        std::make_tuple<int,int,int,int,int>(4200, 2, 0, 1, 1),
        std::make_tuple<int,int,int,int,int>(-3359, 2, 1, 0, -1),
        std::make_tuple<int,int,int,int,int>(2463, 2, -1, -1, 1),
        std::make_tuple<int,int,int,int,int>(2211, 2, -1, 0, 1),
        std::make_tuple<int,int,int,int,int>(2065, 2, -1, -1, -1),
        std::make_tuple<int,int,int,int,int>(-1870, 0, 1, -1, -1),
        std::make_tuple<int,int,int,int,int>(1828, 4, 0, -1, -1),
        std::make_tuple<int,int,int,int,int>(-1794, 0, 1, 0, 1),
        std::make_tuple<int,int,int,int,int>(-1749, 0, 0, 0, 3),
        std::make_tuple<int,int,int,int,int>(-1565, 0, 1, -1, 1),
        std::make_tuple<int,int,int,int,int>(-1491, 1, 0, 0, 1),
        std::make_tuple<int,int,int,int,int>(-1475, 0, 1, 1, 1),
        std::make_tuple<int,int,int,int,int>(-1410, 0, 1, 1, -1),
        std::make_tuple<int,int,int,int,int>(-1344, 0, 1, 0, -1),
        std::make_tuple<int,int,int,int,int>(-1335, 1, 0, 0, -1),
        std::make_tuple<int,int,int,int,int>(1107, 0, 0, 3, 1),
        std::make_tuple<int,int,int,int,int>(1021, 4, 0, 0, -1),
        std::make_tuple<int,int,int,int,int>(833, 4, 0, -1, 1),
        std::make_tuple<int,int,int,int,int>(777, 0, 0, 1, -3),
        std::make_tuple<int,int,int,int,int>(671, 4, 0, -2, 1),
        std::make_tuple<int,int,int,int,int>(607, 2, 0, 0, -3),
        std::make_tuple<int,int,int,int,int>(596, 2, 0, 2, -1),
        std::make_tuple<int,int,int,int,int>(491, 2, -1, 1, -1),
        std::make_tuple<int,int,int,int,int>(-451, 2, 0, -2, 1),
        std::make_tuple<int,int,int,int,int>(439, 0, 0, 3, -1),
        std::make_tuple<int,int,int,int,int>(422, 2, 0, 2, 1),
        std::make_tuple<int,int,int,int,int>(421, 2, 0, -3, -1),
        std::make_tuple<int,int,int,int,int>(-366, 2, 1, -1, 1),
        std::make_tuple<int,int,int,int,int>(-351, 2, 1, 0, 1),
        std::make_tuple<int,int,int,int,int>(331, 4, 0, 0, 1),
        std::make_tuple<int,int,int,int,int>(315, 2, -1, 1, 1),
        std::make_tuple<int,int,int,int,int>(302, 2, -2, 0, -1),
        std::make_tuple<int,int,int,int,int>(-283, 0, 0, 1, 3),
        std::make_tuple<int,int,int,int,int>(-229, 2, 1, 1, -1),
        std::make_tuple<int,int,int,int,int>(223, 1, 1, 0, -1),
        std::make_tuple<int,int,int,int,int>(223, 1, 1, 0, 1),
        std::make_tuple<int,int,int,int,int>(-220, 0, 1, -2, -1),
        std::make_tuple<int,int,int,int,int>(-220, 2, 1, -1, -1),
        std::make_tuple<int,int,int,int,int>(-185, 1, 0, 1, 1),
        std::make_tuple<int,int,int,int,int>(181, 2, -1, -2, -1),
        std::make_tuple<int,int,int,int,int>(-177, 0, 1, 2, 1),
        std::make_tuple<int,int,int,int,int>(176, 4, 0, -2, -1),
        std::make_tuple<int,int,int,int,int>(166, 4, -1, -1, -1),
        std::make_tuple<int,int,int,int,int>(-164, 1, 0, 1, -1),
        std::make_tuple<int,int,int,int,int>(132, 4, 0, 1, -1),
        std::make_tuple<int,int,int,int,int>(-119, 1, 0, -1, -1),
        std::make_tuple<int,int,int,int,int>(115, 4, -1, 0, -1),
        std::make_tuple<int,int,int,int,int>(107, 2, -2, 0, 1),
    };

    angle_t lunar_latitude(const moment_t& t) {
        auto c = julian_centuries(t);
        auto L1 = mean_lunar_longitude(c);
        auto D = lunar_elongation(c);
        auto M = solar_anomaly(c);
        auto M1 = lunar_anomaly(c);
        auto F = moon_node(c);
        auto E = lunar_E_p(c);
        double b{};
        for (auto& [v, w, x, y, z] : lunar_latitude_table)
            b += v * std::pow(E, std::abs(x)) * sin(w * D + x * M + y * M1 + z * F);
        auto venus = 175 * (sin(119.75 + c * 131.849 + F) * sin(119.75 + c * 131.849 - F));
        auto flat_earth = -2235 * sin(L1) + 127 * sin(L1 - M1) - 115 * sin(L1 + M1);
        auto extra = 382 * sin(313.45 + c * 481266.484);
        return (b + venus + flat_earth + extra) * 1e-6;
    }

    angle_t lunar_altitude(const moment_t& t, const location& location) {
        auto &psi = location.latitude;
        auto &phi = location.longitude;
        auto l = lunar_longitude(t);
        auto b = lunar_latitude(t);
        auto a = right_ascension(t, b, l);
        auto d = declination(t, b, l);
        auto O0 = sidereal_from_moment(t);
        auto H = mod(O0 + phi - a, 360);
        auto tmp = sin(psi) * sin(d) + cos(psi) * cos(d) * cos(H);
        auto altitude = asin(tmp);
        return interval_modulus(altitude, -180, 180);
    }

    static constexpr auto lunar_distance_table = std::array{
        std::make_tuple<int,int,int,int,int>(-20905355, 0, 0, 1, 0),
        std::make_tuple<int,int,int,int,int>(-3699111, 2, 0, -1, 0),
        std::make_tuple<int,int,int,int,int>(-2955968, 2, 0, 0, 0),
        std::make_tuple<int,int,int,int,int>(-569925, 0, 0, 2, 0),
        std::make_tuple<int,int,int,int,int>(48888, 0, 1, 0, 0),
        std::make_tuple<int,int,int,int,int>(-3149, 0, 0, 0, 2),
        std::make_tuple<int,int,int,int,int>(246158, 2, 0, -2, 0),
        std::make_tuple<int,int,int,int,int>(-152138, 2, -1, -1, 0),
        std::make_tuple<int,int,int,int,int>(-170733, 2, 0, 1, 0),
        std::make_tuple<int,int,int,int,int>(-204586, 2, -1, 0, 0),
        std::make_tuple<int,int,int,int,int>(-129620, 0, 1, -1, 0),
        std::make_tuple<int,int,int,int,int>(108743, 1, 0, 0, 0),
        std::make_tuple<int,int,int,int,int>(104755, 0, 1, 1, 0),
        std::make_tuple<int,int,int,int,int>(10321, 2, 0, 0, -2),
        std::make_tuple<int,int,int,int,int>(0, 0, 0, 1, 2),
        std::make_tuple<int,int,int,int,int>(79661, 0, 0, 1, -2),
        std::make_tuple<int,int,int,int,int>(-34782, 4, 0, -1, 0),
        std::make_tuple<int,int,int,int,int>(-23210, 0, 0, 3, 0),
        std::make_tuple<int,int,int,int,int>(-21636, 4, 0, -2, 0),
        std::make_tuple<int,int,int,int,int>(24208, 2, 1, -1, 0),
        std::make_tuple<int,int,int,int,int>(30824, 2, 1, 0, 0),
        std::make_tuple<int,int,int,int,int>(-8379, 1, 0, -1, 0),
        std::make_tuple<int,int,int,int,int>(-16675, 1, 1, 0, 0),
        std::make_tuple<int,int,int,int,int>(-12831, 2, -1, 1, 0),
        std::make_tuple<int,int,int,int,int>(-10445, 2, 0, 2, 0),
        std::make_tuple<int,int,int,int,int>(-11650, 4, 0, 0, 0),
        std::make_tuple<int,int,int,int,int>(14403, 2, 0, -3, 0),
        std::make_tuple<int,int,int,int,int>(-7003, 0, 1, -2, 0),
        std::make_tuple<int,int,int,int,int>(0, 2, 0, -1, 2),
        std::make_tuple<int,int,int,int,int>(10056, 2, -1, -2, 0),
        std::make_tuple<int,int,int,int,int>(6322, 1, 0, 1, 0),
        std::make_tuple<int,int,int,int,int>(-9884, 2, -2, 0, 0),
        std::make_tuple<int,int,int,int,int>(5751, 0, 1, 2, 0),
        std::make_tuple<int,int,int,int,int>(0, 0, 2, 0, 0),
        std::make_tuple<int,int,int,int,int>(-4950, 2, -2, -1, 0),
        std::make_tuple<int,int,int,int,int>(4130, 2, 0, 1, -2),
        std::make_tuple<int,int,int,int,int>(0, 2, 0, 0, 2),
        std::make_tuple<int,int,int,int,int>(-3958, 4, -1, -1, 0),
        std::make_tuple<int,int,int,int,int>(0, 0, 0, 2, 2),
        std::make_tuple<int,int,int,int,int>(3258, 3, 0, -1, 0),
        std::make_tuple<int,int,int,int,int>(2616, 2, 1, 1, 0),
        std::make_tuple<int,int,int,int,int>(-1897, 4, -1, -2, 0),
        std::make_tuple<int,int,int,int,int>(-2117, 0, 2, -1, 0),
        std::make_tuple<int,int,int,int,int>(2354, 2, 2, -1, 0),
        std::make_tuple<int,int,int,int,int>(0, 2, 1, -2, 0),
        std::make_tuple<int,int,int,int,int>(0, 2, -1, 0, -2),
        std::make_tuple<int,int,int,int,int>(-1423, 4, 0, 1, 0),
        std::make_tuple<int,int,int,int,int>(-1117, 0, 0, 4, 0),
        std::make_tuple<int,int,int,int,int>(-1571, 4, -1, 0, 0),
        std::make_tuple<int,int,int,int,int>(-1739, 1, 0, -2, 0),
        std::make_tuple<int,int,int,int,int>(0, 2, 1, 0, -2),
        std::make_tuple<int,int,int,int,int>(-4421, 0, 0, 2, -2),
        std::make_tuple<int,int,int,int,int>(0, 1, 1, 1, 0),
        std::make_tuple<int,int,int,int,int>(0, 3, 0, -2, 0),
        std::make_tuple<int,int,int,int,int>(0, 4, 0, -3, 0),
        std::make_tuple<int,int,int,int,int>(0, 2, -1, 2, 0),
        std::make_tuple<int,int,int,int,int>(1165, 0, 2, 1, 0),
        std::make_tuple<int,int,int,int,int>(0, 1, 1, -1, 0),
        std::make_tuple<int,int,int,int,int>(0, 2, 0, 3, 0),
        std::make_tuple<int,int,int,int,int>(8752, 2, 0, -1, -2),
    };

    double lunar_distance(const moment_t& t) {
        auto c = julian_centuries(t);
        auto D = lunar_elongation(c);
        auto M = solar_anomaly(c);
        auto M1 = lunar_anomaly(c);
        auto F = moon_node(c);
        auto E = lunar_E_p(c);
        double correction{};
        for (auto& [v, w, x, y, z] : lunar_distance_table)
            correction += v * std::pow(E, std::abs(x)) * cos(w * D + x * M + y * M1 + z * F);
        return 385000560 + correction;
    }

    angle_t lunar_parallax(const moment_t& t, const location& location) {
        auto geo = lunar_altitude(t, location);
        auto D = lunar_distance(t);
        auto alt = 3878140 / D;
        auto arg = alt * cos(geo);
        return asin(arg);
    }

    angle_t topocentric_lunar_altitude(const moment_t& t, const location& location) {
        return lunar_altitude(t, location) - lunar_parallax(t, location);
    }

    moment_t approx_moment_of_depression(const moment_t& t, const location& location, const angle_t& a, bool early) {
        auto m = sine_offset(t, location, a);
        auto date = t.to_fixed_date();
        auto alt = a >= 0_deg ? static_cast<double>(early ? date : date.add(1)) : date + 0.5;
        double value;
        if (std::abs(m) > 1)
            value = sine_offset(alt, location, a);
        else
            value = m;
        auto offset = interval_modulus(asin(value) / angle_t::full_circle, -0.5, 0.5);
        if (std::abs(value) > 1)
            throw std::runtime_error("value is greater than 1");
        return local_from_apparent(date + (early ? 0.25 - offset : 0.75 + offset), location);
    }

    static constexpr auto s30 = moment_t::hms(0, 0, 30);

    double sine_offset(const moment_t& t, const location& location, const angle_t& a) {
        auto &psi = location.latitude;
        auto t1 = universal_from_local(t, location);
        auto d = declination(t1, 0, solar_longitude(t1));
        return tan(psi) * tan(d) + sin(a) / (cos(d) * cos(psi));
    }

    moment_t moment_of_depression(const moment_t& approx, const location& location, const angle_t& a, bool early) {
        auto t = approx_moment_of_depression(approx, location, a, early);
        if ((approx - t) < static_cast<double>(s30))
            return t;
        else
            return moment_of_depression(t, location, a, early);
    }

    moment_t dawn(const fixed_date_t& date, const location& location, const angle_t& a) {
        auto result = moment_of_depression(date + 0.25, location, a, morning);
        return standard_from_local(result, location);
    }
    moment_t dusk(const fixed_date_t& date, const location& location, const angle_t& a) {
        auto result = moment_of_depression(date + 0.75, location, a, evening);
        return standard_from_local(result, location);
    }
    angle_t refraction(const moment_t& t, const location& location) {
        constexpr auto R{ 6.372e6 };
        auto h = std::max(0.0, location.elevation);
        auto dip = acos(R / (R + h));
        return 34 / 6e1 + dip + 19 * std::sqrt(h) / 3600;
    }
    moment_t sunrise(const fixed_date_t& date, const location& location) {
        auto a = refraction(date + 0.25, location) + angle_t::dms(0, 16);
        return dawn(date, location, a);
    }
    moment_t sunset(const fixed_date_t& date, const location& location) {
        auto a = refraction(date + 0.75, location) + angle_t::dms(0, 16);
        return dusk(date, location, a);
    }
    moment_t jewish_sabbath_ends(const fixed_date_t& date, const location& location) {
        return dusk(date, location, angle_t::dms(7, 5));
    }
    moment_t jewish_dusk(const fixed_date_t& date, const location& location) {
        return dusk(date, location, angle_t::dms(4, 40));
    }
    angle_t observed_lunar_altitude(const moment_t& t, const location& location) {
        return topocentric_lunar_altitude(t, location) + refraction(t, location) + angle_t::dms(0, 16);
    }
    moment_t moonrise(const fixed_date_t& date, const location& location) {
        auto t = universal_from_standard(date, location);
        auto waning = lunar_phase(t) > angle_t::half_circle;
        auto alt = observed_lunar_altitude(t, location);
        auto lat = location.latitude;
        auto offset = alt / angle_t{ 4 * (90 - std::abs(lat)) };
        auto approx = waning ? (offset > 0 ? t + 1 - offset : t - offset) : t + 0.5 + offset;
        auto rise = binary_search(
            [&location](const double& x) { return observed_lunar_altitude(x, location) > 0_deg; },
            [](const double& l, const double& u) { return (u - l) < static_cast<double>(moment_t::hms(0, 1)); },
            approx - 0.25,
            approx + 0.25
        );
        if (rise >= t + 1)
            throw std::runtime_error("is in next day");
        return std::max<double>(standard_from_universal(rise, location), date);
    }
    moment_t moonset(const fixed_date_t& date, const location& location)
    {
        auto t = universal_from_standard(date, location);
        auto waxing = lunar_phase(t) < angle_t::half_circle;
        auto alt = observed_lunar_altitude(t, location);
        auto lat = location.latitude;
        auto offset = alt / angle_t{ 4 * (90 - std::abs(lat)) };
        auto approx = waxing ? (offset > 0 ? t + offset : t + 1 + offset) : t + 0.5 - offset;
        auto rise = binary_search(
            [&location](const double& x) { return observed_lunar_altitude(x, location) < 0_deg; },
            [](const double& l, const double& u) { return (u - l) < static_cast<double>(moment_t::hms(0, 1)); },
            approx - 0.25,
            approx + 0.25
        );
        if (rise >= t + 1)
            throw std::runtime_error("is in next day");
        return std::max<double>(standard_from_universal(rise, location), date);
    }
    moment_t local_zero_hour(const moment_t& t) {
        auto date = t.to_fixed_date();
        return local_from_standard(dusk(date, locations::padua, angle_t::dms(0, 16)) + moment_t::hms(0, 30), locations::padua);
    }
    moment_t local_from_italian(const moment_t& t) {
        auto date = t.to_fixed_date();
        auto z = local_zero_hour(t - 1);
        return t - date + z;
    }
    moment_t italian_from_local(const moment_t& t) {
        auto date = t.to_fixed_date();
        auto z0 = local_zero_hour(t - 1);
        auto z = local_zero_hour(t);
        if (t > z)
            return t + date + 1 - z;
        else
            return t + date - z0;
    }

    moment_t daytime_temporal_hour(const fixed_date_t& date, const location& location) {
        return (sunset(date, location) - sunrise(date, location)) / 12;
    }
    moment_t nighttime_temporal_hour(const fixed_date_t& date, const location& location) {
        return (sunrise(date + 1, location) - sunset(date, location)) / 12;
    }
    moment_t standard_from_sundial(const moment_t& t, const location& location) {
        auto date = t.to_fixed_date();
        auto hour = 24 * t.time();
        double a, b, h;
        if (range_closed_between<double>(6, 18)(hour))
        {
            h = daytime_temporal_hour(date, location);
            a = sunrise(date, location);
            b = (hour - 6);
        }
        else if (hour < 6)
        {
            h = nighttime_temporal_hour(date - 1, location);
            a = sunset(date - 1, location);
            b = (hour + 6);
        }
        else {
            h = nighttime_temporal_hour(date, location);
            a = sunset(date, location);
            b = (hour - 18);
        }
        return a + b * h;
    }
    moment_t jewish_morning_end(const fixed_date_t& date, const location& location) {
        return standard_from_sundial(date + 1e1 / 24, location);
    }
    moment_t asr(const fixed_date_t& date, const location& location) {
        auto noon = midday(date, location);
        auto psi = location.latitude;
        auto d = declination(noon, 0, solar_longitude(noon));
        auto altitude = asin(cos(d) * cos(psi) + sin(d) * sin(psi));
        auto h = interval_modulus(arctan(tan(altitude), 2 * tan(altitude) + 1), -90, 90);
        if (altitude <= 0_deg)
            throw std::runtime_error("altitude is negative");
        return dusk(date, location, -h);
    }
    moment_t alt_asr(const fixed_date_t& date, const location& location) {
        auto noon = midday(date, location);
        auto psi = location.latitude;
        auto d = declination(noon, 0, solar_longitude(noon));
        auto altitude = asin(cos(d) * cos(psi) + sin(d) * sin(psi));
        auto h = interval_modulus(arctan(tan(altitude), tan(altitude) + 1), -90, 90);
        if (altitude <= 0_deg)
            throw std::runtime_error("altitude is negative");
        return dusk(date, location, -h);
    }
    angle_t arc_of_light(const moment_t& t) {
        return acos(cos(lunar_latitude(t)) * cos(lunar_phase(t)));
    }
    moment_t simple_best_view(const fixed_date_t& date, const location& location) {
        double best;
        try {
            best = dusk(date, location, 4.5);
        }
        catch (std::exception&) {
            best = date + 1;
        }
        return universal_from_standard(best, location);
    }
    bool shaukat_criterion(const fixed_date_t& date, const location& location) {
        auto t = simple_best_view(date - 1, location);
        auto phase = lunar_phase(t);
        auto h = lunar_altitude(t, location);
        auto ARCL = arc_of_light(t);
        return range_opened_between<angle_t>(calendrical::lunar_phase::NEW, calendrical::lunar_phase::FIRST_QUARTER).contains(phase)
            && range_closed_between<double>(10.6, 90).contains(ARCL)
            && h > 4.1_deg;
    }
    angle_t arg_of_vision(const moment_t& t, const location& location) {
        return lunar_altitude(t, location) - solar_altitude(t, location);
    }
    moment_t bruin_best_view(const fixed_date_t& date, const location& location) {
        double best;
        try {
            auto sun = sunset(date, location);
            auto moon = moonset(date, location);
            best = (5 * sun + 4 * moon) / 9;
        }
        catch (std::exception&)
        {
            best = date + 1;
        }
        return universal_from_standard(best, location);
    }

    static constexpr auto yallop_criterion_p = make_polynomial(11.8371, -6.3226, 0.7319, -0.1018);

    bool yallop_criterion(const fixed_date_t& date, const location& location)
    {
        constexpr auto e{ -0.14 };
        auto t = bruin_best_view(date - 1, location);
        auto phase = lunar_phase(t);
        auto D = lunar_semi_diameter(t, location);
        auto ARCL = arc_of_light(t);
        auto W = D * (1 - cos(ARCL));
        auto ARCV = arg_of_vision(t, location);
        auto q1 = yallop_criterion_p(W);
        return range_opened_between<angle_t>(calendrical::lunar_phase::NEW, calendrical::lunar_phase::FIRST_QUARTER).contains(phase)
            && ARCV > angle_t(q1 + e);
    }
    double lunar_semi_diameter(const moment_t& t, const location& location) {
        auto h = lunar_altitude(t, location);
        auto p = lunar_parallax(t, location);
        return 0.27245 * p * (sin(h) * sin(p) + 1);
    }
    angle_t lunar_diameter(const moment_t& t) {
        return 1792367000 / (9 * lunar_distance(t));
    }

    fixed_date_t phasis_on_or_before(const fixed_date_t& date, const location& location) {
        auto moon = lunar_phase_at_or_after(calendrical::lunar_phase::NEW, date).to_fixed_date();
        auto age = date - moon;
        auto T = age <= 3 && !visible_crescent(date, location) ? moon.subtract(29) : date;
        return least_passing(std::bind(visible_crescent, std::placeholders::_1, location), T);
    }
    fixed_date_t phasis_on_or_after(const fixed_date_t& date, const location& location) {
        auto moon = lunar_phase_at_or_after(calendrical::lunar_phase::NEW, date).to_fixed_date();
        auto age = date - moon;
        auto T = 4 <= age || visible_crescent(date - 1, location) ? moon.add(29) : date;
        return least_passing(std::bind(visible_crescent, std::placeholders::_1, location), T);
    }
    moment_t moonlag(const fixed_date_t& date, const location& location) {
        moment_t sun = sunset(date, location);
        moment_t moon;
        try {
            moon = moonset(date, location);
        }
        catch (std::exception&) {
            return 1;
        }
        return moon - sun;
    }
    int32_t month_length(const fixed_date_t& date, const location& location) {
        auto moon = phasis_on_or_after(date.add(1), location);
        auto prev = phasis_on_or_before(date, location);
        return static_cast<int32_t>(moon - prev);
    }
    bool is_early_month(const fixed_date_t& date, const location& location) {
        auto start = phasis_on_or_before(date, location);
        auto prev = start.subtract(15);
        auto ml = month_length(prev, location);
        return (date - start) >= 30
            || ml > 30
            || (ml == 30 && is_early_month(prev, location));
    }
}