#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/akan_calendar.hpp"
#include "local_helpers.hpp"

#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

namespace tests {
    using namespace calendrical::calendars;

    namespace {
        const akan_calendar calendar{};

        /** All 42 valid names, in (prefix, stem) order. */
        std::vector<akan_name> all_names() {
            std::vector<akan_name> names;
            for (int32_t prefix = 1; prefix <= akan_name::prefix_count; ++prefix)
                for (int32_t stem = 1; stem <= akan_name::stem_count; ++stem)
                    names.emplace_back(prefix, stem);
            return names;
        }

        /** Reference dates around the epoch, RD 0, cycle boundaries and far from zero in both directions. */
        std::vector<fixed_date_t> reference_dates() {
            std::vector<fixed_date_t> refs{ dates.begin(), dates.end() };
            for (const int64_t rd : { -1000000LL, -214193LL, -43LL, -42LL, -41LL, -1LL, 0LL, 1LL, 36LL, 37LL, 38LL, 78LL, 79LL, 1000000LL })
                refs.emplace_back(rd);
            return refs;
        }
    }

    TEST(akan_calendar, epoch_and_cycle_length) {
        EXPECT_EQ(calendar.epoch(), 37_rd);
        EXPECT_EQ(calendar.cycle_length(), 42);
        EXPECT_EQ(calendar.to_date(37_rd), (akan_name{ 6, 7 }));
        EXPECT_EQ(calendar.to_date(38_rd), (akan_name{ 1, 1 }));
        EXPECT_EQ(calendar.to_date(36_rd), (akan_name{ 5, 6 }));
        EXPECT_EQ(calendar.to_date(0_rd), (akan_name{ 5, 5 }));
        EXPECT_EQ(calendar.to_date(1_rd), (akan_name{ 6, 6 }));
    }

    TEST(akan_calendar, stem_follows_weekday) {
        // CC: stems Wukuo (1) .. Bene (7) fall on Wednesday .. Tuesday, which fixes the epoch at RD 37 modulo 7.
        for (fixed_date_t rd{ -500 }; rd <= fixed_date_t{ 500 }; ++rd) {
            const auto expected_stem = static_cast<int32_t>(calendrical::mod(rd.get_weekday() - calendrical::weekday::wednesday, 7) + 1);
            EXPECT_EQ(calendar.to_date(rd).stem, expected_stem) << rd;
        }
        for (const auto& rd : dates) {
            const auto expected_stem = static_cast<int32_t>(calendrical::mod(rd.get_weekday() - calendrical::weekday::wednesday, 7) + 1);
            EXPECT_EQ(calendar.to_date(rd).stem, expected_stem) << rd;
        }
    }

    TEST(akan_calendar, cycle_covers_all_42_names) {
        std::set<std::pair<int32_t, int32_t>> seen;
        for (auto rd = 38_rd; rd < 38_rd + 42; ++rd) {
            const auto name = calendar.to_date(rd);
            EXPECT_TRUE(calendar.is_valid_date(name)) << name;
            seen.emplace(name.prefix, name.stem);
        }
        EXPECT_EQ(seen.size(), 42u);
    }

    TEST(akan_calendar, names_repeat_every_42_days) {
        for (fixed_date_t rd{ -200 }; rd <= fixed_date_t{ 200 }; ++rd) {
            const auto name = calendar.to_date(rd);
            EXPECT_EQ(calendar.to_date(rd + 42), name) << rd;
            EXPECT_EQ(calendar.to_date(rd - 42), name) << rd;
            for (int k = 1; k < 42; ++k)
                EXPECT_NE(calendar.to_date(rd + k), name) << rd << " + " << k;
        }
    }

    TEST(akan_name, difference_counts_days_forward) {
        for (int64_t n = -42; n < 42; ++n) {
            for (int64_t m = n; m < n + 42; ++m) {
                const auto expected = static_cast<int32_t>(calendrical::adjusted_remainder(m - n, 42));
                EXPECT_EQ(akan_name::from(n).difference(akan_name::from(m)), expected) << n << " -> " << m;
            }
        }
    }

    TEST(akan_calendar, on_or_before_finds_last_occurrence) {
        for (const auto& reference : reference_dates()) {
            for (const auto& name : all_names()) {
                const auto r = calendar.on_or_before(name, reference);
                EXPECT_LE(r, reference) << name << " ref " << reference;
                EXPECT_LT(reference - r, 42) << name << " ref " << reference;
                EXPECT_EQ(calendar.to_date(r), name) << name << " ref " << reference;
            }
        }
    }

    TEST(akan_calendar, on_or_before_round_trip) {
        EXPECT_EQ(calendar.on_or_before(calendar.to_date(37_rd), 37_rd), 37_rd);
        for (fixed_date_t rd{ -1000 }; rd <= fixed_date_t{ 1000 }; ++rd)
            EXPECT_EQ(calendar.on_or_before(calendar.to_date(rd), rd), rd);
        for (const auto& rd : reference_dates())
            EXPECT_EQ(calendar.on_or_before(calendar.to_date(rd), rd), rd);
    }

    TEST(akan_calendar, works_through_cyclic_interface) {
        const calendrical::typed_cyclic_calendar_if<akan_name>& cyclic = calendar;
        EXPECT_EQ(cyclic.epoch(), 37_rd);
        EXPECT_EQ(cyclic.on_or_before(akan_name{ 1, 1 }, 100_rd), 80_rd);
    }

    TEST(akan_calendar, validation_rejects_names_outside_domain) {
        for (const auto& name : all_names())
            EXPECT_TRUE(calendar.is_valid_date(name)) << name;
        for (const auto& name : { akan_name{ 0, 1 }, akan_name{ 7, 1 }, akan_name{ 1, 0 }, akan_name{ 1, 8 },
                                  akan_name{ -1, -1 }, akan_name{ 6, 14 }, akan_name{ 12, 7 } }) {
            EXPECT_FALSE(calendar.is_valid_date(name)) << name;
            EXPECT_THROW(static_cast<void>(calendar.on_or_before(name, 37_rd)), std::invalid_argument) << name;
        }
    }
}
