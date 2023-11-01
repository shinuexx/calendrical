#pragma once

#include <cstdint>
#include <numeric>
#include <string>

namespace calendrical {
    class small_rational {
        int64_t m_n;
        int64_t m_d;

        static constexpr std::tuple<int64_t, int64_t> move_sign(int64_t n, int64_t d) {
            if (d < 0)
                return { -n,-d };
            else
                return { n,d };
        }

        static constexpr std::tuple<int64_t, int64_t> simplify(const std::tuple<int64_t, int64_t>& nd) {
            auto [n, d] = nd;
            if (d > 1) {
                auto gcd = std::gcd(n, d);
                if (gcd > 1) {
                    n /= gcd;
                    d /= gcd;
                }
            }
            if (d == 0)
                n = n < 0 ? -1 : 1;
            return { n,d };
        }

        static constexpr auto _abs(auto v) { return v < 0 ? -v : v; }
        static constexpr auto _pow(auto x, int32_t e) {
            decltype(x) r = 1;
            while (e > 0) {
                if (e & 1) {
                    r *= x;
                }
                x *= x;
                e >>= 1;
            }
            return r;
        }

    public:
        static const small_rational zero;
        static const small_rational one;
        static const small_rational half;
        static const small_rational minus_one;
        static const small_rational minus_half;

        constexpr small_rational(int64_t n, int64_t d) {
            std::tie(m_n, m_d) = simplify(move_sign(n, d));
        }
        constexpr small_rational(int64_t n) : m_n{ n }, m_d{ 1 } {}
        constexpr small_rational() : m_n{ 0 }, m_d{ 1 } {}

        constexpr explicit operator double() const {
            return static_cast<double>(m_n) / static_cast<double>(m_d);
        }

        constexpr explicit operator int64_t() const {
            return m_n / m_d;
        }

        constexpr bool operator==(const small_rational& other) const { return (m_n == other.m_n) && (m_d == other.m_d); }
        constexpr std::partial_ordering operator<=>(const small_rational& other) const {
            if (m_d == other.m_d)
                return m_n <=> other.m_d;
            else if (m_n == other.m_n)
                return other.m_d <=> m_d;
            else
                return ((double)*this) <=> (double)other;
        }

        std::string to_string() const;

        constexpr small_rational operator+() const { return *this; }
        constexpr small_rational operator-() const { return { -m_n, m_d }; }
        constexpr small_rational operator+(const small_rational& rhs) const {
            auto n = m_n * rhs.m_d + m_d * rhs.m_n;
            auto d = m_d * rhs.m_d;
            return { n,d };
        }
        constexpr small_rational operator-(const small_rational& rhs) const {
            auto n = m_n * rhs.m_d - m_d * rhs.m_n;
            auto d = m_d * rhs.m_d;
            return { n,d };
        }
        constexpr small_rational& operator++() {
            m_n += m_d;
            return *this;
        }
        constexpr small_rational& operator--() {
            m_n -= m_d;
            return *this;
        }
        constexpr small_rational operator++(int) {
            auto copy = *this;
            operator++();
            return copy;
        }
        constexpr small_rational operator--(int) {
            auto copy = *this;
            operator--();
            return copy;
        }

        constexpr small_rational operator*(const small_rational& rhs) const {
            auto n = m_n * rhs.m_n;
            auto d = m_d * rhs.m_d;
            return { n,d };
        }
        constexpr small_rational operator/(const small_rational& rhs) const {
            auto n = m_n * rhs.m_d;
            auto d = m_d * rhs.m_n;
            return { n,d };
        }
        constexpr small_rational operator%(const small_rational& rhs) const {
            auto ln = m_n;
            auto ld = m_d;
            auto rn = rhs.m_n;
            auto rd = rhs.m_d;
            auto a = (ln * rd) / (ld * rn);
            auto n = rn * a;
            auto d = rd * ld;
            n = ln * rd - n * ld;
            return { n,d };
        }

        constexpr auto numerator() const { return m_n; }
        constexpr auto denominator() const { return m_d; }

        constexpr auto sign() const {
            if (m_n < 0) return -1;
            else if (m_n > 0) return 1;
            else return 0;
        }

        static constexpr small_rational inverse(const small_rational& value) { return { value.m_d, value.m_n }; }

        static constexpr small_rational trunc(const small_rational& value) { return { (int64_t)value }; }
        static constexpr small_rational floor(const small_rational& value) {
            auto v = _abs(value.m_n) / value.m_d;
            auto s = value.m_n < 0;
            if (s) ++v;
            return s ? -v : v;
        }
        static constexpr small_rational ceil(const small_rational& value) {
            auto v = _abs(value.m_n) / value.m_d;
            auto s = value.m_n < 0;
            if (!s) ++v;
            return s ? -v : v;
        }

        static constexpr small_rational round(const small_rational& value) {
            return floor(value + half);
        }

        static constexpr small_rational max(const small_rational& value1, const small_rational& value2) {
            return value1 > value2 ? value1 : value2;
        }
        static constexpr small_rational min(const small_rational& value1, const small_rational& value2) {
            return value1 < value2 ? value1 : value2;
        }

        static constexpr small_rational pow(const small_rational& value, int32_t exp) {
            if (exp == 1) return value;
            else if (exp < 0) return pow(inverse(value), -exp);
            else if (exp == 0) return one;
            else {
                auto n = _pow(value.m_n, exp);
                auto d = _pow(value.m_d, exp);
                return { n,d };
            }
        }
    };

    inline constexpr small_rational small_rational::zero{ };
    inline constexpr small_rational small_rational::one{ 1 };
    inline constexpr small_rational small_rational::half{ 1,2 };
    inline constexpr small_rational small_rational::minus_one{ -1 };
    inline constexpr small_rational small_rational::minus_half{ -1,2 };
}

std::ostream& operator<<(std::ostream& out, const calendrical::small_rational& sr);