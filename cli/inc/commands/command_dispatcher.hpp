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
    class command_dispatcher final {
        std::map<std::string, std::unique_ptr<command>, std::less<>> m_commands;

        static std::vector<std::string_view> pack_args(int argc, const char* const argv[], int from);
        void print_main_help() const;

    public:
        command_dispatcher() = default;

        command_dispatcher& add(std::unique_ptr<command> cmd);

        template<std::derived_from<command> CommandT, typename... Args>
        command_dispatcher& add(Args&&... args) {
            return add(std::make_unique<CommandT>(std::forward<Args>(args)...));
        }

        const command* operator[](std::string_view name) const;
        int dispatch(int argc, const char* const argv[]) const;

        ~command_dispatcher() = default;
    };
}
