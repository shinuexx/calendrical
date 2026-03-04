#pragma once

#include <array>
#include <functional>
#include <numbers>
#include <ranges>
#include <vector>
#include "interval.hpp"

#include <chrono>

namespace calendrical{

    /** @brief Returns true if @p x is an integer-valued floating-point number. */
    bool is_integer(double x);

    /** @brief Sign function for floating-point values: -1, 0, or 1. */
    static constexpr int sign(double y)
    {
        if (y < 0)
            return -1;
        if (y > 0)
            return 1;
        return 0;
    }
    /** @brief Sign function for integral values: -1, 0, or 1. */
    static constexpr int sign(int64_t y)
    {
        if (y < 0)
            return -1;
        if (y > 0)
            return 1;
        return 0;
    }

    /** @brief Floor division for integers (rounds toward negative infinity). */
    static constexpr int64_t floor_div(int64_t x, int64_t y)
    {
        if (y == 0)
        {
            // Handle division by zero error
            // You can customize the behavior here, such as throwing an exception
            // or returning a special value.
            // In this example, we return zero.
            return 0;
        }

        int64_t quotient = x / y;

        // If the signs of x and y are different and the remainder is non-zero,
        // adjust the quotient to floor division.
        if (const int64_t remainder = x % y; ((x < 0 && y > 0) || (x > 0 && y < 0)) && remainder != 0)
        {
            quotient--;
        }

        return quotient;
    }
    template<std::integral T1, std::integral T2, typename = std::enable_if_t<sizeof(T1) < sizeof(int64_t) && sizeof(T2) < sizeof(int64_t)>>
    /** @brief Narrow-integral overload forwarding to `int64_t` floor division. */
    static constexpr int64_t floor_div(T1 x, T2 y) {
        return floor_div(static_cast<int64_t>(x), static_cast<int64_t>(y));
    }
    /** @brief Floor division for floating-point values. */
    double floor_div(double x, double y);

    /** @brief Mathematical modulo for integers using @ref floor_div semantics. */
    static constexpr int64_t mod(int64_t x, int64_t y)
    {
        return x - y * floor_div(x, y);
    }
    template<std::integral T1, std::integral T2, typename = std::enable_if_t<sizeof(T1) < sizeof(int64_t) && sizeof(T2) < sizeof(int64_t)>>
    /** @brief Narrow-integral overload forwarding to `int64_t` modulo. */
    static constexpr int64_t mod(T1 x, T2 y) {
        return mod(static_cast<int64_t>(x), static_cast<int64_t>(y));
    }
    /** @brief Mathematical modulo for floating-point values. */
    double mod(double x, double y);

    /** @brief Maps @p x into the half-open interval [`a`, `b`) by modular wraparound. */
    static constexpr int64_t interval_modulus(int64_t x, int64_t a, int64_t b)
    {
        if (a == b)
            return x;
        return a + mod(x - a, b - a);
    }
    template<std::integral T1, std::integral T2, std::integral T3, typename = std::enable_if_t<sizeof(T1) < sizeof(int64_t) && sizeof(T2) < sizeof(int64_t) && sizeof(T3) < sizeof(int64_t)>>
    /** @brief Narrow-integral overload forwarding to `int64_t` interval modulus. */
    static constexpr int64_t interval_modulus(T1 x, T2 a, T3 b) {
        return interval_modulus(static_cast<int64_t>(x), static_cast<int64_t>(a), static_cast<int64_t>(b));
    }
    /** @brief Floating-point interval modulus variant. */
    double interval_modulus(double x, double a, double b);

    /** @brief Remainder in `1..b` instead of `0..b-1` (for divisible values returns `b`). */
    static constexpr int64_t adjusted_remainder(int64_t x, int64_t b)
    {
        const auto m = mod(x, b);
        return m == 0 ? b : m;
    }
    template<std::integral T1, std::integral T2, typename = std::enable_if_t<sizeof(T1) < sizeof(int64_t) && sizeof(T2) < sizeof(int64_t)>>
    /** @brief Narrow-integral overload forwarding to `int64_t` adjusted remainder. */
    static constexpr int64_t adjusted_remainder(T1 x, T2 b) {
        return adjusted_remainder(static_cast<int64_t>(x), static_cast<int64_t>(b));
    }

    template <typename T>
    /** @brief Unary predicate type alias. */
    using predicate_t = std::function<bool(const T&)>;
    template <typename T>
    /** @brief Binary predicate type alias. */
    using double_predicate_t = std::function<bool(const T&, const T&)>;
    template <typename T>
    /** @brief Unary value-transform function alias. */
    using value_function_t = std::function<T(const T&)>;

    /** @brief Summation over values generated from indices satisfying predicate @p p. */
    int64_t sigma(const predicate_t<int64_t> &p, const value_function_t<int64_t>& f, int64_t k);
    /** @brief Product over values generated from indices satisfying predicate @p p. */
    int64_t pi(const predicate_t<int64_t>& p, const value_function_t<int64_t>& f, int64_t k);

    /** @brief Smallest integer >= @p d0 satisfying predicate @p phi. */
    int64_t least_passing(const predicate_t<int64_t>& phi, int64_t d0);
    /** @brief Largest integer <= @p d0 satisfying predicate @p phi. */
    int64_t last_passing(const predicate_t<int64_t>& phi, int64_t d0);

    /** @brief Generic binary search guided by predicates on interval/value. */
    double binary_search(const predicate_t<double>& phi, const double_predicate_t<double>& psi, double u, double v);

