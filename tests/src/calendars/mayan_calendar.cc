#include <gtest/gtest.h>

#include <array>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>

#include "calendars/gregorian_calendar.hpp"
#include "calendars/mayan_calendar.hpp"
#include "primitives.hpp"

namespace tests {
    using namespace calendrical;
    using calendars::mayan_calendar;
    using datetypes::mayan_long_count;

    TEST(mayan_calendar, gmt_correlation_and_smithsonian_reference) {
        const mayan_calendar cal;
        EXPECT_EQ(cal.epoch(), -1137142_rd);
        EXPECT_EQ(cal.epoch(), primitives::fixed_from_jd(584283));
        EXPECT_EQ(cal.to_fixed({}), cal.epoch());
        EXPECT_EQ(cal.to_date(cal.epoch()), mayan_long_count{});
        // Smithsonian NMAI, Living Maya Time, "The Calendar System":
        // https://maya.nmai.si.edu/calendar/calendar-system
        // 13.0.0.0.0 corresponds to Gregorian 2012-12-21 (1,872,000 days).
        const auto reference = calendars::gregorian_calendar::standard.to_fixed({2012, month::december, 21});
        const mayan_long_count thirteen{0, 0, 0, 0, 13, 0, 0, 0, 0};
        EXPECT_EQ(reference, 734858_rd);
        EXPECT_EQ(cal.to_fixed(thirteen), reference);
        EXPECT_EQ(cal.to_date(reference), thirteen);
        EXPECT_EQ(reference - cal.epoch(), 1872000);
        EXPECT_EQ(cal.to_date(reference.subtract(1)), (mayan_long_count{0, 0, 0, 0, 12, 19, 19, 17, 19}));
        // The same Smithsonian page gives 2000-01-01 = 12.19.6.15.2.
        const auto millennium = calendars::gregorian_calendar::standard.to_fixed({2000, month::january, 1});
        const mayan_long_count twelve{0, 0, 0, 0, 12, 19, 6, 15, 2};
        EXPECT_EQ(cal.to_fixed(twelve), millennium);
        EXPECT_EQ(cal.to_date(millennium), twelve);
        // Epoch's Gregorian label follows from the same GMT correlation.
        EXPECT_EQ(cal.epoch(), calendars::gregorian_calendar::standard.to_fixed({-3113, month::august, 11}));
    }

    TEST(mayan_calendar, carries_at_every_field_boundary) {
        const mayan_calendar cal;
        constexpr std::array<int64_t, 9> weights{
            23040000000LL, 1152000000LL, 57600000LL, 2880000LL,
            144000LL, 7200LL, 360LL, 20LL, 1LL};
        constexpr std::array<int32_t mayan_long_count::*, 9> members{
            &mayan_long_count::alautun, &mayan_long_count::kinchiltun,
            &mayan_long_count::cakabtun, &mayan_long_count::pictun,
            &mayan_long_count::baktun, &mayan_long_count::katun,
            &mayan_long_count::tun, &mayan_long_count::uinal, &mayan_long_count::kin};
        for (std::size_t i = 0; i < weights.size(); ++i) {
            mayan_long_count one{};
            one.*members[i] = 1;
            EXPECT_EQ(cal.to_fixed(one), cal.epoch().add(weights[i])) << i;
            EXPECT_EQ(cal.to_date(cal.epoch().add(weights[i])), one) << i;
            mayan_long_count before{};
            for (std::size_t j = i + 1; j < members.size(); ++j) {
                before.*members[j] = j == 7 ? 17 : 19;
            }
            EXPECT_EQ(cal.to_date(cal.epoch().add(weights[i] - 1)), before) << i;
        }
        // Baktuns have radix 20, not 13, in the proleptic representation.
        EXPECT_EQ(cal.to_date(cal.epoch().add(20 * 144000)), (mayan_long_count{0, 0, 0, 1, 0, 0, 0, 0, 0}));
    }

    TEST(mayan_calendar, negative_counts_use_floor_and_nonnegative_lower_fields) {
        const mayan_calendar cal;
        const mayan_long_count previous{-1, 19, 19, 19, 19, 19, 19, 17, 19};
        EXPECT_EQ(cal.to_date(cal.epoch().subtract(1)), previous);
        EXPECT_EQ(cal.to_fixed(previous), cal.epoch().subtract(1));
        EXPECT_EQ(cal.to_date(cal.epoch().subtract(mayan_long_count::days_per_alautun)),
                  (mayan_long_count{-1, 0, 0, 0, 0, 0, 0, 0, 0}));
        EXPECT_EQ(cal.to_date(cal.epoch().subtract(mayan_long_count::days_per_alautun + 1)),
                  (mayan_long_count{-2, 19, 19, 19, 19, 19, 19, 17, 19}));
    }

