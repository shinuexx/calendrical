#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <concepts>
#include "type_name.hpp"

namespace calendrical {
    /**
     * @brief Interval boundary policy.
     */
    enum class range_type {
        opened,
        closed,
        left_closed,
        right_closed
    };

    /** @brief Returns a string representation of the boundary policy. */
    std::string to_string(const range_type& type);

    template <typename T>
    /**
     * @brief Generic interval with configurable open/closed endpoints.
     */
    struct range
    {
        /** @brief Endpoint inclusion policy. */
        range_type type;
        /** @brief Lower bound. */
        T min;
        /** @brief Upper bound. */
        T max;

    private:
        constexpr bool in_open(const T& value) const { return min < value && value < max; }
        constexpr bool in_closed(const T& value) const { return min <= value && value <= max; }
        constexpr bool in_left_closed(const T& value) const { return min <= value && value < max; }
        constexpr bool in_right_closed(const T& value) const { return min < value && value <= max; }

    public:
        /** @brief Returns whether @p value belongs to this interval. */
        constexpr bool contains(const T& value) const
        {
            switch (type)
            {
            case range_type::opened:
                return in_open(value);
            case range_type::closed:
                return in_closed(value);
            case range_type::left_closed:
                return in_left_closed(value);
            case range_type::right_closed:
                return in_right_closed(value);
            default:
                return false;
            }
        }
        /** @brief Function-call shorthand for @ref contains. */
        constexpr bool operator()(const T& value) const { return contains(value); }

        template <typename T2, typename = std::enable_if_t<std::convertible_to<T, T2> || std::constructible_from<T, T2>>>
        /** @brief Casts endpoints to another value type while preserving boundary policy. */
        constexpr range<T2> cast() const { return { type, static_cast<T2>(min), static_cast<T2>(max) }; }
        template <typename T2, typename = std::enable_if_t<std::convertible_to<T, T2> || std::constructible_from<T, T2>>>
        // ReSharper disable once CppNonExplicitConversionOperator
        /** @brief Implicit conversion to another `range<T2>` via @ref cast. */
        constexpr operator range<T2>() const { return cast<T2>(); } // NOLINT(*-explicit-constructor)

        /** @brief Returns the same bounds with both endpoints open. */
        constexpr range open() const { return { range_type::opened, min, max }; }
        /** @brief Returns the same bounds with both endpoints closed. */
        constexpr range close() const { return { range_type::closed, min, max }; }
        /** @brief Returns the same bounds with a closed left endpoint. */
        constexpr range close_left() const { return { range_type::left_closed, min, max }; }
        /** @brief Returns the same bounds with a closed right endpoint. */
        constexpr range close_right() const { return { range_type::right_closed, min, max }; }

        /** @brief Equality comparison on type and endpoints. */
        constexpr bool operator==(const range& other) const = default;
    };

    template <typename T>
    /** @brief Returns a debug-friendly textual representation of a range. */
    std::string to_string(const range<T>& range)
    {
        std::ostringstream oss{};
        oss << "range<" << calendrical::type_name<T>() << ">{type:" << calendrical::to_string(range.type) << ",min:" << range.min << ",max:" << range.max << "}";
        return oss.str();
    }

    template <typename T>
    /** @brief Factory for an open interval `(min, max)`. */
    constexpr range<T> range_opened_between(const T& min, const T& max) { return { range_type::opened, min, max }; }
    template <typename T>
    /** @brief Factory for a closed interval `[min, max]`. */
    constexpr range<T> range_closed_between(const T& min, const T& max) { return { range_type::closed, min, max }; }
    template <typename T>
    /** @brief Factory for a left-closed interval `[min, max)`. */
    constexpr range<T> range_left_closed_between(const T& min, const T& max) { return { range_type::left_closed, min, max }; }
    template <typename T>
    /** @brief Factory for a right-closed interval `(min, max]`. */
    constexpr range<T> range_right_closed_between(const T& min, const T& max) { return { range_type::right_closed, min, max }; }
}

template <typename T>
/** @brief Stream output for @ref calendrical::range. */
std::ostream& operator<<(std::ostream& out, const calendrical::range<T>& range)
{
    return out << calendrical::to_string(range);
}
