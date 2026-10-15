#pragma once

#include <concepts>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "command.hpp"

namespace calendrical::cli::commands {
    /**
     * @brief Routes `argv` to the registered subcommand.
     *
     * Also handles `-h`, `--help`, `help`, `help <command>`, `-i`, `--interactive` and `--version`.
     */
    class command_dispatcher final {
        /** @brief Registered subcommands keyed by name. */
        std::map<std::string, std::unique_ptr<command>, std::less<>> m_commands;

        /** @brief Returns `argv[from..argc)` as string views (empty if @p from >= @p argc). */
        [[nodiscard]] static std::vector<std::string_view> pack_args(int argc, const char* const argv[], int from);
        /** @brief Prints the list of registered subcommands and usage examples. */
        void print_main_help() const;

    public:
        /** @brief Creates a dispatcher with no subcommands. */
        command_dispatcher() = default;

        /**
         * @brief Registers @p cmd under its name; a name that is already taken is ignored.
         * @throws std::invalid_argument if @p cmd is null.
         */
        command_dispatcher& add(std::unique_ptr<command> cmd);

        /** @brief Constructs a @p CommandT from @p args and registers it. */
        template<std::derived_from<command> CommandT, typename... Args>
        command_dispatcher& add(Args&&... args) {
            return add(std::make_unique<CommandT>(std::forward<Args>(args)...));
        }

        /** @brief Returns the subcommand registered as @p name, or `nullptr`. */
        [[nodiscard]] const command* operator[](std::string_view name) const;
        /**
         * @brief Runs the subcommand named by `argv[1]` with the remaining arguments.
         * @return The subcommand's exit code, or 0 after printing help.
         * @throws std::runtime_error for an unknown subcommand.
         */
        [[nodiscard]] int dispatch(int argc, const char* const argv[]) const;

        /** @brief Destroys the dispatcher and its subcommands. */
        ~command_dispatcher() = default;
    };
}
