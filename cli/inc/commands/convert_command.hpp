#pragma once

#include <string_view>
#include <vector>

#include "command.hpp"
#include "converter_provider.hpp"

namespace calendrical::cli {
    class convert_command final : public command {
        const converter_provider* m_converter_provider;

    public:
        explicit convert_command(const converter_provider* converter_provider);

        [[nodiscard]] int execute(const std::vector<std::string_view>& args) const override;
    };
}
