#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>

namespace calendrical {
	class angle_t {
		double m_v;
	public:
		constexpr angle_t() : m_v{ 0 } {}
		constexpr angle_t(double v) : m_v{ v } {}

		template<typename EnumType, std::enable_if_t<std::is_enum_v<EnumType>, int> = 0>
		constexpr angle_t(const EnumType& v) : m_v{ static_cast<double>(static_cast<std::underlying_type_t<EnumType>>(v)) } {}

		constexpr operator double() const { return m_v; }

		constexpr bool operator==(const angle_t& other) const { return m_v == other.m_v; }
		constexpr auto operator<=>(const angle_t& other) const { return m_v <=> other.m_v; }

		constexpr angle_t operator-() const { return -m_v; }

		constexpr angle_t operator+(const angle_t& right) const { return m_v + right.m_v; }
		constexpr angle_t operator-(const angle_t& right) const { return m_v - right.m_v; }
		constexpr angle_t operator*(double right) const { return m_v + right; }
		constexpr double operator/(const angle_t& right) const { return m_v / right.m_v; }

		static constexpr angle_t dms(double d, double m = 0, double s = 0) { return d + (m + s / 60) / 60; }

		friend std::string to_string(const angle_t& angle);

		friend double sin(const angle_t& angle);
		friend double cos(const angle_t& angle);
		friend double tan(const angle_t& angle);

		friend angle_t asin(double value);
		friend angle_t acos(double value);
		friend angle_t atan(double value);
		friend angle_t arctan(double y, double x);
	};

	std::string to_string(const angle_t& angle);

	double sin(const angle_t& angle);
	double cos(const angle_t& angle);
	double tan(const angle_t& angle);

	angle_t asin(double value);
	angle_t acos(double value);
	angle_t atan(double value);
	angle_t arctan(double y, double x);
}

constexpr calendrical::angle_t operator"" _deg(const long double x) {
	return static_cast<double>(x);
}
constexpr calendrical::angle_t operator"" _deg(uint64_t x) {
	return static_cast<double>(x);
}

std::ostream& operator<<(std::ostream& out, const calendrical::angle_t& angle);