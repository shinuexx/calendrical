#pragma once

#include <cmath>
#include <array>

namespace calendrical{
   template<std::size_t N>
   class polynomial{
      std::array<double, N> m_coefficients;

   public:

      // ReSharper disable once CppNonExplicitConvertingConstructor
      constexpr polynomial(const std::array<double, N>& coefficients) : m_coefficients(coefficients){} // NOLINT(*-explicit-constructor)

      [[nodiscard]] constexpr double evaluate(const double& x) const{
         double result = 0;
         for(auto& c : std::views::reverse(m_coefficients)){
            result = x * result + c;
         }
         return result;
      }

      constexpr double operator()(const double& x) const { return evaluate(x); }
      constexpr bool operator==(const polynomial& other) const { return m_coefficients == other.m_coefficients; }
   };

   template<typename ... Args>
   static constexpr polynomial<sizeof...(Args)> make_polynomial(Args&&... coefficients){
      return std::array<double, sizeof...(Args)>{{static_cast<double>(std::forward<Args>(coefficients))...}};
   }
}