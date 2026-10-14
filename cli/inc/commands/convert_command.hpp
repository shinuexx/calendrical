#pragma once

#include <string_view>
#include <vector>

#include "command.hpp"
#include "converter_provider.hpp"

namespace calendrical::cli {
    /** @brief `convert <from> <to> <date>`: converts a date between two registered converters. */
    class convert_command final : public command {
        /** @brief Source of named converters (not owned). */
        const converter_provider* m_converter_provider;

    public:
        /** @brief Creates the command over @p converter_provider (not owned, must outlive the command). */
        explicit convert_command(const converter_provider* converter_provider);

        /** @brief Prints usage of this subcommand to standard output. */
        void print_help() const override;
        /** @copydoc command::execute */
        [[nodiscard]] int execute(const std::vector<std::string_view>& args) const override;
    };
}
