#include <gtest/gtest.h>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>

#include "angle.hpp"
#include "fixed_date.hpp"
#include "lunar_phase.hpp"
#include "moment.hpp"
#include "operator_traits.hpp"
#include "sample_fixed_dates.hpp"
#include "season.hpp"

// Compile-time checks of the unit algebra of angle_t, fixed_date_t and moment_t (issue #4).
// Later parts of #4 extend these sections when they make the numeric conversions explicit.
namespace tests::type_safety {
    using namespace calendrical;
    using namespace tests::operator_traits;

    // --- moment_t <-> fixed_date_t -------------------------------------------------------
    // A date converts to the moment of its midnight, exactly for |day| <= 2^53 (beyond that
    // double rounds to the nearest representable day), so that direction is implicit; the
    // opposite one drops the time of day and stays explicit.
    static_assert(std::is_convertible_v<fixed_date_t, moment_t>);
    static_assert(!std::is_convertible_v<moment_t, fixed_date_t>);
    static_assert(std::is_constructible_v<fixed_date_t, moment_t>);

    static_assert(can_compare<moment_t, fixed_date_t>);
    static_assert(can_compare<fixed_date_t, moment_t>);
    static_assert(can_equal<moment_t, fixed_date_t>);
    static_assert(can_equal<fixed_date_t, moment_t>);
    static_assert(subtract_yields<moment_t, fixed_date_t, moment_t>);
    // Sum of two points, accepted on purpose until a dedicated duration type exists.
    static_assert(add_yields<moment_t, fixed_date_t, moment_t>);

    static_assert(subtract_yields<fixed_date_t, fixed_date_t, int64_t>);
    static_assert(add_yields<fixed_date_t, int, fixed_date_t>);
    static_assert(add_yields<int, fixed_date_t, fixed_date_t>);
    static_assert(subtract_yields<fixed_date_t, int, fixed_date_t>);

    static_assert(add_yields<moment_t, moment_t, moment_t>);
    static_assert(add_yields<moment_t, double, moment_t>);
    static_assert(add_yields<double, moment_t, moment_t>);
    static_assert(subtract_yields<moment_t, moment_t, moment_t>);

    // end_of_time is preserved by the implicit conversion, also in constant expressions.
    static_assert(moment_t{ fixed_date_t::end_of_time } == moment_t::end_of_time);
    inline constexpr moment_t converted_end_of_time = fixed_date_t::end_of_time;
    static_assert(converted_end_of_time == moment_t::end_of_time);
    static_assert(moment_t{ 10_rd } == 10_m);
    static_assert(10_rd < 10.5_m && 10.5_m < 11_rd);

    // --- angle_t ---------------------------------------------------------------------------
    static_assert(add_yields<angle_t, angle_t, angle_t>);
    static_assert(subtract_yields<angle_t, angle_t, angle_t>);
    static_assert(multiply_yields<angle_t, double, angle_t>);
    static_assert(multiply_yields<double, angle_t, angle_t>);
    static_assert(divide_yields<angle_t, double, angle_t>);
    static_assert(divide_yields<angle_t, angle_t, double>);

    // Numeric conversion is explicit (issue #19); implicit conversions from angle enums are preserved.
    static_assert(!std::is_convertible_v<angle_t, double>);
    static_assert(std::is_constructible_v<double, angle_t>);
    static_assert(std::is_convertible_v<season, angle_t>);
    static_assert(std::is_convertible_v<lunar_phase, angle_t>);

    // Angles cannot mix additively with calendar dates or moments (e.g. 10_rd + 20_deg).
    static_assert(!can_add<fixed_date_t, angle_t>);
    static_assert(!can_add<angle_t, fixed_date_t>);
    static_assert(!can_subtract<fixed_date_t, angle_t>);
    static_assert(!can_subtract<angle_t, fixed_date_t>);
    static_assert(!can_add<decltype(10_rd), decltype(20_deg)>);
    static_assert(!can_add<decltype(20_deg), decltype(10_rd)>);
    static_assert(!can_subtract<decltype(10_rd), decltype(20_deg)>);
    static_assert(!can_subtract<decltype(20_deg), decltype(10_rd)>);

