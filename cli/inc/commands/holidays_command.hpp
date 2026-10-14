#pragma once

#include <string_view>
#include <vector>

#include "command.hpp"
#include "holiday_provider_provider.hpp"

namespace calendrical::cli {
    /** @brief `holidays [--as-gregorian] <provider> <year>`: lists holidays of a Gregorian year. */
    class holidays_command final : public command {
        /** @brief Source of named holiday providers (not owned). */
        const holiday_provider_provider* m_holiday_provider_provider;

    public:
        /** @brief Creates the command over @p holiday_provider_provider (not owned, must outlive the command). */
        explicit holidays_command(const holiday_provider_provider* holiday_provider_provider);

        /** @brief Prints usage of this subcommand to standard output. */
        void print_help() const override;
        /** @copydoc command::execute */
        [[nodiscard]] int execute(const std::vector<std::string_view>& args) const override;
    };
}
