#pragma once

#include <string_view>
#include <vector>

#include "command.hpp"
#include "converter_provider.hpp"
#include "holiday_provider_provider.hpp"

namespace calendrical::cli {
    enum class list_topic {
        commands,
        calendars,
        holiday_calendars
    };

    class list_command final : public command {
        const converter_provider* m_converter_provider;
        const holiday_provider_provider* m_holiday_provider_provider;

    public:
        list_command(const converter_provider* converter_provider, const holiday_provider_provider* holiday_provider_provider);

        void print_help() const override;
        [[nodiscard]] int execute(const std::vector<std::string_view>& args) const override;
    };
}
