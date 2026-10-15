#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>

#include "commands/command_dispatcher.hpp"
#include "commands/convert_command.hpp"
#include "commands/holidays_command.hpp"
#include "commands/interactive_command.hpp"
#include "commands/list_command.hpp"
#include "converter_provider_builder.hpp"
#include "holiday_provider_provider_builder.hpp"

namespace tests {
    using namespace calendrical::cli;
    using namespace calendrical::cli::commands;

    class cli_interactive_fixture : public ::testing::Test {
    protected:
        converter_provider m_converters = build_converter_provider();
        holiday_provider_provider m_holiday_providers = build_holiday_provider_provider();
        command_dispatcher m_dispatcher;

        void SetUp() override {
            m_dispatcher.add<convert_command>(std::addressof(m_converters));
            m_dispatcher.add<holidays_command>(std::addressof(m_holiday_providers));
            m_dispatcher.add<list_command>(std::addressof(m_converters), std::addressof(m_holiday_providers));
            m_dispatcher.add<interactive_command>(std::addressof(m_dispatcher));
        }
    };

    // ==========================================
    // Tokenizer tests
    // ==========================================

    TEST(interactive_tokenizer, empty_and_whitespace_lines) {
        EXPECT_TRUE(interactive_command::tokenize("").empty());
        EXPECT_TRUE(interactive_command::tokenize("   ").empty());
        EXPECT_TRUE(interactive_command::tokenize("\t  \r\n ").empty());
    }

    TEST(interactive_tokenizer, simple_words) {
        const auto tokens = interactive_command::tokenize("convert gregorian julian 2026-02-18");
        const std::vector<std::string> expected = {"convert", "gregorian", "julian", "2026-02-18"};
        EXPECT_EQ(tokens, expected);
    }

    TEST(interactive_tokenizer, extra_spaces_between_words) {
        const auto tokens = interactive_command::tokenize("   convert   gregorian   julian   2026-02-18   ");
        const std::vector<std::string> expected = {"convert", "gregorian", "julian", "2026-02-18"};
        EXPECT_EQ(tokens, expected);
    }

    TEST(interactive_tokenizer, double_and_single_quotes) {
        const auto tokens1 = interactive_command::tokenize("convert \"gregorian\" 'julian' \"2026-02-18\"");
        const std::vector<std::string> expected1 = {"convert", "gregorian", "julian", "2026-02-18"};
        EXPECT_EQ(tokens1, expected1);

        const auto tokens2 = interactive_command::tokenize("cmd \"word with spaces\" 'another one'");
        const std::vector<std::string> expected2 = {"cmd", "word with spaces", "another one"};
        EXPECT_EQ(tokens2, expected2);
    }

    TEST(interactive_tokenizer, empty_quotes) {
        const auto tokens = interactive_command::tokenize("cmd \"\" '' arg");
        const std::vector<std::string> expected = {"cmd", "", "", "arg"};
        EXPECT_EQ(tokens, expected);
    }

    TEST(interactive_tokenizer, escaped_quotes_and_characters) {
        const auto tokens = interactive_command::tokenize("cmd \"escaped \\\"quote\\\"\" 'single \\'quote\\''");
        const std::vector<std::string> expected = {"cmd", "escaped \"quote\"", "single 'quote'"};
        EXPECT_EQ(tokens, expected);

        const auto tokens_bs = interactive_command::tokenize("cmd \"back\\\\slash\"");
        const std::vector<std::string> expected_bs = {"cmd", "back\\slash"};
        EXPECT_EQ(tokens_bs, expected_bs);
    }

    TEST(interactive_tokenizer, unclosed_quotes_throw) {
        EXPECT_THROW((void)interactive_command::tokenize("convert \"unclosed"), std::runtime_error);
        EXPECT_THROW((void)interactive_command::tokenize("convert 'unclosed"), std::runtime_error);
        EXPECT_THROW((void)interactive_command::tokenize("\""), std::runtime_error);
        EXPECT_THROW((void)interactive_command::tokenize("'"), std::runtime_error);
    }

