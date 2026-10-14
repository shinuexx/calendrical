#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "command.hpp"
#include "commands/command_dispatcher.hpp"
#include "commands/convert_command.hpp"
#include "commands/holidays_command.hpp"
#include "commands/list_command.hpp"
#include "converter_provider_builder.hpp"
#include "holiday_provider_provider_builder.hpp"
#include "version.hpp"

namespace tests {
    using namespace calendrical::cli;
    using namespace calendrical::cli::commands;

    class cli_test_fixture : public ::testing::Test {
    protected:
        converter_provider m_converters = build_converter_provider();
        holiday_provider_provider m_holiday_providers = build_holiday_provider_provider();
    };

    // ==========================================
    // command_dispatcher tests
    // ==========================================

    TEST_F(cli_test_fixture, dispatcher_null_command_throws) {
        command_dispatcher dispatcher;
        EXPECT_THROW(dispatcher.add(nullptr), std::invalid_argument);
    }

    TEST_F(cli_test_fixture, dispatcher_version_flag_prints_libinfo) {
        command_dispatcher dispatcher;
        const char* argv[] = {"calendrical_cli", "--version"};

        testing::internal::CaptureStdout();
        EXPECT_EQ(dispatcher.dispatch(2, argv), 0);
        const auto output = testing::internal::GetCapturedStdout();
        EXPECT_EQ(output, calendrical::libinfo());
    }

    TEST_F(cli_test_fixture, dispatcher_empty_args_shows_help) {
        command_dispatcher dispatcher;
        dispatcher.add<convert_command>(std::addressof(m_converters));

        const char* argv[] = {"calendrical_cli"};
        EXPECT_EQ(dispatcher.dispatch(1, argv), 0);
    }

    TEST_F(cli_test_fixture, dispatcher_help_flags_return_zero) {
        command_dispatcher dispatcher;
        dispatcher.add<convert_command>(std::addressof(m_converters));

        const char* argv_help[] = {"calendrical_cli", "--help"};
        EXPECT_EQ(dispatcher.dispatch(2, argv_help), 0);

        const char* argv_h[] = {"calendrical_cli", "-h"};
        EXPECT_EQ(dispatcher.dispatch(2, argv_h), 0);

        const char* argv_cmd_help[] = {"calendrical_cli", "help"};
        EXPECT_EQ(dispatcher.dispatch(2, argv_cmd_help), 0);
    }

    TEST_F(cli_test_fixture, dispatcher_help_subcommand) {
        command_dispatcher dispatcher;
        dispatcher.add<convert_command>(std::addressof(m_converters));
        dispatcher.add<holidays_command>(std::addressof(m_holiday_providers));
        dispatcher.add<list_command>(std::addressof(m_converters), std::addressof(m_holiday_providers));

        const char* argv_convert[] = {"calendrical_cli", "help", "convert"};
        EXPECT_EQ(dispatcher.dispatch(3, argv_convert), 0);

        const char* argv_holidays[] = {"calendrical_cli", "help", "holidays"};
        EXPECT_EQ(dispatcher.dispatch(3, argv_holidays), 0);

        const char* argv_list[] = {"calendrical_cli", "help", "list"};
        EXPECT_EQ(dispatcher.dispatch(3, argv_list), 0);

        const char* argv_unknown[] = {"calendrical_cli", "help", "unknown"};
        EXPECT_THROW((void)dispatcher.dispatch(3, argv_unknown), std::runtime_error);
    }

    TEST_F(cli_test_fixture, dispatcher_unknown_command_throws) {
        command_dispatcher dispatcher;
        const char* argv[] = {"calendrical_cli", "nonexistent_command"};
        EXPECT_THROW((void)dispatcher.dispatch(2, argv), std::runtime_error);
    }

    TEST_F(cli_test_fixture, dispatcher_lookup_operator) {
        command_dispatcher dispatcher;
        dispatcher.add<convert_command>(std::addressof(m_converters));

        EXPECT_NE(dispatcher["convert"], nullptr);
        EXPECT_EQ(dispatcher["nonexistent"], nullptr);
        EXPECT_EQ(dispatcher["convert"]->name(), "convert");
        EXPECT_FALSE(dispatcher["convert"]->description().empty());
    }

