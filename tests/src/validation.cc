#include <gtest/gtest.h>
#include <stdexcept>

#include "calendar_traits.hpp"
#include "calendars/gregorian_calendar.hpp"
#include "calendars/julian_calendar.hpp"
#include "calendars/hebrew_calendar.hpp"
#include "calendars/coptic_based_calendar.hpp"
#include "calendars/iso_calendar.hpp"
#include "calendars/icelandic_calendar.hpp"

namespace tests {
    using namespace calendrical;
    using namespace calendrical::calendars;
    using namespace calendrical::calendar_traits;

    TEST(calendar_validation, gregorian_is_valid_date) {
        const auto& cal = gregorian_calendar::standard;

        // Valid dates
        EXPECT_TRUE(cal.is_valid_date({2024, month::february, 29})); // Leap year
        EXPECT_TRUE(cal.is_valid_date({2023, month::january, 31}));
        EXPECT_TRUE(cal.is_valid_date({2023, month::april, 30}));
        EXPECT_TRUE(cal.is_valid_date({2000, month::february, 29})); // 400-year leap

        // Invalid dates
        EXPECT_FALSE(cal.is_valid_date({2023, month::february, 29})); // Non-leap year
        EXPECT_FALSE(cal.is_valid_date({1900, month::february, 29})); // Century non-leap
        EXPECT_FALSE(cal.is_valid_date({2026, month::february, 31}));
        EXPECT_FALSE(cal.is_valid_date({2026, month::april, 31}));    // April has 30 days
        EXPECT_FALSE(cal.is_valid_date({2026, month::january, 0}));   // Day 0
        EXPECT_FALSE(cal.is_valid_date({2026, month::january, -1}));  // Negative day
        EXPECT_FALSE(cal.is_valid_date({2026, month::january, 32}));  // Day 32
        EXPECT_FALSE(cal.is_valid_date({2026, static_cast<month>(0), 1}));  // Month 0
        EXPECT_FALSE(cal.is_valid_date({2026, static_cast<month>(13), 1})); // Month 13
    }

    TEST(calendar_validation, julian_is_valid_date) {
        const julian_calendar cal{};

        // Valid dates
        EXPECT_TRUE(cal.is_valid_date({2024, month::february, 29}));
        EXPECT_TRUE(cal.is_valid_date({1900, month::february, 29})); // Julian: 1900 is leap!
        EXPECT_TRUE(cal.is_valid_date({1, month::january, 1}));
        EXPECT_TRUE(cal.is_valid_date({-1, month::january, 1}));

        // Invalid dates
        EXPECT_FALSE(cal.is_valid_date({0, month::january, 1}));      // Year 0 does not exist
        EXPECT_FALSE(cal.is_valid_date({2023, month::february, 29}));
        EXPECT_FALSE(cal.is_valid_date({2026, month::february, 31}));
        EXPECT_FALSE(cal.is_valid_date({2026, month::april, 31}));
        EXPECT_FALSE(cal.is_valid_date({2026, month::january, 0}));
    }

    TEST(calendar_validation, hebrew_is_valid_date) {
        const hebrew_calendar cal{};

        // Valid dates
        EXPECT_TRUE(cal.is_valid_date({5784, hebrew_month::tishri, 1}));
        EXPECT_TRUE(cal.is_valid_date({5784, hebrew_month::nisan, 15}));
        EXPECT_TRUE(cal.is_valid_date({5784, hebrew_month::adarii, 1})); // 5784 is leap year

        // Invalid dates
        EXPECT_FALSE(cal.is_valid_date({0, hebrew_month::tishri, 1}));   // Year < 1
        EXPECT_FALSE(cal.is_valid_date({5783, hebrew_month::adarii, 1})); // 5783 is NOT leap year
        EXPECT_FALSE(cal.is_valid_date({5784, hebrew_month::tishri, 0}));
        EXPECT_FALSE(cal.is_valid_date({5784, hebrew_month::tishri, 31}));
    }

    TEST(calendar_validation, coptic_is_valid_date) {
        // Coptic year 1739 is leap (1739 % 4 == 3)
        EXPECT_TRUE(coptic_calendar.is_valid_date({1739, 13, 6}));
        EXPECT_FALSE(coptic_calendar.is_valid_date({1739, 13, 7}));

        // Coptic year 1740 is non-leap (1740 % 4 == 0 != 3)
        EXPECT_TRUE(coptic_calendar.is_valid_date({1740, 13, 5}));
        EXPECT_FALSE(coptic_calendar.is_valid_date({1740, 13, 6}));

        // Out-of-bounds months and days
        EXPECT_FALSE(coptic_calendar.is_valid_date({1740, 0, 1}));
        EXPECT_FALSE(coptic_calendar.is_valid_date({1740, 14, 1}));
        EXPECT_FALSE(coptic_calendar.is_valid_date({1740, 1, 0}));
        EXPECT_FALSE(coptic_calendar.is_valid_date({1740, 1, 31})); // regular months have 30 days
    }

