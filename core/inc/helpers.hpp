#pragma once

#include <array>
#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <functional>
#include <numbers>
#include <ranges>
#include <vector>
#include "range.hpp"
#include "small_rational.hpp"

#include <chrono>

namespace calendrical{
    using rational = boost::multiprecision::cpp_rational;
    using bigint = boost::multiprecision::cpp_int;

    bigint floor(const rational& r);
    bigint ceil(const rational& r);
    bigint round(const rational& r);
    bigint trunc(const rational& r);

    bool is_integer(double x);
    bool is_integer(const rational& x);

    static constexpr int sign(double y)
    {
        if (y < 0)
            return -1;
        if (y > 0)
            return 1;
        return 0;
    }
    static constexpr int sign(int64_t y)
    {
        if (y < 0)
            return -1;
        if (y > 0)
            return 1;
        return 0;
    }
    int sign(const rational& y);
    int sign(const bigint& y);

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
        int64_t remainder = x % y;

        // If the signs of x and y are different and the remainder is non-zero,
        // adjust the quotient to floor division.
        if (((x < 0 && y > 0) || (x > 0 && y < 0)) && remainder != 0)
        {
            quotient--;
        }

        return quotient;
    }
    template<std::integral T1, std::integral T2, typename = std::enable_if_t<sizeof(T1) < sizeof(int64_t) && sizeof(T2) < sizeof(int64_t)>>
    static constexpr int64_t floor_div(T1 x, T2 y) {
        return floor_div(static_cast<int64_t>(x), static_cast<int64_t>(y));
    }
    double floor_div(double x, double y);
    rational floor_div(const rational& x, const rational& y);
    bigint floor_div(const bigint& x, const bigint& y);

    static constexpr small_rational floor_div(const small_rational& x, const small_rational& y) {
        return small_rational::floor(x / y);
    }

    static constexpr int64_t mod(int64_t x, int64_t y)
    {
        return x - y * floor_div(x, y);
    }
    template<std::integral T1, std::integral T2, typename = std::enable_if_t<sizeof(T1) < sizeof(int64_t) && sizeof(T2) < sizeof(int64_t)>>
    static constexpr int64_t mod(T1 x, T2 y) {
        return mod(static_cast<int64_t>(x), static_cast<int64_t>(y));
    }
    double mod(double x, double y);
    bigint mod(const bigint& x, const bigint& y);
    rational mod(const rational& x, const rational& y);
    static constexpr small_rational mod(const small_rational& x, const small_rational& y) {
        return x - y * floor_div(x, y);
    }

    static constexpr int64_t interval_modulus(int64_t x, int64_t a, int64_t b)
    {
        if (a == b)
            return x;
        return a + mod(x - a, b - a);
    }
    template<std::integral T1, std::integral T2, std::integral T3, typename = std::enable_if_t<sizeof(T1) < sizeof(int64_t) && sizeof(T2) < sizeof(int64_t) && sizeof(T3) < sizeof(int64_t)>>
    static constexpr int64_t interval_modulus(T1 x, T2 a, T3 b) {
        return interval_modulus(static_cast<int64_t>(x), static_cast<int64_t>(a), static_cast<int64_t>(b));
    }
    double interval_modulus(double x, double a, double b);
    bigint interval_modulus(const bigint& x, const bigint& a, const bigint& b);
    rational interval_modulus(const rational& x, const rational& a, const rational& b);
    static constexpr small_rational interval_modulus(const small_rational& x, const small_rational& a, const small_rational& b) {
        if (a == b)
            return x;
        return a + mod(x - a, b - a);
    }

    static constexpr int64_t adjusted_remainder(int64_t x, int64_t b)
    {
        const auto m = mod(x, b);
        return m == 0 ? b : m;
    }
    template<std::integral T1, std::integral T2, typename = std::enable_if_t<sizeof(T1) < sizeof(int64_t) && sizeof(T2) < sizeof(int64_t)>>
    static constexpr int64_t adjusted_remainder(T1 x, T2 b) {
        return adjusted_remainder(static_cast<int64_t>(x), static_cast<int64_t>(b));
    }
    bigint adjusted_remainder(const bigint& x, const bigint& b);

    template <typename T>
    using predicate_t = std::function<bool(const T&)>;
    template <typename T>
    using double_predicate_t = std::function<bool(const T&, const T&)>;
    template <typename T>
    using value_function_t = std::function<T(const T&)>;

    int64_t sigma(const predicate_t<int64_t> &p, const value_function_t<int64_t>& f, int64_t k);
    int64_t pi(const predicate_t<int64_t>& p, const value_function_t<int64_t>& f, int64_t k);

    int64_t least_passing(const predicate_t<int64_t>& phi, int64_t d0);
    int64_t last_passing(const predicate_t<int64_t>& phi, int64_t d0);

    double binary_search(const predicate_t<double>& phi, const double_predicate_t<double>& psi, double u, double v);
    rational binary_search(const predicate_t<rational>& phi, const double_predicate_t<rational>& psi, const rational& u, const rational& v);

    double inverse_angular(const value_function_t<double>& f, double y, double a, double b);
    rational inverse_angular(const value_function_t<rational>& f, const rational& y, const rational& a, const rational& b);

    template<typename T>
    std::vector<T> positions_in_range(const T& p, const T& c, const T& D, const range<T>& rng)
    {
        auto a = rng.min;
        auto tmp = p - D;
        std::vector<T> result{};
        for (auto date = interval_modulus(tmp, a, a + c); date < rng.max; a += c, date = interval_modulus(tmp, a, a + c))
            result.push_back(date);
        return result;
    }

    static constexpr auto D2R = std::numbers::pi / 180.0;
    static constexpr auto R2D = 180.0 / std::numbers::pi;

    static constexpr double radians(double degrees) { return degrees * D2R; }
    static constexpr double degrees(double radians) { return radians * R2D; }

    template <typename T, typename UT = std::underlying_type_t<T>, std::enable_if_t<std::is_enum_v<T>,  int> = 0>
    static constexpr UT operator-(const T& lhs, const T& rhs)
    {
        return static_cast<UT>(lhs) - static_cast<UT>(rhs);
    }

    template <typename T>
    static constexpr bool in(const range<T>& range, const T& value) { return range.contains(value); }

    template <typename T>
    static constexpr bool not_in(const range<T>& range, const T& value) { return !range.contains(value); }

    template <typename T, typename ContainerT>
    static constexpr bool in(const ContainerT& cont, const T& value)
    {
        return std::find(std::begin(cont), std::end(cont), value) != std::end(cont);
    }
    template <typename T, typename ContainerT>
    static constexpr bool not_in(const ContainerT& cont, const T& value)
    {
        return std::find(std::begin(cont), std::end(cont), value) == std::end(cont);
    }

    template<typename T>
    static constexpr bool in(const std::initializer_list<T>& list, const T& value) 
    {
        return std::find(std::begin(list), std::end(list), value) != std::end(list);
    }
    template<typename T>
    static constexpr bool not_in(const std::initializer_list<T>& list, const T& value)
    {
        return std::find(std::begin(list), std::end(list), value) == std::end(list);
    }

    template <typename TIn, typename TOut>
        value_function_t<double> make_value_function(const std::function<TOut(const TIn&)>& func)
    {
        return [func](const double& x)
        {
            auto i = static_cast<TIn>(x);
            return static_cast<double>(func(i));
        };
    }

    template <typename TIn, typename TOut>
    value_function_t<double> make_value_function(std::function<TOut(const TIn&)>&& func)
    {
        return [func](const double& x)
        {
            auto i = static_cast<TIn>(x);
            return static_cast<double>(func(i));
        };
    }
    template <typename TIn, typename TOut>
    value_function_t<double> make_value_function(TOut(&_Func)(const TIn&))
    {
        return [&_Func](const double& x)
        {
            auto i = static_cast<TIn>(x);
            return static_cast<double>(_Func(i));
        };
    }
    template <typename TIn, typename TOut>
    value_function_t<rational> make_rational_value_function(std::function<TOut(const TIn&)>&& func)
    {
        return [func](const rational& x)
            {
                auto i = static_cast<TIn>(x);
                return static_cast<rational>(func(i));
            };
    }
    template <typename TIn, typename TOut>
    value_function_t<rational> make_rational_value_function(TOut(&_Func)(const TIn&))
    {
        return [&_Func](const rational& x)
            {
                auto i = static_cast<TIn>(x);
                return static_cast<rational>(_Func(i));
            };
    }

    template<typename TIn>
    predicate_t<int64_t> make_predicate(std::function<bool(const TIn&)>&& func) {
        return [&func](const int64_t& x) { return func(static_cast<TIn>(x)); };
    }

    template<typename TIn>
    predicate_t<int64_t> make_predicate(const std::function<bool(const TIn&)>& func) {
        return [&func](const int64_t& x) { return func(static_cast<TIn>(x)); };
    }

    template<typename TIn>
    predicate_t<int64_t> make_predicate(bool(&_Func)(const TIn&)) {
        return [&_Func](const int64_t& x) { return _Func(static_cast<TIn>(x)); };
    }

    template <typename T, std::size_t N>
    constexpr T cross_product(const std::array<T, N>& a, const std::array<T, N>& b)
    {
        T r = 0;
        for (std::size_t i = 0; i < N; ++i)
            r += a[i] * b[i];
        return r;
    }

    template <typename T>
    struct bind_constructor
    {
        template <typename... Args>
        constexpr T operator()(Args &&...args) const
        {
            return T(std::forward<Args>(args)...);
        }
    };
}