    // ==========================================
    // convert_command tests
    // ==========================================

    TEST_F(cli_test_fixture, convert_uninitialized_provider_throws) {
        convert_command cmd(nullptr);
        EXPECT_THROW((void)cmd.execute({"gregorian", "julian", "2026-02-18"}), std::runtime_error);
    }

    TEST_F(cli_test_fixture, convert_help_flags_return_zero) {
        convert_command cmd(std::addressof(m_converters));
        EXPECT_EQ(cmd.execute({"--help"}), 0);
        EXPECT_EQ(cmd.execute({"-h"}), 0);
        EXPECT_EQ(cmd.execute({"extra", "--help"}), 0);
    }

    TEST_F(cli_test_fixture, convert_invalid_arg_count_throws) {
        convert_command cmd(std::addressof(m_converters));
        EXPECT_THROW((void)cmd.execute({}), std::runtime_error);
        EXPECT_THROW((void)cmd.execute({"gregorian"}), std::runtime_error);
        EXPECT_THROW((void)cmd.execute({"gregorian", "julian"}), std::runtime_error);
        EXPECT_THROW((void)cmd.execute({"gregorian", "julian", "2026-02-18", "extra"}), std::runtime_error);
    }

    TEST_F(cli_test_fixture, convert_unsupported_converters_throw) {
        convert_command cmd(std::addressof(m_converters));
        EXPECT_THROW((void)cmd.execute({"unknown_src", "julian", "2026-02-18"}), std::runtime_error);
        EXPECT_THROW((void)cmd.execute({"gregorian", "unknown_dst", "2026-02-18"}), std::runtime_error);
    }

    TEST_F(cli_test_fixture, convert_invalid_date_throws) {
        convert_command cmd(std::addressof(m_converters));
        // Invalid month day (caught by is_valid_date)
        EXPECT_THROW((void)cmd.execute({"gregorian", "julian", "2026-02-31"}), std::invalid_argument);
        // Invalid month value (caught by parser)
        EXPECT_THROW((void)cmd.execute({"gregorian", "julian", "2026-13-01"}), std::runtime_error);
        // Invalid format (caught by field parser)
        EXPECT_THROW((void)cmd.execute({"gregorian", "julian", "not-a-date"}), std::runtime_error);
    }

    TEST_F(cli_test_fixture, converters_validate_dates) {
        const auto to_rd = [this](std::string_view conv, const std::string& date) {
            return m_converters[conv]->to_rd(date);
        };

        EXPECT_NO_THROW((void)to_rd("gregorian", "2024-02-29"));
        EXPECT_THROW((void)to_rd("gregorian", "2025-02-29"), std::invalid_argument);
        EXPECT_THROW((void)to_rd("gregorian", "2026-04-31"), std::invalid_argument);

        EXPECT_NO_THROW((void)to_rd("julian", "1900-02-29"));
        EXPECT_THROW((void)to_rd("julian", "0-01-01"), std::invalid_argument);
        EXPECT_THROW((void)to_rd("julian", "2023-02-29"), std::invalid_argument);

        EXPECT_NO_THROW((void)to_rd("iso", "2024-10-5"));
        EXPECT_THROW((void)to_rd("iso", "2024-10-8"), std::invalid_argument);
        EXPECT_THROW((void)to_rd("iso", "2024-10-0"), std::invalid_argument);

        EXPECT_NO_THROW((void)to_rd("coptic", "1739-13-6"));
        EXPECT_THROW((void)to_rd("coptic", "1740-13-6"), std::invalid_argument);
        EXPECT_THROW((void)to_rd("coptic", "1740-00-15"), std::invalid_argument);

        EXPECT_NO_THROW((void)to_rd("hebrew", "5784-adarii-01"));
        EXPECT_THROW((void)to_rd("hebrew", "5783-adarii-01"), std::invalid_argument);

        EXPECT_NO_THROW((void)to_rd("chinese", "78-41-1-false-1"));
        EXPECT_THROW((void)to_rd("chinese", "78-41-13-false-1"), std::invalid_argument);

        EXPECT_NO_THROW((void)to_rd("bahai", "1-10-2-19-1"));
        EXPECT_THROW((void)to_rd("bahai", "1-10-2-20-1"), std::invalid_argument);
    }

