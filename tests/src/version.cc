#include <gtest/gtest.h>

#include <fstream>
#include <regex>
#include <string>

#include "version.hpp"

namespace tests {
    TEST(version, header_version_matches_version_file) {
        std::ifstream file(std::string(CALENDRICAL_DATA_DIR) + "/VERSION");
        ASSERT_TRUE(file.is_open());
        std::string expected;
        std::getline(file, expected);
        EXPECT_EQ(calendrical::version(), expected);
    }

    TEST(version, components_match_version_string) {
        static_assert(calendrical::version_major() == CALENDRICAL_VERSION_MAJOR);
        static_assert(calendrical::version_minor() == CALENDRICAL_VERSION_MINOR);
        static_assert(calendrical::version_patch() == CALENDRICAL_VERSION_PATCH);
        static_assert(CALENDRICAL_VERSION_NUMBER ==
            CALENDRICAL_VERSION_MAJOR * 10000 + CALENDRICAL_VERSION_MINOR * 100 + CALENDRICAL_VERSION_PATCH);

        const auto expected = std::to_string(calendrical::version_major()) + "." +
            std::to_string(calendrical::version_minor()) + "." + std::to_string(calendrical::version_patch());
        EXPECT_EQ(calendrical::version(), expected);
    }

    TEST(version, linked_library_matches_headers) {
        EXPECT_EQ(calendrical::library_version(), calendrical::version());
    }

    TEST(version, build_information_is_well_formed) {
        const std::string commit{calendrical::commit()};
        EXPECT_TRUE(commit == "unknown" || std::regex_match(commit, std::regex("[0-9a-f]{40}"))) << commit;

        const std::string datetime{calendrical::build_datetime()};
        EXPECT_TRUE(datetime == "unknown" ||
                    std::regex_match(datetime, std::regex(R"(\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z)"))) << datetime;

        EXPECT_FALSE(calendrical::branch().empty());
        EXPECT_FALSE(calendrical::build_type().empty());
        EXPECT_FALSE(calendrical::compiler().empty());
        EXPECT_FALSE(calendrical::platform().empty());
        EXPECT_GE(calendrical::cxx_standard(), 202002L);
    }

    TEST(version, libinfo_summarizes_build_information) {
        const auto info = calendrical::libinfo();
        EXPECT_NE(info.find("calendrical " + std::string(calendrical::library_version())), std::string::npos);
        EXPECT_NE(info.find(std::string(calendrical::commit().substr(0, 12))), std::string::npos);
        EXPECT_NE(info.find(std::string(calendrical::build_datetime())), std::string::npos);
        EXPECT_NE(info.find(std::string(calendrical::compiler())), std::string::npos);
        EXPECT_NE(info.find(std::string(calendrical::platform())), std::string::npos);
    }
}