    TEST(mayan_calendar, rejects_noncanonical_fields_without_normalization) {
        const mayan_calendar cal;
        constexpr std::array<int32_t mayan_long_count::*, 8> lower{
            &mayan_long_count::kinchiltun, &mayan_long_count::cakabtun,
            &mayan_long_count::pictun, &mayan_long_count::baktun,
            &mayan_long_count::katun, &mayan_long_count::tun,
            &mayan_long_count::uinal, &mayan_long_count::kin};
        for (const auto member : lower) {
            for (const int32_t invalid : {-1, 20, std::numeric_limits<int32_t>::max()}) {
                mayan_long_count date{};
                date.*member = invalid;
                EXPECT_FALSE(cal.is_valid_date(date));
                EXPECT_THROW((void)cal.to_fixed(date), std::invalid_argument);
            }
        }
        EXPECT_FALSE(cal.is_valid_date({0, 0, 0, 0, 0, 0, 0, 18, 0}));
        EXPECT_FALSE(cal.is_valid_date({0, 0, 0, 0, 0, 0, 0, 19, 0}));
        for (const int32_t invalid : {mayan_long_count::max_alautun + 1, -mayan_long_count::max_alautun - 1,
                                     std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()}) {
            const mayan_long_count date{invalid, 0, 0, 0, 0, 0, 0, 0, 0};
            EXPECT_FALSE(cal.is_valid_date(date));
            EXPECT_THROW((void)cal.to_fixed(date), std::invalid_argument);
        }
    }

    TEST(mayan_calendar, domain_endpoints_and_outside_values) {
        const mayan_calendar cal;
        const mayan_long_count first{-mayan_long_count::max_alautun, 0, 0, 0, 0, 0, 0, 0, 0};
        const mayan_long_count last{mayan_long_count::max_alautun, 19, 19, 19, 19, 19, 19, 17, 19};
        EXPECT_EQ(cal.to_fixed(first), mayan_calendar::min_date);
        EXPECT_EQ(cal.to_fixed(last), mayan_calendar::max_date);
        EXPECT_EQ(cal.to_date(mayan_calendar::min_date), first);
        EXPECT_EQ(cal.to_date(mayan_calendar::max_date), last);
        for (const auto rd : {mayan_calendar::min_date.subtract(1), mayan_calendar::max_date.add(1),
                             fixed_date_t{std::numeric_limits<int64_t>::min()},
                             fixed_date_t{std::numeric_limits<int64_t>::max()}}) {
            EXPECT_THROW((void)cal.to_date(rd), std::out_of_range);
        }
    }

    TEST(mayan_calendar, integer_round_trips_across_the_supported_domain) {
        const mayan_calendar cal;
        std::mt19937_64 engine{584283};
        std::uniform_int_distribution<int64_t> distribution{
            static_cast<int64_t>(mayan_calendar::min_date), static_cast<int64_t>(mayan_calendar::max_date)};
        for (int i = 0; i < 10000; ++i) {
            const fixed_date_t rd{distribution(engine)};
            const auto date = cal.to_date(rd);
            ASSERT_TRUE(cal.is_valid_date(date)) << rd;
            ASSERT_EQ(cal.to_fixed(date), rd);
        }
    }

    TEST(mayan_calendar, structured_format_stream_and_default_value) {
        const mayan_long_count date{};
        EXPECT_TRUE(date.is_valid());
        EXPECT_EQ(date.to_string(), "mayan_long_count{alautun:0,kinchiltun:0,cakabtun:0,pictun:0,baktun:0,katun:0,tun:0,uinal:0,kin:0}");
        std::ostringstream out;
        out << mayan_long_count{-1, 19, 19, 19, 19, 19, 19, 17, 19};
        EXPECT_EQ(out.str(), "mayan_long_count{alautun:-1,kinchiltun:19,cakabtun:19,pictun:19,baktun:19,katun:19,tun:19,uinal:17,kin:19}");
    }
}