    TEST_F(cli_test_fixture, converters_accept_negative_years) {
        const auto& gregorian = *m_converters["gregorian"];
        const auto& julian = *m_converters["julian"];

        const auto rd = gregorian.to_rd("-500-03-15");
        EXPECT_LT(static_cast<int64_t>(rd), 0);
        EXPECT_NE(gregorian.from_rd(rd).find("year:-500"), std::string::npos);

        // Julian has no year 0: 1 BCE is year -1, immediately before 1 CE.
        EXPECT_EQ(static_cast<int64_t>(julian.to_rd("1-01-01")) - static_cast<int64_t>(julian.to_rd("-1-12-31")), 1);

        convert_command cmd(std::addressof(m_converters));
        EXPECT_EQ(cmd.execute({"julian", "gregorian", "-44-03-15"}), 0);
    }

    TEST_F(cli_test_fixture, convert_successful_conversions) {
        convert_command cmd(std::addressof(m_converters));

        EXPECT_EQ(cmd.execute({"gregorian", "julian", "2026-02-18"}), 0);
        EXPECT_EQ(cmd.execute({"julian", "gregorian", "2026-02-05"}), 0);
        EXPECT_EQ(cmd.execute({"gregorian", "rd", "2026-02-18"}), 0);
        EXPECT_EQ(cmd.execute({"rd", "gregorian", "739665"}), 0);
        EXPECT_EQ(cmd.execute({"gregorian", "hebrew", "2026-02-18"}), 0);
        EXPECT_EQ(cmd.execute({"gregorian", "islamic", "2026-02-18"}), 0);
        EXPECT_EQ(cmd.execute({"gregorian", "iso", "2026-02-18"}), 0);
    }

    // ==========================================
    // holidays_command tests
    // ==========================================

    TEST_F(cli_test_fixture, holidays_uninitialized_provider_throws) {
        holidays_command cmd(nullptr);
        EXPECT_THROW((void)cmd.execute({"gregorian", "2026"}), std::runtime_error);
    }

    TEST_F(cli_test_fixture, holidays_help_flags_return_zero) {
        holidays_command cmd(std::addressof(m_holiday_providers));
        EXPECT_EQ(cmd.execute({"--help"}), 0);
        EXPECT_EQ(cmd.execute({"-h"}), 0);
        EXPECT_EQ(cmd.execute({"gregorian", "--help"}), 0);
    }

    TEST_F(cli_test_fixture, holidays_invalid_arg_count_throws) {
        holidays_command cmd(std::addressof(m_holiday_providers));
        EXPECT_THROW((void)cmd.execute({}), std::runtime_error);
        EXPECT_THROW((void)cmd.execute({"gregorian"}), std::runtime_error);
        EXPECT_THROW((void)cmd.execute({"gregorian", "2026", "extra"}), std::runtime_error);
    }

    TEST_F(cli_test_fixture, holidays_invalid_year_throws) {
        holidays_command cmd(std::addressof(m_holiday_providers));
        EXPECT_THROW((void)cmd.execute({"gregorian", "not_a_year"}), std::runtime_error);
        EXPECT_THROW((void)cmd.execute({"gregorian", "2026abc"}), std::runtime_error);
        EXPECT_THROW((void)cmd.execute({"gregorian", ""}), std::runtime_error);
    }

    TEST_F(cli_test_fixture, holidays_unknown_option_throws) {
        holidays_command cmd(std::addressof(m_holiday_providers));
        EXPECT_THROW((void)cmd.execute({"--unknown_flag", "gregorian", "2026"}), std::runtime_error);
        EXPECT_THROW((void)cmd.execute({"-x", "gregorian", "2026"}), std::runtime_error);
    }

