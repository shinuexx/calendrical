#include "commands/list_command.hpp"

#include <iostream>
#include <stdexcept>

namespace calendrical::cli {
    list_command::list_command(
        const converter_provider* converter_provider,
        const holiday_provider_provider* holiday_provider_provider)
        : command("list", "List commands or supported calendars"),
          m_converter_provider(converter_provider),
          m_holiday_provider_provider(holiday_provider_provider) {}

    int list_command::execute(const std::vector<std::string_view>& args) const {
        if (!m_converter_provider || !m_holiday_provider_provider) {
            throw std::runtime_error("Providers for list command are not initialized");
        }

        if (args.size() == 1 && (args[0] == "--help" || args[0] == "-h")) {
            std::cout << "Usage:\n";
            std::cout << "  calendrical_cli list [topic]\n\n";
            std::cout << "Options:\n";
            std::cout << "  -h, --help  Show help\n\n";
            std::cout << "Arguments:\n";
            std::cout << "  topic  commands|calendars|holiday-calendars (default: commands)\n";
            return 0;
        }

        if (args.size() > 1) {
            throw std::runtime_error("list expects at most one positional argument: [topic]");
        }

        const auto topic = args.empty() ? std::string_view{"commands"} : args[0];
        if (topic == "commands") {
            std::cout << "commands:\n";
            std::cout << "  convert\n";
            std::cout << "  holidays\n";
            std::cout << "  list\n";
            return 0;
        }
        if (topic == "calendars") {
            std::cout << "calendars (convert):\n";
            for (const auto& [name, converter] : m_converter_provider->converters()) {
                std::cout << "  " << name << " (" << converter->format() << ")\n";
            }
            return 0;
        }
        if (topic == "holiday-calendars") {
            std::cout << "calendars (holidays):\n";
            for (const auto &name: m_holiday_provider_provider->holiday_providers() | std::views::keys) {
                std::cout << "  " << name << '\n';
            }
            return 0;
        }

        throw std::runtime_error("Unsupported list topic: " + std::string(topic));
    }
}
