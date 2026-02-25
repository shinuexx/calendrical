#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "lazy.hpp"

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
}