    /** @brief Inverts an angular function over interval [`a`, `b`) for target @p y. */
    double inverse_angular(const value_function_t<double>& f, double y, double a, double b);

    template<typename T>
    /** @brief Returns periodic positions within a range using period @p c and offset data. */
    std::vector<T> positions_in_range(const T& p, const T& c, const T& D, const interval<T>& rng)
    {
        auto a = rng.min;
        auto tmp = p - D;
        std::vector<T> result{};
        for (auto date = interval_modulus(tmp, a, a + c); date < rng.max; a += c, date = interval_modulus(tmp, a, a + c))
            result.push_back(date);
        return result;
    }

    /** @brief Degrees-to-radians conversion factor. */
    static constexpr auto D2R = std::numbers::pi / 180.0;
    /** @brief Radians-to-degrees conversion factor. */
    static constexpr auto R2D = 180.0 / std::numbers::pi;

    /** @brief Converts degrees to radians. */
    static constexpr double radians(double degrees) { return degrees * D2R; }
    /** @brief Converts radians to degrees. */
    static constexpr double degrees(double radians) { return radians * R2D; }

    template <typename T, typename UT = std::underlying_type_t<T>, std::enable_if_t<std::is_enum_v<T>,  int> = 0>
    /** @brief Difference of enum values expressed in the underlying type. */
    static constexpr UT operator-(const T& lhs, const T& rhs)
    {
        return static_cast<UT>(lhs) - static_cast<UT>(rhs);
    }

    template <typename T>
    /** @brief Returns whether @p value is contained in an @ref interval. */
    static constexpr bool in(const interval<T>& range, const T& value) { return range.contains(value); }

    template <typename T>
    /** @brief Negated form of @ref in(interval, value). */
    static constexpr bool not_in(const interval<T>& range, const T& value) { return !range.contains(value); }

    template <typename T, typename ContainerT>
    /** @brief Returns whether @p value is present in a generic container. */
    static constexpr bool in(const ContainerT& cont, const T& value)
    {
        return std::find(std::begin(cont), std::end(cont), value) != std::end(cont);
    }
    template <typename T, typename ContainerT>
    /** @brief Negated form of @ref in(cont, value). */
    static constexpr bool not_in(const ContainerT& cont, const T& value)
    {
        return std::find(std::begin(cont), std::end(cont), value) == std::end(cont);
    }

    template<typename T>
    /** @brief Returns whether @p value is present in an initializer list. */
    static constexpr bool in(const std::initializer_list<T>& list, const T& value) 
    {
        return std::find(std::begin(list), std::end(list), value) != std::end(list);
    }
    template<typename T>
    /** @brief Negated form of @ref in(initializer_list, value). */
    static constexpr bool not_in(const std::initializer_list<T>& list, const T& value)
    {
        return std::find(std::begin(list), std::end(list), value) == std::end(list);
    }

    template <typename TIn, typename TOut>
    /** @brief Adapts a typed function to `value_function_t<double>`. */
    value_function_t<double> make_value_function(const std::function<TOut(const TIn&)>& func)
    {
        return [func](const double& x)
        {
            auto i = static_cast<TIn>(x);
            return static_cast<double>(func(i));
        };
    }

    template <typename TIn, typename TOut>
    /** @brief Rvalue overload of @ref make_value_function for `std::function`. */
    value_function_t<double> make_value_function(std::function<TOut(const TIn&)>&& func)
    {
        return [func](const double& x)
        {
            auto i = static_cast<TIn>(x);
            return static_cast<double>(func(i));
        };
    }
    template <typename TIn, typename TOut>
    /** @brief Adapts a function pointer to `value_function_t<double>`. */
    value_function_t<double> make_value_function(TOut(&Func)(const TIn&))
    {
        return [&Func](const double& x)
        {
            auto i = static_cast<TIn>(x);
            return static_cast<double>(Func(i));
        };
    }

    template<typename TIn>
    /** @brief Adapts a typed predicate to `predicate_t<int64_t>` (rvalue overload). */
    predicate_t<int64_t> make_predicate(std::function<bool(const TIn&)>&& func) {
        return [func = std::move(func)](const int64_t& x) { return func(static_cast<TIn>(x)); };
    }

    template<typename TIn>
    /** @brief Adapts a typed predicate to `predicate_t<int64_t>` (const lvalue overload). */
    predicate_t<int64_t> make_predicate(const std::function<bool(const TIn&)>& func) {
        return [func](const int64_t& x) { return func(static_cast<TIn>(x)); };
    }

    template<typename TIn>
    /** @brief Adapts a predicate function pointer to `predicate_t<int64_t>`. */
    predicate_t<int64_t> make_predicate(bool(&Func)(const TIn&)) {
        return [&Func](const int64_t& x) { return Func(static_cast<TIn>(x)); };
    }

    template <typename T, std::size_t N>
    /** @brief Dot product of two fixed-size arrays. */
    constexpr T cross_product(const std::array<T, N>& a, const std::array<T, N>& b)
    {
        T r = 0;
        for (std::size_t i = 0; i < N; ++i)
            r += a[i] * b[i];
        return r;
    }

    template <typename T>
    /**
     * @brief Function object that forwards arguments to `T`'s constructor.
     */
    struct bind_constructor
    {
        template <typename... Args>
        /** @brief Constructs `T` from forwarded arguments. */
        constexpr T operator()(Args &&...args) const
        {
            return T(std::forward<Args>(args)...);
        }
    };
}
