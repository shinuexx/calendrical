#include <exception>
#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

#include "commands/command_dispatcher.hpp"
#include "commands/convert_command.hpp"
#include "commands/holidays_command.hpp"
#include "commands/interactive_command.hpp"
#include "commands/list_command.hpp"
#include "converter_provider_builder.hpp"
#include "holiday_provider_provider_builder.hpp"

int main(int argc, const char* const argv[]) {
    try {
        const auto cnvrtr_prvdr = calendrical::cli::build_converter_provider();
        const auto hld_prvdr_prvdr = calendrical::cli::build_holiday_provider_provider();

        calendrical::cli::commands::command_dispatcher dispatcher{};
        dispatcher.add<calendrical::cli::convert_command>(std::addressof(cnvrtr_prvdr));
        dispatcher.add<calendrical::cli::holidays_command>(std::addressof(hld_prvdr_prvdr));
        dispatcher.add<calendrical::cli::interactive_command>(std::addressof(dispatcher));
        dispatcher.add<calendrical::cli::list_command>(std::addressof(cnvrtr_prvdr), std::addressof(hld_prvdr_prvdr));

        if (argc >= 2 && (std::string_view(argv[1]) == "-i" || std::string_view(argv[1]) == "--interactive")) {
            std::vector<const char*> interactive_argv;
            interactive_argv.reserve(static_cast<std::size_t>(argc));
            interactive_argv.push_back(argv[0]);
            interactive_argv.push_back("interactive");
            for (int i = 2; i < argc; ++i) {
                interactive_argv.push_back(argv[i]);
            }
            return dispatcher.dispatch(static_cast<int>(interactive_argv.size()), interactive_argv.data());
        }

        return dispatcher.dispatch(argc, argv);
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        std::cerr << "Use 'calendrical_cli --help' for usage.\n";
        return 1;
    }
}
