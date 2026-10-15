#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/gregorian_calendar.hpp"
#include "calendars/pawukon_calendar.hpp"
#include "primitives.hpp"

#include <limits>
#include <set>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace tests {
    using namespace calendrical::calendars;

    namespace {
        const pawukon_calendar calendar{};

        static_assert(pawukon_date{} == pawukon_date::from_day(0));
        static_assert(pawukon_date::from_day(0) == pawukon_date{ false, 1, 1, 1, 2, 1, 1, 1, 1, 5 });
        static_assert(pawukon_date::from_day(210) == pawukon_date::from_day(0));
        static_assert(pawukon_date::from_day(-1).day() == 209);
        static_assert(pawukon_date::from_day(73).is_valid());

        fixed_date_t gregorian(int32_t year, calendrical::month month, int32_t day) {
            return gregorian_calendar::standard.to_fixed({ year, month, day });
        }

        /**
         * Pawukon dates of the 33 sample dates of CC appendix C, computed with
         * `bali-pawukon-from-fixed` from the CC reference code (calendar.l, CALENDRICA 4.0).
         */
        const std::vector<std::pair<fixed_date_t, pawukon_date>> sample_pawukon_dates{
            { -214193_rd, { false, 1, 1, 1, 3, 1, 1, 5, 7, 3 } },
            { -61387_rd, { true, 2, 2, 1, 4, 5, 4, 5, 5, 2 } },
            { 25469_rd, { true, 2, 2, 1, 5, 5, 4, 1, 5, 6 } },
            { 49217_rd, { false, 1, 2, 3, 3, 5, 1, 3, 5, 3 } },
            { 171307_rd, { false, 1, 1, 3, 3, 1, 4, 3, 1, 5 } },
            { 210155_rd, { true, 2, 2, 1, 1, 5, 2, 1, 8, 0 } },
            { 253427_rd, { false, 1, 2, 3, 3, 5, 7, 3, 2, 7 } },
            { 369740_rd, { false, 1, 2, 2, 1, 2, 1, 2, 2, 1 } },
            { 400085_rd, { false, 1, 2, 1, 1, 5, 1, 1, 8, 1 } },
            { 434355_rd, { true, 2, 3, 1, 1, 3, 6, 1, 3, 2 } },
            { 452605_rd, { false, 1, 1, 1, 1, 1, 7, 5, 1, 5 } },
            { 470160_rd, { true, 2, 3, 4, 1, 6, 6, 8, 6, 2 } },
            { 473837_rd, { false, 1, 2, 3, 3, 5, 1, 3, 5, 3 } },
            { 507850_rd, { false, 1, 1, 4, 1, 4, 1, 4, 7, 1 } },
            { 524156_rd, { false, 1, 2, 2, 2, 2, 4, 2, 5, 7 } },
            { 544676_rd, { false, 1, 2, 4, 2, 2, 7, 8, 8, 9 } },
            { 567118_rd, { true, 2, 1, 4, 4, 4, 7, 4, 7, 4 } },
            { 569477_rd, { false, 1, 2, 3, 3, 5, 7, 3, 2, 7 } },
            { 601716_rd, { false, 1, 3, 4, 2, 6, 4, 8, 3, 7 } },
            { 613424_rd, { true, 2, 2, 4, 5, 2, 1, 4, 5, 4 } },
            { 626596_rd, { true, 2, 1, 2, 2, 4, 6, 2, 1, 6 } },
            { 645554_rd, { true, 2, 2, 4, 5, 2, 1, 4, 5, 4 } },
            { 664224_rd, { false, 1, 3, 4, 5, 6, 2, 8, 3, 3 } },
            { 671401_rd, { false, 1, 1, 1, 2, 1, 4, 5, 4, 7 } },
            { 694799_rd, { true, 2, 2, 1, 5, 5, 1, 5, 8, 4 } },
            { 704424_rd, { true, 2, 3, 2, 5, 6, 1, 2, 3, 4 } },
            { 708842_rd, { true, 2, 2, 2, 3, 2, 2, 2, 1, 2 } },
            { 709409_rd, { false, 1, 2, 3, 5, 5, 2, 3, 2, 3 } },
            { 709580_rd, { true, 2, 2, 4, 1, 2, 5, 4, 8, 4 } },
            { 727274_rd, { true, 2, 2, 2, 5, 2, 3, 2, 8, 2 } },
            { 728714_rd, { true, 2, 2, 4, 5, 2, 1, 4, 5, 4 } },
            { 744313_rd, { true, 2, 1, 3, 4, 1, 4, 7, 1, 2 } },
            { 764652_rd, { false, 1, 3, 4, 3, 6, 1, 8, 6, 3 } },
        };

        /** Reference dates around the epoch, RD 0, cycle boundaries and far from zero in both directions. */
        std::vector<fixed_date_t> reference_dates() {
            std::vector<fixed_date_t> refs{ dates.begin(), dates.end() };
            for (const int64_t rd : { -2000000LL, -1721489LL, -1721280LL, -1721279LL, -1721278LL, -1721070LL, -1721069LL,
                                      -214193LL, -211LL, -210LL, -209LL, -1LL, 0LL, 1LL, 209LL, 210LL, 1000000LL })
                refs.emplace_back(rd);
            return refs;
        }
    }

    TEST(pawukon_calendar, epoch_and_cycle_length) {
        EXPECT_EQ(calendar.epoch(), fixed_date_t{ -1721279 });
        EXPECT_EQ(calendar.epoch(), calendrical::primitives::fixed_from_jd(146));
        EXPECT_EQ(calendar.cycle_length(), 210);
        EXPECT_EQ(calendar.day_from_fixed(calendar.epoch()), 0);
        EXPECT_EQ(calendar.to_date(calendar.epoch()), pawukon_date::from_day(0));
        EXPECT_EQ(calendar.day_from_fixed(calendar.epoch() - 1), 209);
        EXPECT_EQ(calendar.day_from_fixed(0_rd), 119);
    }

    TEST(pawukon_calendar, sample_dates_match_reference_code) {
        ASSERT_EQ(sample_pawukon_dates.size(), dates.size());
        for (size_t i = 0; i < dates.size(); ++i) {
            const auto& [rd, expected] = sample_pawukon_dates[i];
            ASSERT_EQ(rd, dates[i]);
            EXPECT_EQ(calendar.to_date(rd), expected) << rd;
            EXPECT_TRUE(expected.is_valid()) << expected;
            EXPECT_EQ(calendar.day_from_fixed(rd), expected.day()) << rd;
        }
    }

    TEST(pawukon_calendar, galungan_and_kuningan) {
        // Galungan is Buda (Wednesday) Keliwon of Dungulan, the 11th week; Kuningan,
        // ten days later, is Saniscara (Saturday) Keliwon of the 12th week.
        for (const auto& galungan : { gregorian(2024, calendrical::month::february, 28), gregorian(2024, calendrical::month::september, 25) }) {
            const auto date = calendar.to_date(galungan);
            EXPECT_EQ(galungan.get_weekday(), calendrical::weekday::wednesday);
            EXPECT_EQ(date.day(), 73);
            EXPECT_EQ(date.week(), 11);
            EXPECT_EQ(date.saptawara, 4);
            EXPECT_EQ(date.pancawara, 5);
            const auto kuningan = calendar.to_date(galungan + 10);
            EXPECT_EQ(kuningan.week(), 12);
            EXPECT_EQ(kuningan.saptawara, 7);
            EXPECT_EQ(kuningan.pancawara, 5);
        }
    }

    TEST(pawukon_calendar, saptawara_follows_weekday) {
        for (fixed_date_t rd{ -500 }; rd <= fixed_date_t{ 500 }; ++rd) {
            const auto expected = static_cast<int32_t>(calendrical::mod(rd.get_weekday() - calendrical::weekday::sunday, 7) + 1);
            EXPECT_EQ(calendar.to_date(rd).saptawara, expected) << rd;
        }
    }

    TEST(pawukon_calendar, cycle_covers_210_distinct_dates) {
        std::set<pawukon_date> seen;
        for (int32_t d = 0; d < 210; ++d) {
            const auto date = calendar.to_date(calendar.epoch() + d);
            EXPECT_TRUE(calendar.is_valid_date(date)) << date;
            EXPECT_EQ(date.day(), d) << date;
            EXPECT_EQ(date.week(), d / 7 + 1) << date;
            EXPECT_EQ(date, pawukon_date::from_day(d));
            EXPECT_EQ(date.luang, date.dasawara % 2 == 0) << date;
            seen.insert(date);
        }
        EXPECT_EQ(seen.size(), 210u);
    }

    TEST(pawukon_calendar, dates_repeat_every_210_days) {
        for (fixed_date_t rd{ -1000 }; rd <= fixed_date_t{ 1000 }; ++rd) {
            const auto date = calendar.to_date(rd);
            EXPECT_EQ(calendar.to_date(rd + 210), date) << rd;
            EXPECT_EQ(calendar.to_date(rd - 210), date) << rd;
            EXPECT_EQ(calendar.to_date(rd - 210 * 10000), date) << rd;
        }
        for (fixed_date_t rd{ 0 }; rd < fixed_date_t{ 210 }; ++rd) {
            const auto date = calendar.to_date(rd);
            for (int k = 1; k < 210; ++k)
                EXPECT_NE(calendar.to_date(rd + k), date) << rd << " + " << k;
        }
    }

    TEST(pawukon_calendar, subcycle_periods) {
        // Kajeng Keliwon (triwara 3, pancawara 5) recurs every 15 days and Tumpek
        // (pancawara 5, saptawara 7) every 35 days; neither recurs sooner.
        const auto kajeng_keliwon = [](const pawukon_date& d) { return d.triwara == 3 && d.pancawara == 5; };
        const auto tumpek = [](const pawukon_date& d) { return d.pancawara == 5 && d.saptawara == 7; };
        std::vector<int32_t> kk_days, tumpek_days;
        for (int32_t d = 0; d < 210; ++d) {
            if (kajeng_keliwon(pawukon_date::from_day(d))) kk_days.push_back(d);
            if (tumpek(pawukon_date::from_day(d))) tumpek_days.push_back(d);
        }
        ASSERT_EQ(kk_days.size(), 14u);
        ASSERT_EQ(tumpek_days.size(), 6u);
        for (size_t i = 0; i < kk_days.size(); ++i)
            EXPECT_EQ(kk_days[i], 8 + 15 * static_cast<int32_t>(i));
        for (size_t i = 0; i < tumpek_days.size(); ++i)
            EXPECT_EQ(tumpek_days[i], 13 + 35 * static_cast<int32_t>(i));
    }

    TEST(pawukon_calendar, negative_fixed_dates) {
        for (fixed_date_t rd{ -1721279 - 420 }; rd <= fixed_date_t{ -1721279 + 420 }; ++rd) {
            const auto day = calendar.day_from_fixed(rd);
            EXPECT_GE(day, 0) << rd;
            EXPECT_LT(day, 210) << rd;
            EXPECT_EQ(calendar.to_date(rd).day(), day) << rd;
            EXPECT_EQ(calendar.on_or_before(calendar.to_date(rd), rd), rd);
        }
        EXPECT_EQ(calendar.to_date(fixed_date_t{ -1721279 - 210 }), pawukon_date::from_day(0));
        EXPECT_EQ(calendar.to_date(fixed_date_t{ -1721279 - 1 }), pawukon_date::from_day(209));
    }

    TEST(pawukon_calendar, on_or_before_finds_last_occurrence) {
        for (const auto& reference : reference_dates()) {
            for (int32_t d = 0; d < 210; ++d) {
                const auto date = pawukon_date::from_day(d);
                const auto r = calendar.on_or_before(date, reference);
                EXPECT_LE(r, reference) << date << " ref " << reference;
                EXPECT_LT(reference - r, 210) << date << " ref " << reference;
                EXPECT_EQ(calendar.to_date(r), date) << date << " ref " << reference;
            }
        }
    }

    TEST(pawukon_calendar, on_or_before_round_trip) {
        for (fixed_date_t rd{ -1000 }; rd <= fixed_date_t{ 1000 }; ++rd)
            EXPECT_EQ(calendar.on_or_before(calendar.to_date(rd), rd), rd);
        for (const auto& rd : reference_dates())
            EXPECT_EQ(calendar.on_or_before(calendar.to_date(rd), rd), rd);
    }

    TEST(pawukon_calendar, works_through_cyclic_interface) {
        const calendrical::typed_cyclic_calendar_if<pawukon_date>& cyclic = calendar;
        EXPECT_EQ(cyclic.epoch(), fixed_date_t{ -1721279 });
        EXPECT_EQ(cyclic.cycle_length(), 210);
        EXPECT_EQ(cyclic.on_or_before(pawukon_date::from_day(0), cyclic.epoch() + 209), cyclic.epoch());
        EXPECT_EQ(cyclic.on_or_before(pawukon_date::from_day(0), cyclic.epoch() + 210), cyclic.epoch() + 210);
    }

    TEST(pawukon_calendar, validation_rejects_invalid_dates) {
        const auto valid = pawukon_date::from_day(73);
        ASSERT_TRUE(calendar.is_valid_date(valid));

        std::vector<pawukon_date> invalid;
        // Each component out of range, one at a time.
        auto with = [&](auto member, int32_t value) { auto d = valid; d.*member = value; invalid.push_back(d); };
        with(&pawukon_date::dwiwara, 0);
        with(&pawukon_date::dwiwara, 3);
        with(&pawukon_date::triwara, 0);
        with(&pawukon_date::triwara, 4);
        with(&pawukon_date::caturwara, 0);
        with(&pawukon_date::caturwara, 5);
        with(&pawukon_date::pancawara, 0);
        with(&pawukon_date::pancawara, 6);
        with(&pawukon_date::sadwara, 0);
        with(&pawukon_date::sadwara, 7);
        with(&pawukon_date::saptawara, 0);
        with(&pawukon_date::saptawara, 8);
        with(&pawukon_date::asatawara, 0);
        with(&pawukon_date::asatawara, 9);
        with(&pawukon_date::sangawara, 0);
        with(&pawukon_date::sangawara, 10);
        with(&pawukon_date::dasawara, -1);
        with(&pawukon_date::dasawara, 10);
        // In range but inconsistent with the position given by pancawara, sadwara and saptawara.
        with(&pawukon_date::triwara, valid.triwara % 3 + 1);
        with(&pawukon_date::asatawara, valid.asatawara % 8 + 1);
        with(&pawukon_date::sangawara, valid.sangawara % 9 + 1);
        with(&pawukon_date::dasawara, (valid.dasawara + 1) % 10);
        auto flipped = valid;
        flipped.luang = !valid.luang;
        invalid.push_back(flipped);

        for (const auto& date : invalid) {
            EXPECT_FALSE(calendar.is_valid_date(date)) << date;
            EXPECT_THROW(static_cast<void>(calendar.on_or_before(date, 0_rd)), std::invalid_argument) << date;
            EXPECT_THROW(static_cast<void>(calendar.in_range(date, calendrical::interval_closed_between(0_rd, 1000_rd))), std::invalid_argument) << date;
        }

        // Pancawara, sadwara and saptawara determine the date: each of their 210
        // combinations names a distinct position whose date keeps them.
        std::set<int32_t> positions;
        for (int32_t p = 1; p <= 5; ++p)
            for (int32_t s = 1; s <= 6; ++s)
                for (int32_t w = 1; w <= 7; ++w) {
                    const auto day = pawukon_date{ false, 1, 1, 1, p, s, w, 1, 1, 0 }.day();
                    const auto date = pawukon_date::from_day(day);
                    EXPECT_EQ(date.pancawara, p);
                    EXPECT_EQ(date.sadwara, s);
                    EXPECT_EQ(date.saptawara, w);
                    positions.insert(day);
                }
        EXPECT_EQ(positions.size(), 210u);
    }

    TEST(pawukon_calendar, positions_in_range_matches_brute_force) {
        using calendrical::interval;
        const std::vector<interval<fixed_date_t>> ranges{
            calendrical::interval_closed_between(fixed_date_t{ -1721500 }, fixed_date_t{ -1721000 }),
            calendrical::interval_left_closed_between(-1000_rd, 1000_rd),
            calendrical::interval_right_closed_between(-30_rd, 15_rd),
            calendrical::interval_opened_between(730119_rd, 730485_rd),
            calendrical::interval_closed_between(5_rd, 5_rd),
        };
        for (const auto& range : ranges) {
            for (const auto period : { 1LL, 15LL, 35LL, 210LL }) {
                for (int64_t day = -3; day < 212; day += 7) {
                    std::vector<fixed_date_t> expected;
                    for (auto rd = range.min; rd <= range.max; ++rd)
                        if (range.contains(rd) && calendrical::mod(calendar.day_from_fixed(rd) - day, period) == 0)
                            expected.push_back(rd);
                    EXPECT_EQ(calendar.positions_in_range(day, period, range), expected)
                        << "day " << day << " period " << period << " range " << range.min << ".." << range.max;
                }
            }
        }
        // Extreme day arguments match their reduced positions.
        for (const auto period : { 1LL, 15LL, 35LL, 210LL }) {
            for (const auto day : { std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::min() + 1,
                                    std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max() - 1 }) {
                const auto reduced = (day % period + period) % period;
                for (const auto& range : ranges)
                    EXPECT_EQ(calendar.positions_in_range(day, period, range), calendar.positions_in_range(reduced, period, range))
                        << "day " << day << " period " << period;
            }
        }
        for (const auto period : { 0LL, -15LL, 11LL, 420LL })
            EXPECT_THROW(static_cast<void>(calendar.positions_in_range(0, period, ranges[1])), std::invalid_argument) << period;
    }

    TEST(pawukon_calendar, in_range_lists_every_occurrence) {
        const auto range = calendrical::interval_left_closed_between(-1000_rd, 1000_rd);
        for (int32_t d = 0; d < 210; ++d) {
            const auto date = pawukon_date::from_day(d);
            const auto found = calendar.in_range(date, range);
            ASSERT_FALSE(found.empty());
            EXPECT_LT(found.front() - range.min, 210);
            EXPECT_GE(range.max - found.back(), 1);
            EXPECT_LE(range.max - found.back(), 210);
            for (size_t i = 0; i < found.size(); ++i) {
                EXPECT_EQ(calendar.to_date(found[i]), date);
                if (i > 0) {
                    EXPECT_EQ(found[i] - found[i - 1], 210);
                }
            }
        }
    }

    TEST(pawukon_date, string_representation) {
        const auto date = pawukon_date::from_day(0);
        const std::string expected =
            "pawukon_date{luang:false,dwiwara:1,triwara:1,caturwara:1,pancawara:2,"
            "sadwara:1,saptawara:1,asatawara:1,sangawara:1,dasawara:5}";
        EXPECT_EQ(date.to_string(), expected);
        std::ostringstream oss;
        oss << date;
        EXPECT_EQ(oss.str(), expected);
    }
}