    TEST_F(cli_test_fixture, holidays_negative_year_is_not_an_option) {
        holidays_command cmd(std::addressof(m_holiday_providers));
        EXPECT_EQ(cmd.execute({"gregorian", "-500"}), 0);
        EXPECT_EQ(cmd.execute({"-g", "gregorian", "-1"}), 0);
    }

    TEST_F(cli_test_fixture, holidays_unsupported_provider_throws) {
        holidays_command cmd(std::addressof(m_holiday_providers));
        EXPECT_THROW((void)cmd.execute({"unsupported_provider", "2026"}), std::runtime_error);
    }

    TEST_F(cli_test_fixture, holidays_same_day_are_printed_by_name) {
        struct same_day_provider final : calendrical::holiday_provider_if {
            [[nodiscard]] const calendrical::calendar_if* calendar() const override { return nullptr; }
            [[nodiscard]] std::vector<calendrical::holiday_t> in_year(int32_t) const override {
                using calendrical::fixed_date_t;
                return {{"zeta", fixed_date_t{10}}, {"beta", fixed_date_t{5}}, {"alpha", fixed_date_t{10}}};
            }
        };
        holiday_provider_provider providers;
        providers.add<same_day_provider>("same_day");
        holidays_command cmd(std::addressof(providers));

        testing::internal::CaptureStdout();
        EXPECT_EQ(cmd.execute({"same_day", "2026"}), 0);
        const auto output = testing::internal::GetCapturedStdout();

        const auto beta = output.find("beta");
        const auto alpha = output.find("alpha");
        const auto zeta = output.find("zeta");
        ASSERT_NE(beta, std::string::npos);
        ASSERT_NE(alpha, std::string::npos);
        ASSERT_NE(zeta, std::string::npos);
        EXPECT_LT(beta, alpha);
        EXPECT_LT(alpha, zeta);
    }

    TEST_F(cli_test_fixture, holidays_successful_executions) {
        holidays_command cmd(std::addressof(m_holiday_providers));

        // Default fixed_date output
        EXPECT_EQ(cmd.execute({"gregorian", "2026"}), 0);
        // Gregorian date output
        EXPECT_EQ(cmd.execute({"--as-gregorian", "gregorian", "2026"}), 0);
        EXPECT_EQ(cmd.execute({"-g", "gregorian", "2026"}), 0);

        // Providers and alias names
        EXPECT_EQ(cmd.execute({"julian", "2026"}), 0);
        EXPECT_EQ(cmd.execute({"julian_calendar", "2026"}), 0);
        EXPECT_EQ(cmd.execute({"chinese", "2026"}), 0);
        EXPECT_EQ(cmd.execute({"chinese_calendar", "2026"}), 0);
        EXPECT_EQ(cmd.execute({"korean", "2026"}), 0);
        EXPECT_EQ(cmd.execute({"korean_calendar", "2026"}), 0);
        EXPECT_EQ(cmd.execute({"vietnamese", "2026"}), 0);
        EXPECT_EQ(cmd.execute({"vietnamese_calendar", "2026"}), 0);
        EXPECT_EQ(cmd.execute({"hebrew", "5786"}), 0);
        EXPECT_EQ(cmd.execute({"hebrew_based_calendar", "5786"}), 0);
        EXPECT_EQ(cmd.execute({"islamic", "1447"}), 0);
        EXPECT_EQ(cmd.execute({"islamic_based_calendar", "1447"}), 0);
        EXPECT_EQ(cmd.execute({"persian", "1404"}), 0);
        EXPECT_EQ(cmd.execute({"bahai", "182"}), 0);
        EXPECT_EQ(cmd.execute({"modern_hindu_lunar_calendar", "2082"}), 0);
        EXPECT_EQ(cmd.execute({"us", "2026"}), 0);
        EXPECT_EQ(cmd.execute({"usa", "2026"}), 0);
        EXPECT_EQ(cmd.execute({"us_calendar", "2026"}), 0);
        EXPECT_EQ(cmd.execute({"--as-gregorian", "us", "2026"}), 0);
    }

