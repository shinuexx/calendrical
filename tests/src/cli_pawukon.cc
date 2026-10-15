#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "commands/holidays_command.hpp"
#include "commands/list_command.hpp"
#include "converter_provider_builder.hpp"
#include "holiday_provider_provider_builder.hpp"

namespace tests {
    using namespace calendrical::cli;

    class cli_pawukon_fixture : public ::testing::Test {
    protected:
        converter_provider m_converters = build_converter_provider();
        holiday_provider_provider m_holiday_providers = build_holiday_provider_provider();
    };

    TEST_F(cli_pawukon_fixture, provider_is_registered) {
        ASSERT_NE(m_holiday_providers["pawukon"], nullptr);

        list_command cmd(std::addressof(m_converters), std::addressof(m_holiday_providers));
        testing::internal::CaptureStdout();
        EXPECT_EQ(cmd.execute({"holiday-calendars"}), 0);
        const auto output = testing::internal::GetCapturedStdout();
        EXPECT_NE(output.find("pawukon"), std::string::npos);
    }

    TEST_F(cli_pawukon_fixture, holidays_as_gregorian) {
        holidays_command cmd(std::addressof(m_holiday_providers));

        testing::internal::CaptureStdout();
        EXPECT_EQ(cmd.execute({"--as-gregorian", "pawukon", "2000"}), 0);
        const auto output = testing::internal::GetCapturedStdout();

        EXPECT_NE(output.find("provider=pawukon year=2000"), std::string::npos);
        // 2000-01-15 is both the first Kajeng Keliwon and the first Tumpek of the year.
        const auto kajeng_keliwon = output.find("gregorian_date{year:2000,month:january,day:15} | kajeng keliwon");
        const auto tumpek = output.find("gregorian_date{year:2000,month:january,day:15} | tumpek");
        const auto next_kajeng_keliwon = output.find("gregorian_date{year:2000,month:january,day:30} | kajeng keliwon");
        const auto next_tumpek = output.find("gregorian_date{year:2000,month:february,day:19} | tumpek");
        ASSERT_NE(kajeng_keliwon, std::string::npos);
        ASSERT_NE(tumpek, std::string::npos);
        ASSERT_NE(next_kajeng_keliwon, std::string::npos);
        ASSERT_NE(next_tumpek, std::string::npos);
        EXPECT_LT(kajeng_keliwon, tumpek);
        EXPECT_LT(tumpek, next_kajeng_keliwon);
        EXPECT_LT(next_kajeng_keliwon, next_tumpek);
    }

    TEST_F(cli_pawukon_fixture, holidays_as_fixed_dates) {
        holidays_command cmd(std::addressof(m_holiday_providers));
        EXPECT_EQ(cmd.execute({"pawukon", "2026"}), 0);
        EXPECT_EQ(cmd.execute({"pawukon", "-1"}), 0);
    }
}
