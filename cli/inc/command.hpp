#pragma once

#include <algorithm>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace calendrical::cli {
    /**
     * @brief Base class for CLI subcommands (`convert`, `holidays`, `list`, ...).
     *
     * Registered in a @ref commands::command_dispatcher under @ref name().
     */
    class command {
    protected:
        /** @brief Subcommand name used on the command line. */
        std::string m_name;
        /** @brief One-line description shown in the main help. */
        std::string m_description;

        /** @brief Initializes the subcommand name and description. */
        command(std::string name, std::string description)
            : m_name(std::move(name)), m_description(std::move(description)) {}

    public:
        virtual ~command() = default;

        /** @brief Subcommand name used on the command line. */
        [[nodiscard]] const std::string& name() const noexcept {
            return m_name;
        }
        /** @brief One-line description shown in the main help. */
        [[nodiscard]] const std::string& description() const noexcept {
            return m_description;
        }

        /** @brief Prints usage of this subcommand to standard output. */
        virtual void print_help() const = 0;
        /**
         * @brief Runs the subcommand.
         * @param args Arguments following the subcommand name.
         * @return Process exit code (0 on success).
         * @throws std::exception subclasses on invalid input; the caller reports them.
         */
        [[nodiscard]] virtual int execute(const std::vector<std::string_view>& args) const = 0;
    };

    /** @brief Returns whether @p args contain `-h` or `--help`. */
    [[nodiscard]] inline bool has_help_flag(const std::vector<std::string_view>& args) noexcept {
        return std::ranges::any_of(args, [](std::string_view arg) {
            return arg == "--help" || arg == "-h";
        });
    }
}