    static_assert(!can_add<moment_t, angle_t>);
    static_assert(!can_add<angle_t, moment_t>);
    static_assert(!can_subtract<moment_t, angle_t>);
    static_assert(!can_subtract<angle_t, moment_t>);
    static_assert(!can_add<decltype(10_m), decltype(20_deg)>);
    static_assert(!can_add<decltype(20_deg), decltype(10_m)>);
    static_assert(!can_subtract<decltype(10_m), decltype(20_deg)>);
    static_assert(!can_subtract<decltype(20_deg), decltype(10_m)>);

    // --- typed helpers keep the unit -------------------------------------------------------
    static_assert(std::same_as<decltype(mod(angle_t{}, 360)), angle_t>);
    static_assert(std::same_as<decltype(mod(angle_t{}, 360.0)), angle_t>);
    static_assert(std::same_as<decltype(mod(angle_t{}, angle_t{})), angle_t>);
    static_assert(std::same_as<decltype(interval_modulus(angle_t{}, angle_t{}, angle_t{})), angle_t>);
    static_assert(std::same_as<decltype(floor_div(angle_t{}, angle_t{})), double>);
    static_assert(std::same_as<decltype(floor(angle_t{})), angle_t>);
    static_assert(std::same_as<decltype(ceil(angle_t{})), angle_t>);
    static_assert(std::same_as<decltype(abs(angle_t{})), angle_t>);
    static_assert(std::same_as<decltype(floor(moment_t{})), fixed_date_t>);
    static_assert(std::same_as<decltype(ceil(moment_t{})), fixed_date_t>);
    static_assert(std::same_as<decltype(round(moment_t{})), fixed_date_t>);
    static_assert(std::same_as<decltype(abs(moment_t{})), moment_t>);

    static_assert(abs(-5_deg) == 5_deg);
    static_assert(abs(-2.5_m) == 2.5_m);

    // --- raw value accessors ---------------------------------------------------------------
    static_assert(std::same_as<decltype(angle_t{}.degrees()), double>);
    static_assert(std::same_as<decltype(angle_t{}.day_fraction()), double>);
    static_assert(std::same_as<decltype(day_fraction(angle_t{})), double>);
    static_assert(std::same_as<decltype(moment_t{}.days()), double>);
    static_assert(std::same_as<decltype(fixed_date_t{}.value()), int64_t>);
    static_assert((12.5_deg).degrees() == 12.5);
    static_assert((90_deg).day_fraction() == 0.25);
    static_assert(day_fraction(180_deg) == 0.5);
    static_assert((3.25_m).days() == 3.25);
    static_assert((7_rd).value() == 7);

    TEST(type_safety, end_of_time_round_trips_between_moment_and_date) {
        EXPECT_EQ(moment_t{ fixed_date_t::end_of_time }, moment_t::end_of_time);
        EXPECT_EQ(moment_t::end_of_time.to_fixed_date(), fixed_date_t::end_of_time);
        EXPECT_EQ(static_cast<fixed_date_t>(moment_t::end_of_time), fixed_date_t::end_of_time);
        EXPECT_EQ(floor(moment_t::end_of_time), fixed_date_t::end_of_time);
        EXPECT_EQ(ceil(moment_t::end_of_time), fixed_date_t::end_of_time);
        EXPECT_EQ(round(moment_t::end_of_time), fixed_date_t::end_of_time);
    }

    TEST(type_safety, date_to_moment_is_midnight_and_round_trips) {
        for (const auto& date : dates) {
            const moment_t midnight = date;
            EXPECT_EQ(midnight.days(), static_cast<double>(date.value()));
            EXPECT_EQ(midnight.to_fixed_date(), date);
            EXPECT_EQ(floor(midnight + 0.75), date);
        }
    }

    TEST(type_safety, date_to_moment_is_exact_up_to_2_pow_53) {
        constexpr int64_t exact_limit = int64_t{ 1 } << 53;
        EXPECT_EQ(moment_t{ fixed_date_t{ exact_limit } }.to_fixed_date(), fixed_date_t{ exact_limit });
        EXPECT_EQ(moment_t{ fixed_date_t{ -exact_limit } }.to_fixed_date(), fixed_date_t{ -exact_limit });
        // Not every whole day above 2^53 is a double: the conversion rounds to the nearest one.
        EXPECT_EQ(moment_t{ fixed_date_t{ exact_limit + 1 } }.to_fixed_date(), fixed_date_t{ exact_limit });
    }

