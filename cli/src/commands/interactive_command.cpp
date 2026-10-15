#include "commands/interactive_command.hpp"

#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "commands/command_dispatcher.hpp"

namespace calendrical::cli {

    interactive_command::interactive_command(
        const commands::command_dispatcher* dispatcher,
        std::istream& in,
        std::ostream& out,
        std::ostream& err)
        : command("interactive", "Interactive date conversion and holidays session"),
          m_dispatcher(dispatcher),
          m_in(&in),
          m_out(&out),
          m_err(&err) {}

    void interactive_command::print_help() const {
        std::cout << "Usage:\n";
        std::cout << "  calendrical_cli interactive\n";
        std::cout << "  calendrical_cli -i\n";
        std::cout << "  calendrical_cli --interactive\n\n";
        std::cout << "Starts an interactive REPL session for date conversions, holiday queries,\n";
        std::cout << "and calendar listings without restarting the application between calls.\n\n";
        std::cout << "Supported commands inside the session:\n";
        std::cout << "  convert <from> <to> <date>                 Convert dates between calendar systems\n";
        std::cout << "  holidays [--as-gregorian] <provider> <year> List holidays for a calendar\n";
        std::cout << "  list [commands|calendars|holidays]         List available commands or calendars\n";
        std::cout << "  help [command]                             Show help for all or specific command\n";
        std::cout << "  exit | quit                                End interactive session\n";
    }

    int interactive_command::execute(const std::vector<std::string_view>& args) const {
        if (has_help_flag(args)) {
            print_help();
            return 0;
        }

        if (!args.empty()) {
            throw std::runtime_error("interactive does not accept positional arguments");
        }

        if (!m_dispatcher) {
            throw std::runtime_error("Dispatcher for interactive command is not initialized");
        }

        return run_session(*m_in, *m_out, *m_err);
    }

    std::vector<std::string> interactive_command::tokenize(std::string_view line) {
        std::vector<std::string> tokens;
        std::string current;
        bool in_quotes = false;
        char quote_char = '\0';
        bool in_token = false;

        for (std::size_t i = 0; i < line.size(); ++i) {
            const char c = line[i];

            if (in_quotes) {
                if (c == '\\' && i + 1 < line.size() && (line[i + 1] == quote_char || line[i + 1] == '\\')) {
                    current += line[++i];
                } else if (c == quote_char) {
                    in_quotes = false;
                    quote_char = '\0';
                } else {
                    current += c;
                }
            } else {
                if (c == '\\' && i + 1 < line.size() &&
                    (line[i + 1] == '"' || line[i + 1] == '\'' || line[i + 1] == '\\' ||
                     std::isspace(static_cast<unsigned char>(line[i + 1])))) {
                    in_token = true;
                    current += line[++i];
                } else if (c == '"' || c == '\'') {
                    in_token = true;
                    in_quotes = true;
                    quote_char = c;
                } else if (std::isspace(static_cast<unsigned char>(c))) {
                    if (in_token) {
                        tokens.push_back(std::move(current));
                        current.clear();
                        in_token = false;
                    }
                } else {
                    in_token = true;
                    current += c;
                }
            }
        }

        if (in_quotes) {
            throw std::runtime_error("Unclosed quote in command line");
        }

        if (in_token) {
            tokens.push_back(std::move(current));
        }

        return tokens;
    }

    int interactive_command::run_session(std::istream& in, std::ostream& out, std::ostream& err) const {
        if (!m_dispatcher) {
            throw std::runtime_error("Dispatcher for interactive command is not initialized");
        }

        out << "Enter commands (e.g., 'convert gregorian julian 2026-02-18' or 'holidays gregorian 2026') or type 'exit' to quit:\n";
        out.flush();

        while (true) {
            out << "> ";
            out.flush();

            std::string line;
            if (!std::getline(in, line)) {
                out << '\n';
                out.flush();
                break; // EOF
            }

            std::vector<std::string> tokens;
            try {
                tokens = tokenize(line);
            } catch (const std::exception& ex) {
                err << "Error: " << ex.what() << '\n';
                err.flush();
                continue;
            }

            if (tokens.empty()) {
                continue;
            }

            const auto& cmd_name = tokens.front();
            if (cmd_name == "exit" || cmd_name == "quit") {
                break;
            }

            if (cmd_name == "interactive") {
                out << "Interactive session is already running. Type 'exit' to quit.\n";
                out.flush();
                continue;
            }

            if (cmd_name == "help" || cmd_name == "--help" || cmd_name == "-h") {
                if (tokens.size() > 1) {
                    const auto& target = tokens[1];
                    if (target == "exit" || target == "quit") {
                        out << "exit / quit: Terminates the interactive session.\n";
                    } else if (const auto* target_cmd = (*m_dispatcher)[target]; target_cmd) {
                        target_cmd->print_help();
                    } else {
                        err << "Unknown command for help: " << target << '\n';
                        err.flush();
                    }
                } else {
                    out << "Commands:\n";
                    out << "  convert   <from> <to> <date>\n";
                    out << "  holidays  [--as-gregorian] <provider> <year>\n";
                    out << "  list      [commands|calendars|holidays]\n";
                    out << "  help      [command]\n";
                    out << "  exit      (or quit)\n";
                }
                out.flush();
                continue;
            }

            const auto* cmd = (*m_dispatcher)[cmd_name];
            if (!cmd) {
                err << "Unknown command: " << cmd_name << ". Type 'help' for available commands.\n";
                err.flush();
                continue;
            }

            std::vector<std::string_view> cmd_args;
            cmd_args.reserve(tokens.size() - 1);
            for (std::size_t i = 1; i < tokens.size(); ++i) {
                cmd_args.emplace_back(tokens[i]);
            }

            try {
                if (const int rc = cmd->execute(cmd_args); rc != 0) {
                    err << "Command exited with code " << rc << '\n';
                    err.flush();
                }
            } catch (const std::exception& ex) {
                err << "Error: " << ex.what() << '\n';
                err.flush();
            }
            out.flush();
        }

        return 0;
    }

} // namespace calendrical::cli
