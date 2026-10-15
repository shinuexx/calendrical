#include <exception>
#include <iostream>
#include <memory>

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

        return dispatcher.dispatch(argc, argv);
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        std::cerr << "Use 'calendrical_cli --help' for usage.\n";
        return 1;
    }
}
