#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "lazy.hpp"
#include "time_and_astronomy.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace tests {
    using namespace calendrical;
    using namespace std::chrono_literals;

    TEST(lazy_test, concurrent_load_initializes_once) {
        std::atomic<int> calls{0};
        lazy<int> value{std::function<int()>([&calls]() {
            calls.fetch_add(1, std::memory_order_relaxed);
            std::this_thread::sleep_for(2ms);
            return 42;
        })};

        constexpr int thread_count = 16;
        constexpr int iterations_per_thread = 32;
        std::atomic<int> bad_reads{0};
        std::vector<std::thread> threads;
        threads.reserve(thread_count);

        for (int i = 0; i < thread_count; ++i) {
            threads.emplace_back([&]() {
                for (int j = 0; j < iterations_per_thread; ++j) {
                    if (*value != 42) {
                        bad_reads.fetch_add(1, std::memory_order_relaxed);
                    }
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }

        EXPECT_EQ(bad_reads.load(std::memory_order_relaxed), 0);
        EXPECT_EQ(calls.load(std::memory_order_relaxed), 1);
    }

    TEST(lazy_test, const_correctness_and_mutation) {
        lazy<std::string> l{std::function<std::string()>([] { return "hello"; })};
        const auto& cl = l;

        static_assert(std::is_same_v<decltype(l.load()), std::string&>);
        static_assert(std::is_same_v<decltype(*l), std::string&>);
        static_assert(std::is_same_v<decltype(l.operator->()), std::string*>);

        static_assert(std::is_same_v<decltype(cl.load()), const std::string&>);
        static_assert(std::is_same_v<decltype(*cl), const std::string&>);
        static_assert(std::is_same_v<decltype(cl.operator->()), const std::string*>);

        EXPECT_EQ(*cl, "hello");
        *l += " world";
        EXPECT_EQ(*cl, "hello world");
        EXPECT_EQ(cl->length(), 11u);

        l->clear();
        EXPECT_TRUE(cl->empty());
    }

    TEST(lazy_test, constructors_and_deferred_default) {
        lazy<int> def;
        EXPECT_EQ(*def, 0);

        lazy<int> val{123};
        EXPECT_EQ(*val, 123);

        auto def_deferred = make_deferred_default_lazy<int>();
        EXPECT_EQ(*def_deferred, 0);
    }

    TEST(lazy_test, explicit_conversions) {
        lazy<int> l{std::function<int()>([] { return 99; })};
        int val_copy = static_cast<int>(l);
        EXPECT_EQ(val_copy, 99);

        int& ref = static_cast<int&>(l);
        ref = 100;
        EXPECT_EQ(*l, 100);

        const auto& cl = l;
        const int& cref = static_cast<const int&>(cl);
        EXPECT_EQ(cref, 100);
    }

    TEST(lazy_test, compile_time_constants) {
        static_assert(time_and_astronomy::J2000 == moment_t{ 730120.5 });
        static_assert(calendars::gregorian_calendar::standard.epoch() == fixed_date_t{ 1 });

        constexpr auto j2000_val = static_cast<double>(time_and_astronomy::J2000);
        static_assert(j2000_val == 730120.5);

        EXPECT_EQ(calendars::gregorian_calendar::standard.to_fixed({ 2000, month::january, 1 }), fixed_date_t{ 730120 });
    }
}
