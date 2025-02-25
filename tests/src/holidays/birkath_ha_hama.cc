#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "local_helpers.hpp"
#include <optional>

#include "holiday_providers/hebrew_based_calendar_holiday_provider.hpp"
#include "calendars/hebrew_calendar.hpp"

namespace tests{
    static constexpr std::array<std::optional<std::tuple<int32_t,int32_t>>, years.size()> month_day_dates{
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::make_tuple<int32_t,int32_t>(4, 8),
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::make_tuple<int32_t,int32_t>(4, 8),
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::make_tuple<int32_t,int32_t>(4, 8),
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::make_tuple<int32_t,int32_t>(4, 8),
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
    };

    class birkath_ha_hama_test : public helpers::input_output_test<int32_t, std::optional<std::tuple<int32_t, int32_t>>> {
    protected:
        const calendars::gregorian_calendar* m_gregorian;
        const calendars::hebrew_calendar m_hebrew{};
        holiday_providers::hebrew_based_calendar_holiday_provider* m_provider;

        void SetUp() override {
            m_provider = new holiday_providers::hebrew_based_calendar_holiday_provider(std::addressof(m_hebrew));
            m_gregorian = std::addressof(calendars::gregorian_calendar::standard);
        }
        void TearDown() override {
            delete m_provider;
            m_provider = nullptr;
            m_gregorian = nullptr;
        }
    };

    static std::optional<datetypes::gregorian_date> opt(const std::vector<fixed_date_t>& dates, const calendars::gregorian_calendar* gregorian) {
        if(dates.size() > 0) {
            return gregorian->to_date(dates.front());
        }
        else{
            return std::nullopt;
        }
    }

    template<typename T>
    std::string optional_to_string(const std::optional<T>& opt) {
        return opt ? opt->to_string() : "null";
    }

    TEST_P(birkath_ha_hama_test, calculate) {
        auto &[year, month_day] = GetParam();
        const auto dates = m_provider->birhath_ha_hama(year);
        const auto actual = opt(dates, m_gregorian);
        std::optional<datetypes::gregorian_date> expected{std::nullopt};
        if(month_day)
        {
            auto &[month, day] = *month_day;
            expected = datetypes::gregorian_date{year, calendrical::month{month}, day};
        }
        EXPECT_EQ(actual, expected) << year << " : " << optional_to_string(actual) << " : " << optional_to_string(expected) << std::endl;
    }

    INSTANTIATE_TEST_SUITE_P(birkath_ha_hama, birkath_ha_hama_test, testing::ValuesIn(helpers::make_test_params_values(years, month_day_dates)));
}