    // ==========================================
    // interactive_command lifecycle & help
    // ==========================================

    TEST_F(cli_interactive_fixture, uninitialized_dispatcher_throws) {
        interactive_command cmd(nullptr);
        EXPECT_THROW((void)cmd.execute({}), std::runtime_error);
    }

    TEST_F(cli_interactive_fixture, help_flags_return_zero_without_session) {
        std::istringstream in("");
        std::ostringstream out;
        std::ostringstream err;
        interactive_command cmd(std::addressof(m_dispatcher), in, out, err);

        testing::internal::CaptureStdout();
        EXPECT_EQ(cmd.execute({"--help"}), 0);
        EXPECT_EQ(cmd.execute({"-h"}), 0);
        const auto captured = testing::internal::GetCapturedStdout();
        EXPECT_NE(captured.find("Usage:"), std::string::npos);
        EXPECT_NE(captured.find("interactive"), std::string::npos);
        // Did not print prompt because session didn't start
        EXPECT_TRUE(out.str().empty());
    }

    TEST_F(cli_interactive_fixture, unexpected_positional_arguments_throw) {
        std::istringstream in("");
        std::ostringstream out;
        std::ostringstream err;
        interactive_command cmd(std::addressof(m_dispatcher), in, out, err);

        EXPECT_THROW((void)cmd.execute({"unexpected_arg"}), std::runtime_error);
        EXPECT_THROW((void)cmd.execute({"arg1", "arg2"}), std::runtime_error);
    }

    // ==========================================
    // REPL session behavior
    // ==========================================

    TEST_F(cli_interactive_fixture, empty_stream_eof_exits_cleanly) {
        std::istringstream in("");
        std::ostringstream out;
        std::ostringstream err;
        interactive_command cmd(std::addressof(m_dispatcher), in, out, err);

        EXPECT_EQ(cmd.execute({}), 0);
        EXPECT_NE(out.str().find("Enter commands"), std::string::npos);
        EXPECT_NE(out.str().find("> "), std::string::npos);
        EXPECT_FALSE(out.str().empty());
        EXPECT_EQ(out.str().back(), '\n');
        EXPECT_TRUE(err.str().empty());
    }

    TEST_F(cli_interactive_fixture, exit_and_quit_commands) {
        {
            std::istringstream in("exit\n");
            std::ostringstream out;
            std::ostringstream err;
            interactive_command cmd(std::addressof(m_dispatcher), in, out, err);
            EXPECT_EQ(cmd.execute({}), 0);
            EXPECT_TRUE(err.str().empty());
        }
        {
            std::istringstream in("quit\n");
            std::ostringstream out;
            std::ostringstream err;
            interactive_command cmd(std::addressof(m_dispatcher), in, out, err);
            EXPECT_EQ(cmd.execute({}), 0);
            EXPECT_TRUE(err.str().empty());
        }
    }

    TEST_F(cli_interactive_fixture, blank_lines_and_spaces_are_ignored) {
        std::istringstream in("\n   \n\t\nexit\n");
        std::ostringstream out;
        std::ostringstream err;
        interactive_command cmd(std::addressof(m_dispatcher), in, out, err);

        EXPECT_EQ(cmd.execute({}), 0);
        EXPECT_TRUE(err.str().empty());
    }

    TEST_F(cli_interactive_fixture, nested_interactive_prints_warning) {
        std::istringstream in("interactive\nexit\n");
        std::ostringstream out;
        std::ostringstream err;
        interactive_command cmd(std::addressof(m_dispatcher), in, out, err);

        EXPECT_EQ(cmd.execute({}), 0);
        EXPECT_NE(out.str().find("Interactive session is already running"), std::string::npos);
        EXPECT_TRUE(err.str().empty());
    }

