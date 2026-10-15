#pragma once

#include <concepts>
#include <utility>

/**
 * Compile-time probes for the unit-safety checks of angle_t, fixed_date_t and moment_t.
 *
 * Each `can_*` concept tells whether an expression compiles at all; each `*_result_t`
 * alias names its result type, and `*_yields` combines both. Tests state the expected
 * algebra with `static_assert`, so a regression fails the build instead of a test run.
 */
namespace tests::operator_traits {
    template<typename A, typename B>
    concept can_add = requires(const A& a, const B& b) { a + b; };
    template<typename A, typename B>
    concept can_subtract = requires(const A& a, const B& b) { a - b; };
    template<typename A, typename B>
    concept can_multiply = requires(const A& a, const B& b) { a * b; };
    template<typename A, typename B>
    concept can_divide = requires(const A& a, const B& b) { a / b; };
    template<typename A, typename B>
    concept can_compare = requires(const A& a, const B& b) { a < b; a <= b; a > b; a >= b; };
    template<typename A, typename B>
    concept can_equal = requires(const A& a, const B& b) { a == b; a != b; };

    template<typename A, typename B>
    using add_result_t = decltype(std::declval<const A&>() + std::declval<const B&>());
    template<typename A, typename B>
    using subtract_result_t = decltype(std::declval<const A&>() - std::declval<const B&>());
    template<typename A, typename B>
    using multiply_result_t = decltype(std::declval<const A&>() * std::declval<const B&>());
    template<typename A, typename B>
    using divide_result_t = decltype(std::declval<const A&>() / std::declval<const B&>());

    template<typename A, typename B, typename R>
    concept add_yields = can_add<A, B> && std::same_as<add_result_t<A, B>, R>;
    template<typename A, typename B, typename R>
    concept subtract_yields = can_subtract<A, B> && std::same_as<subtract_result_t<A, B>, R>;
    template<typename A, typename B, typename R>
    concept multiply_yields = can_multiply<A, B> && std::same_as<multiply_result_t<A, B>, R>;
    template<typename A, typename B, typename R>
    concept divide_yields = can_divide<A, B> && std::same_as<divide_result_t<A, B>, R>;
}
