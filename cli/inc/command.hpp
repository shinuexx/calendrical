#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace calendrical::cli {
    class command {
    protected:
        std::string m_name;
        std::string m_description;

        command(std::string name, std::string description)
            : m_name(std::move(name)), m_description(std::move(description)) {}

    public:
        virtual ~command() = default;

        [[nodiscard]] const std::string& name() const noexcept {
            return m_name;
        }
        [[nodiscard]] const std::string& description() const noexcept {
            return m_description;
        }

        [[nodiscard]] virtual int execute(const std::vector<std::string_view>& args) const = 0;
    };
}