    TEST_F(cli_interactive_fixture, help_commands_in_session) {
        std::istringstream in("help\nhelp exit\nhelp convert\nhelp unknown_cmd\nexit\n");
        std::ostringstream out;
        std::ostringstream err;
        interactive_command cmd(std::addressof(m_dispatcher), in, out, err);

        testing::internal::CaptureStdout();
        EXPECT_EQ(cmd.execute({}), 0);
        const auto captured = testing::internal::GetCapturedStdout();

        EXPECT_NE(out.str().find("Commands:"), std::string::npos);
        EXPECT_NE(out.str().find("convert"), std::string::npos);
        EXPECT_NE(out.str().find("exit / quit"), std::string::npos);
        EXPECT_NE(captured.find("Usage:"), std::string::npos);
        EXPECT_NE(err.str().find("Unknown command for help: unknown_cmd"), std::string::npos);
    }

    TEST_F(cli_interactive_fixture, unknown_command_reports_error_and_continues) {
        std::istringstream in("bad_command_xyz\nexit\n");
        std::ostringstream out;
        std::ostringstream err;
        interactive_command cmd(std::addressof(m_dispatcher), in, out, err);

        EXPECT_EQ(cmd.execute({}), 0);
        EXPECT_NE(err.str().find("Unknown command: bad_command_xyz"), std::string::npos);
    }

    namespace {
        class mock_failing_command final : public command {
        public:
            mock_failing_command() : command("failing", "Test command returning error code") {}
            void print_help() const override {}
            int execute(const std::vector<std::string_view>&) const override { return 42; }
        };
    }

    TEST_F(cli_interactive_fixture, command_runtime_error_reports_and_continues) {
        // convert with invalid arguments causes convert_command to throw,
        // session should report error to err and continue until exit
        std::istringstream in("convert gregorian julian invalid_date_format\nexit\n");
        std::ostringstream out;
        std::ostringstream err;
        interactive_command cmd(std::addressof(m_dispatcher), in, out, err);

        EXPECT_EQ(cmd.execute({}), 0);
        EXPECT_NE(err.str().find("Error:"), std::string::npos);
    }

    TEST_F(cli_interactive_fixture, command_non_zero_exit_code_reports_and_continues) {
        command_dispatcher dispatcher;
        dispatcher.add<mock_failing_command>();

        std::istringstream in("failing\nexit\n");
        std::ostringstream out;
        std::ostringstream err;
        interactive_command cmd(std::addressof(dispatcher), in, out, err);

        EXPECT_EQ(cmd.execute({}), 0);
        EXPECT_NE(err.str().find("Command exited with code 42"), std::string::npos);
    }

    TEST_F(cli_interactive_fixture, unclosed_quote_reports_error_and_continues) {
        std::istringstream in("convert \"unclosed\nexit\n");
        std::ostringstream out;
        std::ostringstream err;
        interactive_command cmd(std::addressof(m_dispatcher), in, out, err);

        EXPECT_EQ(cmd.execute({}), 0);
        EXPECT_NE(err.str().find("Error: Unclosed quote"), std::string::npos);
    }

    TEST_F(cli_interactive_fixture, multiple_successful_commands_in_session) {
        std::istringstream in(
            "convert gregorian julian 2026-02-18\n"
            "holidays --as-gregorian us 2026\n"
            "list commands\n"
            "exit\n"
        );
        std::ostringstream out;
        std::ostringstream err;
        interactive_command cmd(std::addressof(m_dispatcher), in, out, err);

        testing::internal::CaptureStdout();
        EXPECT_EQ(cmd.execute({}), 0);
        const auto captured = testing::internal::GetCapturedStdout();

        // Convert output
        EXPECT_NE(captured.find("julian_date{year:2026,month:february,day:5}"), std::string::npos);
        // Holidays output
        EXPECT_NE(captured.find("provider=us year=2026"), std::string::npos);
        EXPECT_NE(captured.find("independence day"), std::string::npos);
        // List output
        EXPECT_NE(captured.find("commands:"), std::string::npos);

        EXPECT_TRUE(err.str().empty());
    }

