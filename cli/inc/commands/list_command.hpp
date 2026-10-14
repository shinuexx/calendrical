#pragma once

#include <string_view>
#include <vector>

#include "command.hpp"
#include "converter_provider.hpp"
#include "holiday_provider_provider.hpp"

namespace calendrical::cli {
    /** @brief Topics accepted by `list`. */
    enum class list_topic {
        /** @brief Available subcommands. */
        commands,
        /** @brief Registered date converters. */
        calendars,
        /** @brief Registered holiday providers. */
        holiday_calendars
    };

    /** @brief `list [topic]`: prints available commands, converters or holiday providers. */
    class list_command final : public command {
        /** @brief Source of named converters (not owned). */
        const converter_provider* m_converter_provider;
        /** @brief Source of named holiday providers (not owned). */
        const holiday_provider_provider* m_holiday_provider_provider;

    public:
        /** @brief Creates the command over both providers (not owned, must outlive the command). */
        list_command(const converter_provider* converter_provider, const holiday_provider_provider* holiday_provider_provider);

        /** @brief Prints usage of this subcommand to standard output. */
        void print_help() const override;
        /** @copydoc command::execute */
        [[nodiscard]] int execute(const std::vector<std::string_view>& args) const override;
    };
}
