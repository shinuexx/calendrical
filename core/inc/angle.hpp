#pragma once

#include <string>
#include <type_traits>
#include "helpers.hpp"

namespace calendrical
{
    template<typename T>
    struct angle_enum_type : std::false_type{};

    template<typename T>
    concept angle_enum = std::is_enum_v<T> && angle_enum_type<T>::value;

    /**
     * @brief Angle value represented in degrees.
     *
     * The type wraps a `double` and provides basic arithmetic plus trigonometric
     * helpers used throughout astronomical calculations.
     */
    class angle_t
    {
        double m_v;
 
    public:
        /** @brief Constructs zero angle. */
        constexpr angle_t() : m_v{0} {}
        /** @brief Constructs from raw degree value. */
        constexpr explicit angle_t(double v) : m_v{v} {}

        template <angle_enum EnumType>
        /** @brief Constructs from an enum by using its underlying integral value as degrees. */
        constexpr angle_t(const EnumType &v) : m_v{static_cast<double>(static_cast<std::underlying_type_t<EnumType>>(v))} {}

        // ReSharper disable once CppNonExplicitConversionOperator
        /** @brief Converts to raw degree value. */
        constexpr operator double() const { return m_v; } // NOLINT(*-explicit-constructor)

        /** @brief Equality comparison on the raw degree value. */
        constexpr bool operator==(const angle_t &other) const { return m_v == other.m_v; }
        /** @brief Three-way comparison on the raw degree value. */
        constexpr auto operator<=>(const angle_t &other) const { return m_v <=> other.m_v; }

        /** @brief Unary negation. */
        [[nodiscard]] constexpr angle_t operator-() const { return angle_t{-m_v}; }

        /** @brief Adds two angles. */
        [[nodiscard]] constexpr angle_t operator+(const angle_t &right) const { return angle_t{m_v + right.m_v}; }
        /** @brief Subtracts two angles. */
        [[nodiscard]] constexpr angle_t operator-(const angle_t &right) const { return angle_t{m_v - right.m_v}; }
        template <typename T>
            requires std::is_arithmetic_v<T>
        /** @brief Scales an angle by a scalar factor. */
        [[nodiscard]] constexpr angle_t operator*(T right) const { return angle_t{m_v * static_cast<double>(right)}; }
        template <typename T>
            requires std::is_arithmetic_v<T>
        /** @brief Scales an angle by a scalar factor (commutative). */
        [[nodiscard]] friend constexpr angle_t operator*(T left, const angle_t &right) { return right * left; }
        template <typename T>
            requires std::is_arithmetic_v<T>
        /** @brief Divides an angle by a scalar factor. */
        [[nodiscard]] constexpr angle_t operator/(T right) const { return angle_t{m_v / static_cast<double>(right)}; }
        /** @brief Returns the ratio of two angles. */
        [[nodiscard]] constexpr double operator/(const angle_t &right) const { return m_v / right.m_v; }

        /** @brief Creates an angle from degrees/minutes/seconds components. */
        [[nodiscard]] static constexpr angle_t dms(double d, double m = 0, double s = 0) { return angle_t{d + (m + s / 60) / 60}; }

        /** @brief 360 degrees. */
        static const angle_t full_circle;
        /** @brief 180 degrees. */
        static const angle_t half_circle;

        /** @brief Returns a string representation of the angle. */
        friend std::string to_string(const angle_t &angle);

        /** @brief Sine of the angle (input interpreted in degrees). */
        friend double sin(const angle_t &angle);
        /** @brief Cosine of the angle (input interpreted in degrees). */
        friend double cos(const angle_t &angle);
        /** @brief Tangent of the angle (input interpreted in degrees). */
        friend double tan(const angle_t &angle);

        /** @brief Inverse sine, returned as an angle in degrees. */
        friend angle_t asin(double value);
        /** @brief Inverse cosine, returned as an angle in degrees. */
        friend angle_t acos(double value);
        /** @brief Inverse tangent, returned as an angle in degrees. */
        friend angle_t atan(double value);
        /** @brief Quadrant-aware inverse tangent (`atan2`) returned in degrees. */
        friend angle_t arctan(double y, double x);
    };

    inline constexpr angle_t angle_t::full_circle{ 360 };
    inline constexpr angle_t angle_t::half_circle{ 180 };

    /** @brief Returns a string representation of the angle. */
    std::string to_string(const angle_t &angle);

    /** @brief Sine of the angle (degrees). */
    double sin(const angle_t &angle);
    /** @brief Cosine of the angle (degrees). */
    double cos(const angle_t &angle);
    /** @brief Tangent of the angle (degrees). */
    double tan(const angle_t &angle);

    /** @brief Inverse sine result expressed in degrees. */
    angle_t asin(double value);
    /** @brief Inverse cosine result expressed in degrees. */
    angle_t acos(double value);
    /** @brief Inverse tangent result expressed in degrees. */
    angle_t atan(double value);
    /** @brief Quadrant-aware inverse tangent result expressed in degrees. */
    angle_t arctan(double y, double x);
    /** @brief Stream output for @ref angle_t. */
    std::ostream &operator<<(std::ostream &out, const angle_t &angle);
}

/** @brief User-defined angle literal in degrees (floating-point overload). */
constexpr calendrical::angle_t operator""_deg(long double x)
{
    return calendrical::angle_t{static_cast<double>(x)};
}
/** @brief User-defined angle literal in degrees (integer overload). */
constexpr calendrical::angle_t operator""_deg(unsigned long long x)
{
    return calendrical::angle_t{static_cast<double>(x)};
}
