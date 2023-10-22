#pragma once

#include <cstdint>
#include <array>
#include <iostream>
#include <string>
#include <sstream>

namespace calendrical {
    template<std::size_t N>
    class polynomial {
        std::array<double, N> m_c;

    public:
        constexpr polynomial(std::initializer_list<double> c) : m_c{ 0 } {
            auto end = c.end();
            if (std::distance(c.begin(), c.end()) > N) {
                end = std::next(c.begin(), N);
            }
            std::copy(c.begin(), end, m_c.begin());
        }
        constexpr polynomial(const std::array<double, N>& c) : m_c{ c } {}

        constexpr double evaluate(double x) const {
            double result = 0;
            double p = 1;
            for (auto& c : m_c) {
                result += p * c;
                p *= x;
            }
            return result;
        }

        std::string to_string() {
            std::ostringstream oss{};
            oss << "polynomial<" << N << ">{";
            if constexpr (N > 0) {
                oss << m_c[0];
                for (auto i = 1; i < N; ++i)
                    oss << "," << m_c[i];
            }
            oss << "}";
            return oss.str();
        }

        constexpr double operator()(double x) const { return evaluate(x); }
        constexpr bool operator==(const polynomial<N>& other) const { return m_c == other.m_c; }
    };

    template<typename ...Args>
    static constexpr polynomial<sizeof...(Args)> make_polynomial(Args&&... args) {
        return { static_cast<double>(args)... };
    }
}

template<std::size_t N>
std::ostream& operator<<(std::ostream& out, const calendrical::polynomial<N>& poly) {
    return out << poly.to_string();
}