    TEST(calendar_validation, iso_is_valid_date) {
        const iso_calendar cal{};

        EXPECT_TRUE(cal.is_valid_date({2024, 1, 1}));
        EXPECT_TRUE(cal.is_valid_date({2024, 1, 7}));
        EXPECT_FALSE(cal.is_valid_date({2024, 1, 0}));
        EXPECT_FALSE(cal.is_valid_date({2024, 1, 8}));
    }

    TEST(parser_validation, parsers_do_not_validate_calendar_semantics) {
        // Calendar-specific validation lives in the converters (is_valid_date), not in the parsers.
        EXPECT_NO_THROW((datetype_from_compact_string<gregorian_date>("2026-02-31")));
        EXPECT_NO_THROW((datetype_from_compact_string<julian_date>("0-01-01")));
        EXPECT_NO_THROW((datetype_from_compact_string<iso_date>("2024-10-8")));
    }

    TEST(parser_validation, compact_format_accepts_negative_values) {
        const auto g = datetype_from_compact_string<gregorian_date>("-500-03-15");
        EXPECT_EQ(g.year, -500);
        EXPECT_EQ(g.month, month::march);
        EXPECT_EQ(g.day, 15);

        const auto j = datetype_from_compact_string<julian_date>(" -44 - 3 - 15 ");
        EXPECT_EQ(j.year, -44);
        EXPECT_EQ(j.month, month::march);
        EXPECT_EQ(j.day, 15);

        // Negative values in non-leading fields are signs too.
        const auto s = datetype_from_compact_string<simple_date>("1-2--3");
        EXPECT_EQ(s.year, 1);
        EXPECT_EQ(s.month, 2);
        EXPECT_EQ(s.day, -3);

        // A '-' not followed by a digit is still a separator, so empty fields keep failing.
        EXPECT_THROW((datetype_from_compact_string<gregorian_date>("2024--")), std::runtime_error);
        EXPECT_THROW((datetype_from_compact_string<gregorian_date>("-2024-03")), std::runtime_error);
    }

    TEST(parser_validation, sexagesimal_name_parser) {
        EXPECT_NO_THROW((datetype_from_compact_string<datetypes::sexagesimal_name>("1-1")));
        EXPECT_NO_THROW((datetype_from_compact_string<datetypes::sexagesimal_name>("2-2")));
        EXPECT_THROW((datetype_from_compact_string<datetypes::sexagesimal_name>("1-2")), std::invalid_argument);
        EXPECT_THROW((datetype_from_compact_string<datetypes::sexagesimal_name>("0-2")), std::invalid_argument);
        EXPECT_THROW((datetype_from_compact_string<datetypes::sexagesimal_name>("11-1")), std::invalid_argument);
    }

    TEST(parser_validation, olympiad_parser) {
        EXPECT_NO_THROW((datetype_from_compact_string<datetypes::olympiad>("1-1")));
        EXPECT_NO_THROW((datetype_from_compact_string<datetypes::olympiad>("700-4")));
        EXPECT_THROW((datetype_from_compact_string<datetypes::olympiad>("0-1")), std::invalid_argument);
        EXPECT_THROW((datetype_from_compact_string<datetypes::olympiad>("1-0")), std::invalid_argument);
        EXPECT_THROW((datetype_from_compact_string<datetypes::olympiad>("1-5")), std::invalid_argument);
    }

    TEST(helpers_validation, floor_div_division_by_zero) {
        EXPECT_THROW((floor_div(int64_t{10}, int64_t{0})), std::invalid_argument);
        EXPECT_THROW((floor_div(10, 0)), std::invalid_argument);
        EXPECT_THROW((floor_div(10.0, 0.0)), std::invalid_argument);

        // Valid divisions
        EXPECT_EQ(floor_div(int64_t{10}, int64_t{2}), 5);
        EXPECT_EQ(floor_div(int64_t{-10}, int64_t{3}), -4);
        EXPECT_DOUBLE_EQ(floor_div(10.0, 2.0), 5.0);
    }

    TEST(helpers_validation, least_passing_guard_exceeded) {
        // Predicate that is never satisfied
        auto never_true = [](const int64_t&) { return false; };
        EXPECT_THROW(least_passing(never_true, 0), std::runtime_error);

        // Predicate satisfied within guard
        auto satisfied = [](const int64_t& x) { return x >= 10; };
        EXPECT_EQ(least_passing(satisfied, 0), 10);
    }

    TEST(helpers_validation, last_passing_guard_exceeded) {
        // Predicate that is always true (never transitions to false within guard)
        auto always_true = [](const int64_t&) { return true; };
        EXPECT_THROW(last_passing(always_true, 0), std::runtime_error);

        // Normal boundary
        auto boundary = [](const int64_t& x) { return x < 10; };
        EXPECT_EQ(last_passing(boundary, 0), 9);
    }

    TEST(helpers_validation, binary_search_non_converging) {
        // psi never returns true (never terminates naturally)
        auto psi_never = [](const double&, const double&) { return false; };
        auto phi = [](const double& x) { return x >= 5.0; };
        EXPECT_THROW(binary_search(phi, psi_never, 0.0, 10.0), std::runtime_error);

        // Normal convergence
        auto psi_normal = [](const double& l, const double& u) { return u - l < 1e-4; };
        EXPECT_NEAR(binary_search(phi, psi_normal, 0.0, 10.0), 5.0, 1e-3);
    }
}
