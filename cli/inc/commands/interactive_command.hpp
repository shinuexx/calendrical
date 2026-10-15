#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "command.hpp"

namespace calendrical::cli::commands {
    class command_dispatcher;
}

namespace calendrical::cli {
    /**
     * @brief `interactive` / `-i` / `--interactive`: starts an interactive REPL session.
     *
     * Evaluates commands (`convert`, `holidays`, `list`, `help`, ...) through
     * the registered @ref commands::command_dispatcher until `exit`, `quit` or EOF.
     */
    class interactive_command final : public command {
        /** @brief Dispatcher used to route interactive commands (not owned). */
        const commands::command_dispatcher* m_dispatcher;
        /** @brief Default input stream for the interactive session. */
        std::istream* m_in;
        /** @brief Default standard output stream for the interactive session. */
        std::ostream* m_out;
        /** @brief Default standard error stream for the interactive session. */
        std::ostream* m_err;

    public:
        /**
         * @brief Creates the interactive command bound to a dispatcher and optional I/O streams.
         * @param dispatcher Dispatcher routing subcommands (must outlive this command).
         * @param in Input stream (defaults to std::cin).
         * @param out Output stream for prompts and command outputs (defaults to std::cout).
         * @param err Output stream for error messages (defaults to std::cerr).
         */
        explicit interactive_command(
            const commands::command_dispatcher* dispatcher,
            std::istream& in = std::cin,
            std::ostream& out = std::cout,
            std::ostream& err = std::cerr);

        ~interactive_command() override = default;

        /** @brief Prints usage of this subcommand to standard output. */
        void print_help() const override;

        /**
         * @brief Runs the interactive command.
         *
         * If `--help` or `-h` is passed, prints usage without starting a session.
         * Otherwise, begins the REPL session using the configured streams.
         *
         * @param args Command-line arguments passed after `interactive`.
         * @return 0 on successful session completion.
         * @throws std::runtime_error if uninitialized or unexpected positional arguments are passed.
         */
        [[nodiscard]] int execute(const std::vector<std::string_view>& args) const override;

        /**
         * @brief Runs an interactive REPL session on explicit streams.
         *
         * Useful for deterministic testing and embedding.
         *
         * @param in Stream to read commands from.
         * @param out Stream for prompts and normal output.
         * @param err Stream for error reports.
         * @return 0 when the session exits normally.
         */
        [[nodiscard]] int run_session(std::istream& in, std::ostream& out, std::ostream& err) const;

        /**
         * @brief Splits an input line into command-line tokens, respecting single and double quotes.
         *
         * Supports:
         * - Bare words separated by whitespace.
         * - Double-quoted strings (`"..."`) and single-quoted strings (`'...'`).
         * - Escaped quotes and backslashes via `\`.
         *
         * @param line Raw input string.
         * @return Vector of parsed string tokens.
         * @throws std::runtime_error on an unclosed quote.
         */
        [[nodiscard]] static std::vector<std::string> tokenize(std::string_view line);
    };
}
