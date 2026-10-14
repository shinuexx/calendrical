#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>
#include <algorithm>

#include "primitives.hpp"
#include "types.hpp"
#include "angle.hpp"
#include "season.hpp"
#include "lunar_phase.hpp"
#include "weekday.hpp"
#include "month.hpp"
#include "interval.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::primitives;

    static constexpr std::array expected_julian_days{
        1507231.5,
        1660037.5,
        1746893.5,
        1770641.5,
        1892731.5,
        1931579.5,
        1974851.5,
        2091164.5,
        2121509.5,
        2155779.5,
        2174029.5,
        2191584.5,
        2195261.5,
        2229274.5,
        2245580.5,
        2266100.5,
        2288542.5,
        2290901.5,
        2323140.5,
        2334848.5,
        2348020.5,
        2366978.5,
        2385648.5,
        2392825.5,
        2416223.5,
        2425848.5,
        2430266.5,
        2430833.5,
        2431004.5,
        2448698.5,
        2450138.5,
        2465737.5,
        2486076.5
    };

    static constexpr std::array<int64_t, 33> expected_modified_julian_days{
        -892769,
        -739963,
        -653107,
        -629359,
        -507269,
        -468421,
        -425149,
        -308836,
        -278491,
        -244221,
        -225971,
        -208416,
        -204739,
        -170726,
        -154420,
        -133900,
        -111458,
        -109099,
        -76860,
        -65152,
        -51980,
        -33022,
        -14352,
        -7175,
        16223,
        25848,
        30266,
        30833,
        31004,
        48698,
        50138,
        65737,
        86076
    };

    static constexpr std::array<double, 33> expected_unix_time{
        -80641958400,
        -67439520000,
        -59935161600,
        -57883334400,
        -47334758400,
        -43978291200,
        -40239590400,
        -30190147200,
        -27568339200,
        -24607411200,
        -23030611200,
        -21513859200,
        -21196166400,
        -18257443200,
        -16848604800,
        -15075676800,
        -13136688000,
        -12932870400,
        -10147420800,
        -9135849600,
        -7997788800,
        -6359817600,
        -4746729600,
        -4126636800,
        -2105049600,
        -1273449600,
        -891734400,
        -842745600,
        -827971200,
        700790400,
        825206400,
        2172960000,
        3930249600,
    };

    class julian_days_test : public helpers::input_output_test<fixed_date_t, double>{};
    class modified_julian_days_test : public helpers::input_output_test<fixed_date_t, int64_t>{};
    class unix_time_test : public helpers::input_output_test<fixed_date_t, double>{};

    TEST_P(julian_days_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_DOUBLE_EQ(moment_from_jd(output), moment_t{ input }) << output << " : " << moment_from_jd(output) << " : " << input << std::endl;
    }
    TEST_P(modified_julian_days_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(fixed_from_mjd(output), input) << output << " : " << fixed_from_mjd(output) << " : " << input << std::endl;
    }
    TEST_P(unix_time_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_DOUBLE_EQ(moment_from_unix(output), moment_t{ input }) << output << " : " << moment_from_unix(output) << " : " << input << std::endl;
    }

    TEST_P(julian_days_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_DOUBLE_EQ(jd_from_moment(moment_t{ input }), output) << input << " : " << jd_from_moment(moment_t{ input }) << " : " << output << std::endl;
    }
    TEST_P(modified_julian_days_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(mjd_from_fixed(input), output) << input << " : " << mjd_from_fixed(input) << " : " << output << std::endl;
    }
    TEST_P(unix_time_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_DOUBLE_EQ(unix_from_moment(moment_t{ input }), output) << input << " : " << unix_from_moment(moment_t{ input }) << " : " << output << std::endl;
    }

    INSTANTIATE_TEST_SUITE_P(julian_days, julian_days_test, testing::ValuesIn(helpers::make_test_params_values(dates, expected_julian_days)));
    INSTANTIATE_TEST_SUITE_P(modified_julian_days, modified_julian_days_test, testing::ValuesIn(helpers::make_test_params_values(dates, expected_modified_julian_days)));
    INSTANTIATE_TEST_SUITE_P(unix_time, unix_time_test, testing::ValuesIn(helpers::make_test_params_values(dates, expected_unix_time)));

    TEST(holiday_types, equality_considers_name_and_date) {
        using namespace calendrical;
        holiday_t h1{"Christmas", 100_rd};
        holiday_t h2{"Christmas", 100_rd};
        holiday_t h3{"Other", 100_rd};
        holiday_t h4{"Christmas", 200_rd};

        EXPECT_EQ(h1, h2);
        EXPECT_NE(h1, h3);
        EXPECT_NE(h1, h4);
    }

    TEST(holiday_types, date_only_less) {
        using namespace calendrical;
        holiday_t earlier_z{"Zeta Holiday", 10_rd};
        holiday_t later_a{"Alpha Holiday", 20_rd};

        // Default <=> is chronological, ties broken by name
        EXPECT_TRUE(earlier_z < later_a);
        EXPECT_TRUE((holiday_t{"Alpha", 10_rd} < holiday_t{"Beta", 10_rd}));

        // date_only_less_t sorts strictly by date
        holiday_t::date_only_less_t cmp;
        EXPECT_TRUE(cmp(earlier_z, later_a));
        EXPECT_FALSE(cmp(later_a, earlier_z));

        std::vector<holiday_t> holidays{later_a, earlier_z};
        std::ranges::sort(holidays, holiday_t::date_only_less_t{});
        EXPECT_EQ(holidays[0], earlier_z);
        EXPECT_EQ(holidays[1], later_a);
    }

    TEST(angle_types, explicit_construction_prevents_implicit_conversion_from_numbers) {
        using namespace calendrical;
        static_assert(!std::is_convertible_v<double, angle_t>);
        static_assert(!std::is_convertible_v<int, angle_t>);
        static_assert(std::is_constructible_v<angle_t, double>);
        static_assert(std::is_constructible_v<angle_t, int>);

        angle_t a1{45.0};
        angle_t a2 = 45_deg;
        EXPECT_EQ(a1, a2);
    }

    TEST(angle_types, angle_enum_concept_and_conversions) {
        using namespace calendrical;
        // Verify concept satisfactions
        static_assert(angle_enum<season>);
        static_assert(angle_enum<lunar_phase>);
        static_assert(!angle_enum<weekday>);
        static_assert(!angle_enum<month>);
        static_assert(!angle_enum<double>);

        // Verify constructibility / convertibility
        static_assert(std::is_convertible_v<season, angle_t>);
        static_assert(std::is_convertible_v<lunar_phase, angle_t>);
        static_assert(!std::is_convertible_v<weekday, angle_t>);
        static_assert(!std::is_convertible_v<month, angle_t>);
        static_assert(!std::is_constructible_v<angle_t, weekday>);
        static_assert(!std::is_constructible_v<angle_t, month>);

        // Verify degree values of season
        EXPECT_EQ(angle_t(season::spring), 0_deg);
        EXPECT_EQ(angle_t(season::summer), 90_deg);
        EXPECT_EQ(angle_t(season::autumn), 180_deg);
        EXPECT_EQ(angle_t(season::winter), 270_deg);

        // Verify degree values of lunar_phase
        EXPECT_EQ(angle_t(lunar_phase::new_moon), 0_deg);
        EXPECT_EQ(angle_t(lunar_phase::first_quarter), 90_deg);
        EXPECT_EQ(angle_t(lunar_phase::full_moon), 180_deg);
        EXPECT_EQ(angle_t(lunar_phase::last_quarter), 270_deg);
    }

    TEST(angle_types, arithmetic_operations) {
        using namespace calendrical;
        EXPECT_EQ(2 * 45_deg, 90_deg);
        EXPECT_EQ(45_deg * 2, 90_deg);
        EXPECT_EQ(90_deg / 2, 45_deg);
        EXPECT_DOUBLE_EQ(90_deg / 45_deg, 2.0);
        EXPECT_EQ(30_deg + 60_deg, 90_deg);
        EXPECT_EQ(90_deg - 30_deg, 60_deg);
        EXPECT_EQ(-90_deg, angle_t{-90});
    }

    template <typename From, typename To>
    concept interval_castable = requires(calendrical::interval<From> i) {
        i.template cast<To>();
    };

    TEST(interval_types, cast_concepts_and_constraints) {
        using namespace calendrical;
        // Constructible / convertible types satisfy cast constraints
        static_assert(interval_castable<double, angle_t>);
        static_assert(interval_castable<int, double>);
        static_assert(interval_castable<double, int>);
        static_assert(interval_castable<int, long>);

        // Incompatible types do not satisfy cast constraints
        static_assert(!interval_castable<std::string, int>);
        static_assert(!interval_castable<int, std::string>);
        static_assert(!interval_castable<angle_t, std::string>);
    }

    TEST(interval_types, cast_execution) {
        using namespace calendrical;
        const auto int_interval = interval_closed_between(0, 180);
        const auto double_interval = int_interval.cast<double>();
        EXPECT_EQ(double_interval.type, interval_type::closed);
        EXPECT_DOUBLE_EQ(double_interval.min, 0.0);
        EXPECT_DOUBLE_EQ(double_interval.max, 180.0);

        // angle_t has explicit constructor from double - cast must work
        const auto angle_interval = double_interval.cast<angle_t>();
        EXPECT_EQ(angle_interval.type, interval_type::closed);
        EXPECT_EQ(angle_interval.min, 0_deg);
        EXPECT_EQ(angle_interval.max, 180_deg);
    }

    TEST(interval_types, conversion_operator) {
        using namespace calendrical;
        const interval<int> int_interval = interval_opened_between(10, 20);
        const interval<double> double_interval = int_interval;
        EXPECT_EQ(double_interval.type, interval_type::opened);
        EXPECT_DOUBLE_EQ(double_interval.min, 10.0);
        EXPECT_DOUBLE_EQ(double_interval.max, 20.0);

        const interval<double> deg_interval = interval_left_closed_between(0.0, 90.0);
        const interval<angle_t> angle_interval = deg_interval;
        EXPECT_EQ(angle_interval.type, interval_type::left_closed);
        EXPECT_EQ(angle_interval.min, 0_deg);
        EXPECT_EQ(angle_interval.max, 90_deg);
    }

    TEST(interval_types, containment_and_boundaries) {
        using namespace calendrical;
        const auto open = interval_opened_between(10, 20);
        EXPECT_FALSE(open.contains(10));
        EXPECT_TRUE(open.contains(15));
        EXPECT_FALSE(open.contains(20));
        EXPECT_FALSE(open(9));
        EXPECT_TRUE(open(15));

        const auto closed = interval_closed_between(10, 20);
        EXPECT_TRUE(closed.contains(10));
        EXPECT_TRUE(closed.contains(15));
        EXPECT_TRUE(closed.contains(20));
        EXPECT_FALSE(closed.contains(9));
        EXPECT_FALSE(closed.contains(21));

        const auto left_closed = interval_left_closed_between(10, 20);
        EXPECT_TRUE(left_closed.contains(10));
        EXPECT_TRUE(left_closed.contains(15));
        EXPECT_FALSE(left_closed.contains(20));

        const auto right_closed = interval_right_closed_between(10, 20);
        EXPECT_FALSE(right_closed.contains(10));
        EXPECT_TRUE(right_closed.contains(15));
        EXPECT_TRUE(right_closed.contains(20));

        EXPECT_EQ(open.close(), closed);
        EXPECT_EQ(closed.open(), open);
        EXPECT_EQ(open.close_left(), left_closed);
        EXPECT_EQ(open.close_right(), right_closed);
    }

    TEST(fixed_date_types, explicit_construction_prevents_implicit_conversion_from_numbers) {
        using namespace calendrical;
        static_assert(!std::is_convertible_v<int, fixed_date_t>);
        static_assert(!std::is_convertible_v<int64_t, fixed_date_t>);
        static_assert(!std::is_convertible_v<double, fixed_date_t>);
        static_assert(std::is_constructible_v<fixed_date_t, int>);
        static_assert(std::is_constructible_v<fixed_date_t, int64_t>);
        static_assert(!std::is_constructible_v<fixed_date_t, double>);

        fixed_date_t d1{100};
        fixed_date_t d2 = 100_rd;
        EXPECT_EQ(d1, d2);
    }

    TEST(fixed_date_types, arithmetic_operations) {
        using namespace calendrical;
        fixed_date_t d = 100_rd;
        EXPECT_EQ(d + 5, 105_rd);
        EXPECT_EQ(5 + d, 105_rd);
        EXPECT_EQ(d - 5, 95_rd);
        EXPECT_EQ(105_rd - 100_rd, 5);
        EXPECT_EQ(100_rd - 105_rd, -5);

        EXPECT_EQ(-100_rd, fixed_date_t{-100});
        EXPECT_EQ(-d, -100_rd);

        auto d_mut = d;
        d_mut += 10;
        EXPECT_EQ(d_mut, 110_rd);
        d_mut -= 20;
        EXPECT_EQ(d_mut, 90_rd);

        EXPECT_EQ(++d_mut, 91_rd);
        EXPECT_EQ(d_mut++, 91_rd);
        EXPECT_EQ(d_mut, 92_rd);
        EXPECT_EQ(--d_mut, 91_rd);
        EXPECT_EQ(d_mut--, 91_rd);
        EXPECT_EQ(d_mut, 90_rd);

        EXPECT_EQ(d.add(5), 105_rd);
        EXPECT_EQ(d.subtract(5), 95_rd);
        EXPECT_EQ(fixed_date_t::of(42), 42_rd);
    }

    TEST(fixed_date_types, comparison_and_interval) {
        using namespace calendrical;
        EXPECT_LT(10_rd, 20_rd);
        EXPECT_LE(10_rd, 10_rd);
        EXPECT_GT(20_rd, 10_rd);
        EXPECT_GE(20_rd, 20_rd);
        EXPECT_EQ(10_rd, 10_rd);
        EXPECT_NE(10_rd, 20_rd);

        static_assert(interval_castable<int, fixed_date_t>);
        static_assert(interval_castable<int64_t, fixed_date_t>);
        const auto int_interval = interval_closed_between(10, 20);
        const auto date_interval = int_interval.cast<fixed_date_t>();
        EXPECT_EQ(date_interval.min, 10_rd);
        EXPECT_EQ(date_interval.max, 20_rd);
        EXPECT_TRUE(date_interval.contains(15_rd));
        EXPECT_FALSE(date_interval.contains(25_rd));
    }

    TEST(moment_types, explicit_construction_prevents_implicit_conversion) {
        using namespace calendrical;
        static_assert(!std::is_convertible_v<int, moment_t>);
        static_assert(!std::is_convertible_v<double, moment_t>);
        static_assert(!std::is_convertible_v<fixed_date_t, moment_t>);
        static_assert(std::is_constructible_v<moment_t, int>);
        static_assert(std::is_constructible_v<moment_t, double>);
        static_assert(std::is_constructible_v<moment_t, fixed_date_t>);

        constexpr moment_t m_zero{};
        EXPECT_EQ(m_zero, 0_m);

        constexpr moment_t m1{ 42 };
        constexpr moment_t m2{ 42.5 };
        EXPECT_EQ(m1, 42_m);
        EXPECT_EQ(m2, 42.5_m);

        constexpr fixed_date_t d{ 100 };
        const moment_t md{ d };
        EXPECT_EQ(md, 100_m);
        EXPECT_EQ(static_cast<fixed_date_t>(md), d);
    }

    TEST(moment_types, arithmetic_operations) {
        using namespace calendrical;
        const moment_t m = 10.5_m;
        EXPECT_EQ(-m, moment_t{ -10.5 });
        EXPECT_EQ(m + 2.5, 13.0_m);
        EXPECT_EQ(2.5 + m, 13.0_m);
        EXPECT_EQ(m + 2_m, 12.5_m);

        EXPECT_EQ(m - 2.5, 8.0_m);
        EXPECT_EQ(12.5 - m, 2.0_m);
        EXPECT_EQ(m - 2.5_m, 8.0_m);

        EXPECT_EQ(m * 2.0, 21.0_m);
        EXPECT_EQ(2.0 * m, 21.0_m);

        EXPECT_EQ(m / 2.0, 5.25_m);
        EXPECT_DOUBLE_EQ(m / 2.0_m, 5.25);

        auto m_mut = m;
        m_mut += 1.5;
        EXPECT_EQ(m_mut, 12.0_m);
        m_mut += 2.0_m;
        EXPECT_EQ(m_mut, 14.0_m);
        m_mut -= 3.0;
        EXPECT_EQ(m_mut, 11.0_m);
        m_mut -= 1.0_m;
        EXPECT_EQ(m_mut, 10.0_m);
        m_mut *= 2.0;
        EXPECT_EQ(m_mut, 20.0_m);
        m_mut /= 4.0;
        EXPECT_EQ(m_mut, 5.0_m);

        EXPECT_EQ(m.add(1.5), 12.0_m);
        EXPECT_EQ(m.subtract(1.5), 9.0_m);
        EXPECT_DOUBLE_EQ((10.25_m).time(), 0.25);
        EXPECT_EQ(moment_t::hms(6, 0, 0), 0.25_m);
    }

    TEST(moment_types, comparison_and_interval) {
        using namespace calendrical;
        EXPECT_LT(10.0_m, 20.0_m);
        EXPECT_LE(10.0_m, 10.0_m);
        EXPECT_GT(20.0_m, 10.0_m);
        EXPECT_GE(20.0_m, 20.0_m);
        EXPECT_EQ(10.0_m, 10.0_m);
        EXPECT_NE(10.0_m, 20.0_m);

        static_assert(interval_castable<double, moment_t>);
        static_assert(interval_castable<int, moment_t>);
        const auto double_interval = interval_closed_between(10.0, 20.0);
        const auto moment_interval = double_interval.cast<moment_t>();
        EXPECT_EQ(moment_interval.min, 10.0_m);
        EXPECT_EQ(moment_interval.max, 20.0_m);
        EXPECT_TRUE(moment_interval.contains(15.5_m));
        EXPECT_FALSE(moment_interval.contains(25.0_m));

        std::ostringstream oss;
        oss << 10.5_m;
        EXPECT_FALSE(oss.str().empty());
    }

    TEST(helpers_functions, make_value_function_move_semantics) {
        using namespace calendrical;

        struct move_tracker {
            std::shared_ptr<int> copies;
            std::shared_ptr<int> moves;
            move_tracker(std::shared_ptr<int> c, std::shared_ptr<int> m) : copies(std::move(c)), moves(std::move(m)) {}
            move_tracker(const move_tracker& o) : copies(o.copies), moves(o.moves) {
                if (copies) ++(*copies);
            }
            move_tracker(move_tracker&& o) noexcept : copies(o.copies), moves(o.moves) {
                if (moves) ++(*moves);
            }
            double operator()(const double& x) const { return x * 3.0; }
        };

        auto copies = std::make_shared<int>(0);
        auto moves = std::make_shared<int>(0);
        std::function<double(const double&)> fn = move_tracker(copies, moves);

        const int copies_before = *copies;
        auto val_fn = make_value_function(std::move(fn));
        // Moving std::function into lambda must not create any new copies of the functor
        EXPECT_EQ(*copies, copies_before);
        EXPECT_DOUBLE_EQ(val_fn(4.0), 12.0);

        static const auto test_square = [](const double& x) -> double { return x * x; };
        auto fn_ref = make_value_function(*+test_square);
        EXPECT_DOUBLE_EQ(fn_ref(3.0), 9.0);
    }

    TEST(helpers_functions, make_predicate_move_semantics) {
        using namespace calendrical;

        struct move_tracker {
            std::shared_ptr<int> copies;
            std::shared_ptr<int> moves;
            move_tracker(std::shared_ptr<int> c, std::shared_ptr<int> m) : copies(std::move(c)), moves(std::move(m)) {}
            move_tracker(const move_tracker& o) : copies(o.copies), moves(o.moves) {
                if (copies) ++(*copies);
            }
            move_tracker(move_tracker&& o) noexcept : copies(o.copies), moves(o.moves) {
                if (moves) ++(*moves);
            }
            bool operator()(const int64_t& x) const { return x > 0; }
        };

        auto copies = std::make_shared<int>(0);
        auto moves = std::make_shared<int>(0);
        std::function<bool(const int64_t&)> fn = move_tracker(copies, moves);

        const int copies_before = *copies;
        auto pred = make_predicate(std::move(fn));
        EXPECT_EQ(*copies, copies_before);
        EXPECT_TRUE(pred(5));
        EXPECT_FALSE(pred(-1));
    }

}