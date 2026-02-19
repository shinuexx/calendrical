#include "commands/convert_command.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace calendrical::cli {
    convert_command::convert_command(const converter_provider* converter_provider)
        : command("convert", "Convert date between calendars"),
          m_converter_provider(converter_provider) {}

    int convert_command::execute(const std::vector<std::string_view>& args) const {
        if (!m_converter_provider) {
            throw std::runtime_error("Converter provider is not initialized");
        }

        if (args.size() == 1 && (args[0] == "--help" || args[0] == "-h")) {
            std::cout << "Usage:\n";
            std::cout << "  calendrical_cli convert <from> <to> <date>\n\n";
            std::cout << "Options:\n";
            std::cout << "  -h, --help  Show help\n\n";
            std::cout << "Arguments:\n";
            std::cout << "  from      Source converter\n";
            std::cout << "  to        Target converter\n";
            std::cout << "  date      Date in source converter format\n\n";
            std::cout << "Supported converters:\n";
            for (const auto& [name, converter] : m_converter_provider->converters()) {
                std::cout << "  " << name << ": " << converter->format() << '\n';
            }
            return 0;
        }

        if (args.size() != 3) {
            throw std::runtime_error("convert expects exactly 3 positional arguments: <from> <to> <date>");
        }

        const auto from_name = std::string(args[0]);
        const auto to_name = std::string(args[1]);
        const auto date = std::string(args[2]);

        const auto* from_converter = (*m_converter_provider)[from_name];
        if (!from_converter) {
            throw std::runtime_error("Unsupported source converter: " + from_name);
        }
        const auto* to_converter = (*m_converter_provider)[to_name];
        if (!to_converter) {
            throw std::runtime_error("Unsupported target converter: " + to_name);
        }

        const auto rd = from_converter->to_rd(date);
        const auto output = to_converter->from_rd(rd);

        std::cout << "from=" << from_name << " to=" << to_name << '\n';
        std::cout << "rd=" << static_cast<int64_t>(rd) << '\n';
        std::cout << output << '\n';
        return 0;
    }
}
