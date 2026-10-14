#pragma once

#include <concepts>
#include <iostream>
#include <sstream>
#include <string>

#include "type_name.hpp"

namespace calendrical {
    /**
     * @brief Interval boundary policy.
     */
    enum class interval_type {
        /** @brief Open interval `(min, max)`. */
        opened,
        /** @brief Closed interval `[min, max]`. */
        closed,
        /** @brief Left-closed interval `[min, max)`. */
        left_closed,
        /** @brief Right-closed interval `(min, max]`. */
        right_closed
    };

    /** @brief Returns a string representation of the boundary policy. */
    inline std::string to_string(const interval_type& type)
    {
        switch (type)
        {
        case interval_type::opened:
            return "opened";
        case interval_type::closed:
            return "closed";
        case interval_type::left_closed:
            return "left_closed";
        case interval_type::right_closed:
            return "right_closed";
        default:
            return "unknown";
        }
    }

    template <typename T>
    /**
     * @brief Generic interval with configurable open/closed endpoints.
     */
    struct interval
    {
        /** @brief Endpoint inclusion policy. */
        interval_type type;
        /** @brief Lower bound. */
        T min;
        /** @brief Upper bound. */
        T max;

    private:
        /** @brief Membership test for (min, max). */
        constexpr bool in_open(const T& value) const { return min < value && value < max; }
        /** @brief Membership test for [min, max]. */
        constexpr bool in_closed(const T& value) const { return min <= value && value <= max; }
        /** @brief Membership test for [min, max). */
        constexpr bool in_left_closed(const T& value) const { return min <= value && value < max; }
        /** @brief Membership test for (min, max]. */
        constexpr bool in_right_closed(const T& value) const { return min < value && value <= max; }

    public:
        /** @brief Returns whether @p value belongs to this interval. */
        constexpr bool contains(const T& value) const
        {
            switch (type)
            {
            case interval_type::opened:
                return in_open(value);
            case interval_type::closed:
                return in_closed(value);
            case interval_type::left_closed:
                return in_left_closed(value);
            case interval_type::right_closed:
                return in_right_closed(value);
            default:
                return false;
            }
        }

        /** @brief Function-call shorthand for @ref contains. */
        constexpr bool operator()(const T& value) const { return contains(value); }

        /** @brief Casts endpoints to another value type while preserving boundary policy. */
        template <typename T2>
            requires (std::convertible_to<T, T2> || std::constructible_from<T2, T>)
        constexpr interval<T2> cast() const { return { type, static_cast<T2>(min), static_cast<T2>(max) }; }

        /** @brief Implicit conversion to another `interval<T2>` via @ref cast. */
        template <typename T2>
            requires (std::convertible_to<T, T2> || std::constructible_from<T2, T>)
        // ReSharper disable once CppNonExplicitConversionOperator
        constexpr operator interval<T2>() const { return cast<T2>(); } // NOLINT(*-explicit-constructor)

        /** @brief Returns the same bounds with both endpoints open. */
        constexpr interval open() const { return { interval_type::opened, min, max }; }
        /** @brief Returns the same bounds with both endpoints closed. */
        constexpr interval close() const { return { interval_type::closed, min, max }; }
        /** @brief Returns the same bounds with a closed left endpoint. */
        constexpr interval close_left() const { return { interval_type::left_closed, min, max }; }
        /** @brief Returns the same bounds with a closed right endpoint. */
        constexpr interval close_right() const { return { interval_type::right_closed, min, max }; }

        /** @brief Equality comparison on type and endpoints. */
        constexpr bool operator==(const interval& other) const = default;
    };

    template <typename T>
    /** @brief Returns a debug-friendly textual representation of an interval. */
    std::string to_string(const interval<T>& value)
    {
        std::ostringstream oss{};
        oss << "interval<" << calendrical::type_name<T>() << ">{type:" << calendrical::to_string(value.type) << ",min:" << value.min << ",max:" << value.max << "}";
        return oss.str();
    }

    template <typename T>
    /** @brief Factory for an open interval `(min, max)`. */
    constexpr interval<T> interval_opened_between(const T& min, const T& max) { return { interval_type::opened, min, max }; }

    template <typename T>
    /** @brief Factory for a closed interval `[min, max]`. */
    constexpr interval<T> interval_closed_between(const T& min, const T& max) { return { interval_type::closed, min, max }; }

    template <typename T>
    /** @brief Factory for a left-closed interval `[min, max)`. */
    constexpr interval<T> interval_left_closed_between(const T& min, const T& max) { return { interval_type::left_closed, min, max }; }

    template <typename T>
    /** @brief Factory for a right-closed interval `(min, max]`. */
    constexpr interval<T> interval_right_closed_between(const T& min, const T& max) { return { interval_type::right_closed, min, max }; }

    template <typename T>
    /** @brief Stream output for @ref calendrical::interval. */
    inline std::ostream& operator<<(std::ostream& out, const interval<T>& value)
    {
        return out << to_string(value);
    }
}
