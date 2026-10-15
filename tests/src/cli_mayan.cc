#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include "calendar_traits.hpp"
#include "calendars/mayan_calendar.hpp"
#include "commands/convert_command.hpp"
#include "converter_provider_builder.hpp"

namespace tests {
    using namespace calendrical;
    using datetypes::mayan_long_count;
    using namespace calendar_traits;

    TEST(cli_mayan, compact_five_and_nine_field_parsers) {
        const mayan_long_count reference{0, 0, 0, 0, 13, 0, 0, 0, 0};
        EXPECT_EQ(datetype_from_compact_string<mayan_long_count>("13-0-0-0-0"), reference);
        EXPECT_EQ(datetype_from_compact_string<mayan_long_count>("0-0-0-0-13-0-0-0-0"), reference);
        EXPECT_EQ(datetype_from_compact_string<mayan_long_count>(" -1 - 19 - 19 - 19 - 19 - 19 - 19 - 17 - 19 "),
                  (mayan_long_count{-1, 19, 19, 19, 19, 19, 19, 17, 19}));
    }

    TEST(cli_mayan, structured_parser_round_trip_and_field_checks) {
        const mayan_long_count date{-1, 19, 19, 19, 19, 19, 19, 17, 19};
        EXPECT_EQ(datetype_from_string<mayan_long_count>(datetype_to_string(date)), date);
        EXPECT_THROW(datetype_from_string<mayan_long_count>("mayan_long_count{alautun:0}"), std::runtime_error);
        EXPECT_THROW(datetype_from_string<mayan_long_count>(date.to_string().insert(16, "unknown:0,")), std::runtime_error);
        EXPECT_THROW(datetype_from_string<mayan_long_count>(date.to_string().insert(16, "alautun:0,")), std::runtime_error);
        auto invalid_number = date.to_string();
        invalid_number.replace(invalid_number.find("alautun:-1"), 10, "alautun:2147483648");
        EXPECT_THROW(datetype_from_string<mayan_long_count>(invalid_number), std::runtime_error);
    }

    TEST(cli_mayan, malformed_compact_values_are_rejected) {
        for (const std::string input : {"", "1-2-3-4", "0-0-0-0-0-0", "0-0-0-0-0-0-0-0",
                                       "0-0-0-0-0-0-0-0-0-0", "13.0.0.0.0", "13-0-x-0-0",
                                       "13-0-0-0-1tail", "2147483648-0-0-0-0-0-0-0-0"}) {
            EXPECT_THROW(datetype_from_compact_string<mayan_long_count>(input), std::runtime_error) << input;
        }
    }

    TEST(cli_mayan, registered_converters_validate_fields_and_preserve_full_rd_precision) {
        const auto provider = cli::build_converter_provider();
        for (const std::string name : {"mayan", "mayan_long_count"}) {
            const auto* converter = provider[name];
            ASSERT_NE(converter, nullptr);
            EXPECT_EQ(converter->to_rd("13-0-0-0-0"), 734858_rd);
            EXPECT_EQ(converter->to_rd("-1-19-19-19-19-19-19-17-19"), -1137143_rd);
            EXPECT_NE(converter->format().find("ALAUTUN"), std::string::npos);
            for (const std::string input : {"20-0-0-0-0", "13-0-0-18-0", "13-0-0-0--1",
                                           "-1-0-0-0-0", "400319966-0-0-0-0-0-0-0-0"}) {
                EXPECT_THROW((void)converter->to_rd(input), std::invalid_argument) << input;
            }
            const auto last = converter->from_rd(calendars::mayan_calendar::max_date);
            const auto parsed = datetype_from_string<mayan_long_count>(last);
            EXPECT_EQ(calendars::mayan_calendar{}.to_fixed(parsed), calendars::mayan_calendar::max_date);
            EXPECT_THROW((void)converter->from_rd(fixed_date_t{std::numeric_limits<int64_t>::max()}), std::out_of_range);
        }
    }

    TEST(cli_mayan, convert_command_both_directions_and_help) {
        const auto provider = cli::build_converter_provider();
        const cli::convert_command command{&provider};
        testing::internal::CaptureStdout();
        EXPECT_EQ(command.execute({"mayan", "gregorian", "13-0-0-0-0"}), 0);
        const auto gregorian = testing::internal::GetCapturedStdout();
        EXPECT_NE(gregorian.find("rd=734858"), std::string::npos);
        EXPECT_NE(gregorian.find("year:2012,month:december,day:21"), std::string::npos);
        testing::internal::CaptureStdout();
        EXPECT_EQ(command.execute({"gregorian", "mayan_long_count", "2012-12-21"}), 0);
        const auto mayan = testing::internal::GetCapturedStdout();
        EXPECT_NE(mayan.find("baktun:13,katun:0,tun:0,uinal:0,kin:0"), std::string::npos);
        testing::internal::CaptureStdout();
        command.print_help();
        const auto help = testing::internal::GetCapturedStdout();
        EXPECT_NE(help.find("mayan:"), std::string::npos);
        EXPECT_NE(help.find("mayan_long_count:"), std::string::npos);
        EXPECT_THROW((void)command.execute({"mayan", "gregorian", "13-0-0-18-0"}), std::invalid_argument);
    }
}