    TEST_F(cli_test_fixture, holidays_us_command) {
        holidays_command cmd(std::addressof(m_holiday_providers));

        testing::internal::CaptureStdout();
        EXPECT_EQ(cmd.execute({"--as-gregorian", "us", "2026"}), 0);
        const auto output = testing::internal::GetCapturedStdout();

        EXPECT_NE(output.find("provider=us year=2026"), std::string::npos);
        EXPECT_NE(output.find("independence day"), std::string::npos);
        EXPECT_NE(output.find("labor day"), std::string::npos);
        EXPECT_NE(output.find("memorial day"), std::string::npos);
        EXPECT_NE(output.find("election day"), std::string::npos);
        EXPECT_NE(output.find("daylight saving start"), std::string::npos);
        EXPECT_NE(output.find("daylight saving end"), std::string::npos);
    }

    // ==========================================
    // list_command tests
    // ==========================================

    TEST_F(cli_test_fixture, list_uninitialized_providers_throw) {
        list_command cmd1(nullptr, std::addressof(m_holiday_providers));
        EXPECT_THROW((void)cmd1.execute({}), std::runtime_error);

        list_command cmd2(std::addressof(m_converters), nullptr);
        EXPECT_THROW((void)cmd2.execute({}), std::runtime_error);
    }

    TEST_F(cli_test_fixture, list_help_flags_return_zero) {
        list_command cmd(std::addressof(m_converters), std::addressof(m_holiday_providers));
        EXPECT_EQ(cmd.execute({"--help"}), 0);
        EXPECT_EQ(cmd.execute({"-h"}), 0);
    }

    TEST_F(cli_test_fixture, list_too_many_arguments_throws) {
        list_command cmd(std::addressof(m_converters), std::addressof(m_holiday_providers));
        EXPECT_THROW((void)cmd.execute({"commands", "extra_topic"}), std::runtime_error);
    }

    TEST_F(cli_test_fixture, list_unsupported_topic_throws) {
        list_command cmd(std::addressof(m_converters), std::addressof(m_holiday_providers));
        EXPECT_THROW((void)cmd.execute({"unsupported_topic"}), std::runtime_error);
    }

    TEST_F(cli_test_fixture, list_supported_topics) {
        list_command cmd(std::addressof(m_converters), std::addressof(m_holiday_providers));

        EXPECT_EQ(cmd.execute({}), 0); // default: commands
        EXPECT_EQ(cmd.execute({"commands"}), 0);
        EXPECT_EQ(cmd.execute({"calendars"}), 0);
        EXPECT_EQ(cmd.execute({"convert"}), 0);
        EXPECT_EQ(cmd.execute({"holiday-calendars"}), 0);
        EXPECT_EQ(cmd.execute({"holidays"}), 0);
    }

    // ==========================================
    // Full dispatcher end-to-end execution tests
    // ==========================================

    TEST_F(cli_test_fixture, full_dispatcher_e2e_dispatch) {
        command_dispatcher dispatcher;
        dispatcher.add<convert_command>(std::addressof(m_converters));
        dispatcher.add<holidays_command>(std::addressof(m_holiday_providers));
        dispatcher.add<list_command>(std::addressof(m_converters), std::addressof(m_holiday_providers));

        const char* convert_argv[] = {"calendrical_cli", "convert", "gregorian", "julian", "2026-02-18"};
        EXPECT_EQ(dispatcher.dispatch(5, convert_argv), 0);

        const char* holidays_argv[] = {"calendrical_cli", "holidays", "--as-gregorian", "gregorian", "2026"};
        EXPECT_EQ(dispatcher.dispatch(5, holidays_argv), 0);

        const char* list_argv[] = {"calendrical_cli", "list", "calendars"};
        EXPECT_EQ(dispatcher.dispatch(3, list_argv), 0);
    }
}
