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
        /** @brief Angle in degrees. */
        double m_v;
 
    public:
        /** @brief Constructs zero angle. */
        constexpr angle_t() : m_v{0} {}
        /** @brief Constructs from raw degree value. */
        constexpr explicit angle_t(double v) : m_v{v} {}

        template <angle_enum EnumType>
        /** @brief Constructs from an enum by using its underlying integral value as degrees. */
        constexpr angle_t(const EnumType &v) : m_v{static_cast<double>(static_cast<std::underlying_type_t<EnumType>>(v))} {}

        /** @brief Explicitly converts to raw degree value. */
        explicit constexpr operator double() const { return m_v; }
        /** @brief Raw degree value in degrees. */
        [[nodiscard]] constexpr double degrees() const { return m_v; }
        /** @brief Day fraction corresponding to this angle (degrees / 360°). */
        [[nodiscard]] constexpr double day_fraction() const { return m_v / 360.0; }

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
        /** @brief Scales an angle by a scalar factor. */
        template <typename T>
            requires std::is_arithmetic_v<T>
        [[nodiscard]] constexpr angle_t operator*(T right) const { return angle_t{m_v * static_cast<double>(right)}; }
        /** @brief Scales an angle by a scalar factor (commutative). */
        template <typename T>
            requires std::is_arithmetic_v<T>
        [[nodiscard]] friend constexpr angle_t operator*(T left, const angle_t &right) { return right * left; }
        /** @brief Divides an angle by a scalar factor. */
        template <typename T>
            requires std::is_arithmetic_v<T>
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

    /** @brief Floor modulo of @p x by @p y degrees; the result stays an angle in [0, @p y). */
    [[nodiscard]] inline angle_t mod(const angle_t &x, double y) { return angle_t{mod(x.degrees(), y)}; }
    /** @brief Floor modulo of @p x by the angle @p y; the result stays an angle in [0, @p y). */
    [[nodiscard]] inline angle_t mod(const angle_t &x, const angle_t &y) { return mod(x, y.degrees()); }
    /** @brief Maps @p x into the half-open angular range [`a`, `b`) by modular wraparound. */
    [[nodiscard]] inline angle_t interval_modulus(const angle_t &x, const angle_t &a, const angle_t &b)
    {
        return angle_t{interval_modulus(x.degrees(), a.degrees(), b.degrees())};
    }
    /** @brief Number of whole @p y steps contained in @p x (floor of the ratio, dimensionless). */
    [[nodiscard]] inline double floor_div(const angle_t &x, const angle_t &y) { return floor_div(x.degrees(), y.degrees()); }
    /** @brief Largest whole number of degrees not greater than @p x. */
    [[nodiscard]] angle_t floor(const angle_t &x);
    /** @brief Smallest whole number of degrees not less than @p x. */
    [[nodiscard]] angle_t ceil(const angle_t &x);
    /** @brief Absolute value of the angle. */
    [[nodiscard]] constexpr angle_t abs(const angle_t &x) { return x.degrees() < 0 ? -x : x; }
    /** @brief Converts an angle to a day fraction (angle / 360°), as a dimensionless scalar. */
    [[nodiscard]] constexpr double day_fraction(const angle_t &a) { return a.day_fraction(); }
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
