#pragma once

#include <string_view>
#include <vector>

#include "command.hpp"
#include "holiday_provider_provider.hpp"

namespace calendrical::cli {
    class holidays_command final : public command {
        const holiday_provider_provider* m_holiday_provider_provider;

    public:
        explicit holidays_command(const holiday_provider_provider* holiday_provider_provider);

        void print_help() const override;
        [[nodiscard]] int execute(const std::vector<std::string_view>& args) const override;
    };
}