    TEST_F(cli_interactive_fixture, quoted_arguments_in_session) {
        std::istringstream in("convert \"gregorian\" 'julian' \"2026-02-18\"\nexit\n");
        std::ostringstream out;
        std::ostringstream err;
        interactive_command cmd(std::addressof(m_dispatcher), in, out, err);

        testing::internal::CaptureStdout();
        EXPECT_EQ(cmd.execute({}), 0);
        const auto captured = testing::internal::GetCapturedStdout();

        EXPECT_NE(captured.find("julian_date{year:2026,month:february,day:5}"), std::string::npos);
        EXPECT_TRUE(err.str().empty());
    }

    TEST_F(cli_interactive_fixture, dispatcher_interactive_command_lookup) {
        EXPECT_NE(m_dispatcher["interactive"], nullptr);
        EXPECT_EQ(m_dispatcher["interactive"]->name(), "interactive");
        EXPECT_FALSE(m_dispatcher["interactive"]->description().empty());
    }

    TEST_F(cli_interactive_fixture, dispatcher_interactive_flags_dispatch) {
        command_dispatcher dispatcher;
        std::istringstream in("exit\n");
        std::ostringstream out;
        std::ostringstream err;
        dispatcher.add(std::make_unique<interactive_command>(&dispatcher, in, out, err));

        const char* const argv_i[] = {"calendrical_cli", "-i"};
        EXPECT_EQ(dispatcher.dispatch(2, argv_i), 0);
        EXPECT_NE(out.str().find("Enter commands"), std::string::npos);

        std::istringstream in2("exit\n");
        std::ostringstream out2;
        std::ostringstream err2;
        command_dispatcher dispatcher2;
        dispatcher2.add(std::make_unique<interactive_command>(&dispatcher2, in2, out2, err2));

        const char* const argv_long[] = {"calendrical_cli", "--interactive"};
        EXPECT_EQ(dispatcher2.dispatch(2, argv_long), 0);
        EXPECT_NE(out2.str().find("Enter commands"), std::string::npos);
    }

    TEST_F(cli_interactive_fixture, dispatcher_interactive_help_flags) {
        command_dispatcher dispatcher;
        std::istringstream in("");
        std::ostringstream out;
        std::ostringstream err;
        dispatcher.add(std::make_unique<interactive_command>(&dispatcher, in, out, err));

        const char* const argv_i_help[] = {"calendrical_cli", "-i", "--help"};
        testing::internal::CaptureStdout();
        EXPECT_EQ(dispatcher.dispatch(3, argv_i_help), 0);
        const auto captured = testing::internal::GetCapturedStdout();
        EXPECT_NE(captured.find("Usage:"), std::string::npos);

        const char* const argv_long_help[] = {"calendrical_cli", "--interactive", "-h"};
        testing::internal::CaptureStdout();
        EXPECT_EQ(dispatcher.dispatch(3, argv_long_help), 0);
        const auto captured2 = testing::internal::GetCapturedStdout();
        EXPECT_NE(captured2.find("Usage:"), std::string::npos);
    }

    TEST_F(cli_interactive_fixture, dispatcher_unregistered_interactive_flag_throws) {
        command_dispatcher dispatcher;
        const char* const argv_i[] = {"calendrical_cli", "-i"};
        EXPECT_THROW((void)dispatcher.dispatch(2, argv_i), std::runtime_error);
        const char* const argv_long[] = {"calendrical_cli", "--interactive"};
        EXPECT_THROW((void)dispatcher.dispatch(2, argv_long), std::runtime_error);
    }

    TEST_F(cli_interactive_fixture, dispatcher_main_help_mentions_interactive) {
        command_dispatcher dispatcher;
        const char* const argv[] = {"calendrical_cli", "--help"};
        testing::internal::CaptureStdout();
        EXPECT_EQ(dispatcher.dispatch(2, argv), 0);
        const auto captured = testing::internal::GetCapturedStdout();
        EXPECT_NE(captured.find("-i | --interactive"), std::string::npos);
    }
}
