#include "commands/holidays_command.hpp"

#include <algorithm>
#include <charconv>
#include <iostream>
#include <stdexcept>
#include <string>

#include "calendars/gregorian_calendar.hpp"

namespace calendrical::cli {
    namespace {
        struct holidays_request {
            std::string provider_name;
            int32_t year{};
            bool as_gregorian{};
        };

        [[nodiscard]] int32_t parse_year(std::string_view token) {
            int32_t value{};
            if (const auto [ptr, ec] = std::from_chars(token.data(), token.data() + token.size(), value);
                ec != std::errc{} || ptr != token.data() + token.size()) {
                throw std::runtime_error("Invalid year value: " + std::string(token));
            }
            return value;
        }

        [[nodiscard]] std::string resolve_provider_name(std::string_view raw_name) {
            if (raw_name == "gregorian" || raw_name == "gregorian_calendar") return "gregorian";
            if (raw_name == "julian" || raw_name == "julian_calendar") return "julian";
            if (raw_name == "islamic" || raw_name == "islamic_based_calendar") return "islamic_based";
            if (raw_name == "hebrew" || raw_name == "hebrew_based_calendar") return "hebrew_based";
            if (raw_name == "bahai" || raw_name == "bahai_based_calendar") return "bahai_based";
            if (raw_name == "persian" || raw_name == "persian_based_calendar") return "persian_based";
            if (raw_name == "modern_hindu_lunar_calendar") return "modern_hindu_lunar";
            return std::string(raw_name);
        }

        [[nodiscard]] holidays_request parse_request(const std::vector<std::string_view>& args) {
            bool as_gregorian = false;
            std::vector<std::string_view> positional;
            positional.reserve(args.size());

            for (const auto arg : args) {
                if (arg == "--as-gregorian" || arg == "-g") {
                    as_gregorian = true;
                    continue;
                }
                if (arg.starts_with("-")) {
                    throw std::runtime_error("Unknown option: " + std::string(arg));
                }
                positional.push_back(arg);
            }

            if (positional.size() != 2) {
                throw std::runtime_error("holidays expects: [--as-gregorian] <provider> <year>");
            }

            return holidays_request{
                .provider_name = resolve_provider_name(positional[0]),
                .year = parse_year(positional[1]),
                .as_gregorian = as_gregorian
            };
        }

        void print_holidays(std::vector<holiday_t> holidays, bool as_gregorian) {
            const auto& gregorian = calendars::gregorian_calendar::standard;
            std::ranges::sort(holidays, holiday_t::date_only_less_t{});

            for (const auto &[name, date] : holidays) {
                if (as_gregorian) {
                    std::cout << gregorian.to_date(date).to_string() << " | " << name << '\n';
                } else {
                    std::cout << static_cast<int64_t>(date) << " | " << name << '\n';
                }
            }
        }
    }

    holidays_command::holidays_command(const holiday_provider_provider* holiday_provider_provider)
        : command("holidays", "List holidays for calendar/year"),
          m_holiday_provider_provider(holiday_provider_provider) {}

    void holidays_command::print_help() const {
        std::cout << "Usage:\n";
        std::cout << "  calendrical_cli holidays [--as-gregorian] <provider> <year>\n\n";
        std::cout << "Options:\n";
        std::cout << "  -h, --help          Show help\n";
        std::cout << "  -g, --as-gregorian  Print holiday date as gregorian_date instead of fixed_date\n";
    }

    int holidays_command::execute(const std::vector<std::string_view>& args) const {
        if (!m_holiday_provider_provider) {
            throw std::runtime_error("Holiday provider provider is not initialized");
        }

        if (has_help_flag(args)) {
            print_help();
            return 0;
        }

        const auto req = parse_request(args);

        const auto* provider = (*m_holiday_provider_provider)[req.provider_name];
        if (!provider) {
            throw std::runtime_error("Unsupported holiday provider: " + req.provider_name);
        }

        auto holidays = provider->in_year(req.year);
        std::cout << "provider=" << req.provider_name
                  << " year=" << req.year
                  << " output=" << (req.as_gregorian ? "gregorian_date" : "fixed_date")
                  << '\n';
        print_holidays(std::move(holidays), req.as_gregorian);
        return 0;
    }
}
