#pragma once

#include <cmath>
#include <ranges>
#include <array>

namespace calendrical{
   /**
    * @brief Fixed-size polynomial with coefficients in ascending degree order.
    *
    * For coefficients `{c0, c1, ...}`, evaluation computes
    * `c0 + c1*x + c2*x^2 + ...`.
    */
   template<std::size_t N>
   class polynomial{
      /** @brief Coefficients in ascending order of power. */
      std::array<double, N> m_coefficients;

   public:

      // ReSharper disable once CppNonExplicitConvertingConstructor
      /** @brief Constructs a polynomial from its coefficient array. */
      constexpr polynomial(const std::array<double, N>& coefficients) : m_coefficients(coefficients){} // NOLINT(*-explicit-constructor)

      /** @brief Evaluates the polynomial at @p x using Horner's method. */
      [[nodiscard]] constexpr double evaluate(const double& x) const{
         double result = 0;
         for(auto& c : std::views::reverse(m_coefficients)){
            result = x * result + c;
         }
         return result;
      }

      /** @brief Function-call shorthand for @ref evaluate. */
      constexpr double operator()(const double& x) const { return evaluate(x); }
      /** @brief Equality comparison on the coefficient array. */
      constexpr bool operator==(const polynomial& other) const { return m_coefficients == other.m_coefficients; }
   };

   template<typename ... Args>
   /** @brief Helper factory that builds a polynomial from numeric coefficients. */
   inline constexpr polynomial<sizeof...(Args)> make_polynomial(Args&&... coefficients){
      return std::array<double, sizeof...(Args)>{{static_cast<double>(std::forward<Args>(coefficients))...}};
   }
}