    TEST(type_safety, moment_to_date_rejects_values_without_a_date) {
        constexpr double nan = std::numeric_limits<double>::quiet_NaN();
        constexpr double inf = std::numeric_limits<double>::infinity();
        constexpr double two_pow_63 = 0x1p63;
        const std::array<moment_t, 4> invalid{
            moment_t{ nan },
            moment_t{ -inf },
            moment_t{ two_pow_63 },
            // INT64_MAX - 1 is not a double; it converts to 2^63, which is out of range.
            moment_t{ fixed_date_t{ std::numeric_limits<int64_t>::max() - 1 } },
        };
        for (const auto& m : invalid) {
            EXPECT_THROW((void)m.to_fixed_date(), std::domain_error) << m;
            EXPECT_THROW((void)floor(m), std::domain_error) << m;
            EXPECT_THROW((void)ceil(m), std::domain_error) << m;
            EXPECT_THROW((void)round(m), std::domain_error) << m;
        }

        const moment_t lowest{ -two_pow_63 };
        EXPECT_EQ(floor(lowest), fixed_date_t{ std::numeric_limits<int64_t>::min() });
        EXPECT_EQ(ceil(lowest), fixed_date_t{ std::numeric_limits<int64_t>::min() });
        EXPECT_EQ(round(lowest), fixed_date_t{ std::numeric_limits<int64_t>::min() });
        const moment_t highest{ std::nextafter(two_pow_63, 0.0) };
        const fixed_date_t highest_date{ static_cast<int64_t>(std::nextafter(two_pow_63, 0.0)) };
        EXPECT_EQ(floor(highest), highest_date);
        EXPECT_EQ(ceil(highest), highest_date);
        EXPECT_EQ(round(highest), highest_date);

        EXPECT_EQ(floor(moment_t{ inf }), fixed_date_t::end_of_time);
        EXPECT_EQ(ceil(moment_t{ inf }), fixed_date_t::end_of_time);
        EXPECT_EQ(round(moment_t{ inf }), fixed_date_t::end_of_time);
    }

    TEST(type_safety, moment_and_date_mix_chronologically) {
        EXPECT_LT(10_rd, 10.5_m);
        EXPECT_LT(10.5_m, 11_rd);
        EXPECT_EQ(10_rd, 10_m);
        EXPECT_EQ(10.75_m - 10_rd, 0.75_m);
    }

    TEST(type_safety, moment_rounding_helpers) {
        EXPECT_EQ(floor(10.75_m), 10_rd);
        EXPECT_EQ(floor(-0.5_m), fixed_date_t{ -1 });
        EXPECT_EQ(ceil(10.25_m), 11_rd);
        EXPECT_EQ(ceil(10_m), 10_rd);
        EXPECT_EQ(round(10.5_m), 11_rd);
        EXPECT_EQ(round(10.4_m), 10_rd);
        EXPECT_EQ(abs(-2.5_m), 2.5_m);
    }

    TEST(type_safety, angle_helpers) {
        EXPECT_EQ(mod(-30_deg, 360), 330_deg);
        EXPECT_EQ(mod(725_deg, 360), 5_deg);
        EXPECT_EQ(mod(-30_deg, 360_deg), 330_deg);
        EXPECT_EQ(interval_modulus(270_deg, -180_deg, 180_deg), -90_deg);
        EXPECT_EQ(floor_div(95_deg, 30_deg), 3.0);
        EXPECT_EQ(floor_div(-5_deg, 30_deg), -1.0);
        EXPECT_EQ(floor(12.75_deg), 12_deg);
        EXPECT_EQ(ceil(12.25_deg), 13_deg);
        EXPECT_EQ(abs(-5_deg), 5_deg);
    }

    TEST(type_safety, angle_day_fraction) {
        EXPECT_DOUBLE_EQ(day_fraction(0_deg), 0.0);
        EXPECT_DOUBLE_EQ(day_fraction(90_deg), 0.25);
        EXPECT_DOUBLE_EQ(day_fraction(180_deg), 0.5);
        EXPECT_DOUBLE_EQ(day_fraction(360_deg), 1.0);
        EXPECT_DOUBLE_EQ((270_deg).day_fraction(), 0.75);
    }

    TEST(type_safety, angle_enum_implicit_conversion) {
        const angle_t spring = season::spring;
        EXPECT_EQ(spring, 0_deg);
        const angle_t summer = season::summer;
        EXPECT_EQ(summer, 90_deg);
        const angle_t full = lunar_phase::full_moon;
        EXPECT_EQ(full, 180_deg);
    }
}
