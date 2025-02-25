#pragma once

#include <cstdint>
#include <array>
#include "printers.hpp"

namespace tests::helpers{

    template<typename InputType, typename OutputType, std::size_t N>
    decltype(auto) make_test_params_values(const std::array<InputType, N> &input, const std::array<OutputType, N> &output){
        std::array<std::tuple<InputType, OutputType>, N> params;
        for(std::size_t i = 0; i < N; ++i){
            params[i] = std::forward_as_tuple(input[i], output[i]);
        }
        return params;
    }

    template<typename InputType, typename OutputType>
    class input_output_test : public testing::TestWithParam<std::tuple<InputType, OutputType>>{};
}