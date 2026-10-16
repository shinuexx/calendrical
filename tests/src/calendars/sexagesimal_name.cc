#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <vector>

#include "calendars/chinese_based_calendar.hpp"
#include "datetypes/sexagesimal_name.hpp"
#include "sample_fixed_dates.hpp"

namespace tests {
    using namespace calendrical;
    using calendars::chinese_based_calendar;
    using datetypes::sexagesimal_name;

    namespace {
        // Enumerate the valid stem/branch pairs in forward day order, independently
        // of difference(): Jia-Zi (1, 1) through Gui-Hai (10, 12).
        std::array<sexagesimal_name, 60> all_names() {
            std::array<sexagesimal_name, 60> names{};
            for (int32_t i = 0; i < 60; ++i)
                names[i] = sexagesimal_name{ i % 10 + 1, i % 12 + 1 };
            return names;
        }

        std::vector<fixed_date_t> reference_dates() {
            std::vector<fixed_date_t> refs{ dates.begin(), dates.end() };
            for (int64_t rd = -100; rd < 100; ++rd)
                refs.emplace_back(rd);
            // Far beyond double's contiguous integer range, with room for cycle
            // arithmetic in int64_t in both directions.
            constexpr int64_t large = int64_t{ 1 } << 60;
            for (const int64_t rd : { -large - 61, -large, -large + 61,
                                      large - 61, large, large + 61 })
                refs.emplace_back(rd);
            return refs;
        }
    }

    TEST(sexagesimal_name, difference_counts_days_forward_for_all_pairs) {
        const auto names = all_names();
        for (int32_t from = 0; from < 60; ++from) {
            for (int32_t to = 0; to < 60; ++to) {
                const auto offset = (to - from + 60) % 60;
                const auto expected = offset == 0 ? 60 : offset;
                const auto actual = names[from].difference(names[to]);
                EXPECT_EQ(actual, expected) << "from " << names[from] << " to " << names[to];
                EXPECT_GE(actual, 1);
                EXPECT_LE(actual, 60);
                EXPECT_EQ(sexagesimal_name::from(from + 1 + actual), names[to]);
            }
        }
    }

    TEST(sexagesimal_name, day_name_on_or_before_finds_last_occurrence) {
        for (const auto& reference : reference_dates()) {
            for (const auto& name : all_names()) {
                const auto result = chinese_based_calendar::day_name_on_or_before(name, reference);
                EXPECT_LE(result, reference) << name << " ref " << reference;
                EXPECT_GE(result, reference - 59) << name << " ref " << reference;
                EXPECT_EQ(chinese_based_calendar::day_name(result), name) << name << " ref " << reference;
            }
        }
    }

    TEST(sexagesimal_name, day_name_on_or_before_round_trip) {
        for (fixed_date_t rd{ -1000 }; rd <= 1000_rd; ++rd)
            EXPECT_EQ(chinese_based_calendar::day_name_on_or_before(chinese_based_calendar::day_name(rd), rd), rd) << rd;
        for (const auto& rd : reference_dates())
            EXPECT_EQ(chinese_based_calendar::day_name_on_or_before(chinese_based_calendar::day_name(rd), rd), rd) << rd;
    }
}
