#include "commands/command_dispatcher.hpp"

#include <iostream>
#include <stdexcept>

namespace calendrical::cli::commands {
    std::vector<std::string_view> command_dispatcher::pack_args(int argc, const char* const argv[], int from) {
        if (argc <= from) {
            return {};
        }

        const auto size = static_cast<std::size_t>(argc - from);
        std::vector<std::string_view> result(size);
        for (std::size_t i = 0; i < size; ++i) {
            result[i] = argv[from + static_cast<int>(i)];
        }
        return result;
    }

    void command_dispatcher::print_main_help() const {
        std::cout << "Usage:\n";
        std::cout << "  calendrical_cli <command> [args]\n\n";
        std::cout << "Commands:\n";
        for (const auto& [name, cmd] : m_commands) {
            std::cout << "  " << name << "   " << cmd->description() << '\n';
        }
        std::cout << "\nExamples:\n";
        std::cout << "  calendrical_cli convert gregorian julian 2026-02-18\n";
        std::cout << "  calendrical_cli holidays --as-gregorian gregorian 2026\n";
        std::cout << "  calendrical_cli list calendars\n";
    }

    command_dispatcher& command_dispatcher::add(std::unique_ptr<command> cmd) {
        if (!cmd) {
            throw std::invalid_argument("Cannot register null command");
        }

        if (!m_commands.contains(cmd->name())) {
            m_commands.emplace(cmd->name(), std::move(cmd));
        }
        return *this;
    }

    const command* command_dispatcher::operator[](std::string_view name) const {
        if (const auto it = m_commands.find(name); it != m_commands.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    int command_dispatcher::dispatch(int argc, const char* const argv[]) const {
        if (argc < 2) {
            print_main_help();
            return 0;
        }

        const std::string_view subcommand = argv[1];
        if (subcommand == "help" || subcommand == "--help" || subcommand == "-h") {
            if (subcommand == "help" && argc > 2) {
                const std::string_view target_cmd = argv[2];
                if (const auto* cmd = (*this)[target_cmd]; cmd) {
                    cmd->print_help();
                    return 0;
                }
                throw std::runtime_error("Unknown command for help: " + std::string(target_cmd));
            }
            print_main_help();
            return 0;
        }

        const auto* cmd = (*this)[subcommand];
        if (!cmd) {
            throw std::runtime_error("Unknown command: " + std::string(subcommand));
        }

        return cmd->execute(pack_args(argc, argv, 2));
    }
}
