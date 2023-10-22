#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include "type_name.hpp"

namespace calendrical {
    enum class range_type {
        opened,
        closed,
        left_closed,
        right_closed
    };

    std::string to_string(const range_type& type);

    template <typename T>
    struct range
    {
        range_type type;
        T min;
        T max;

    private:
        constexpr bool in_open(const T& value) const { return (min < value) && (value < max); }
        constexpr bool in_closed(const T& value) const { return (min <= value) && (value <= max); }
        constexpr bool in_left_closed(const T& value) const { return (min <= value) && (value < max); }
        constexpr bool in_right_closed(const T& value) const { return (min < value) && (value <= max); }

    public:
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
        constexpr bool operator()(const T& value) const { return contains(value); }

        template <typename T2, std::enable_if_t<std::is_convertible_v<T, T2>, int> = 0>
        constexpr range<T2> cast() const { return { type, (T2)(min), (T2)(max) }; }
        template <typename T2, std::enable_if_t<std::is_convertible_v<T, T2>, int> = 0>
        constexpr operator range<T2>() const { return cast<T2>(); }

        constexpr range<T> open() const { return { range_type::opened, min, max }; }
        constexpr range<T> close() const { return { range_type::closed, min, max }; }
        constexpr range<T> close_left() const { return { range_type::left_closed, min, max }; }
        constexpr range<T> close_right() const { return { range_type::right_closed, min, max }; }

        constexpr bool operator==(const range<T>& other) const { return (type == other.type) && (min == other.min) && (max == other.max); }
    };

    template <typename T>
    std::string to_string(const range<T>& range)
    {
        std::ostringstream oss{};
        oss << "range<" << calendrical::type_name<T>() << ">{type:" << calendrical::to_string(range.type) << ",min:" << range.min << ",max:" << range.max << "}";
        return oss.str();
    }

    template <typename T>
    constexpr range<T> range_opened_between(const T& min, const T& max) { return { range_type::opened, min, max }; }
    template <typename T>
    constexpr range<T> range_closed_between(const T& min, const T& max) { return { range_type::closed, min, max }; }
    template <typename T>
    constexpr range<T> range_left_closed_between(const T& min, const T& max) { return { range_type::left_closed, min, max }; }
    template <typename T>
    constexpr range<T> range_right_closed_between(const T& min, const T& max) { return { range_type::right_closed, min, max }; }
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const calendrical::range<T>& range)
{
    return out << calendrical::to_string(range);
}