#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "calendars/observational_hebrew_calendar.hpp"

namespace tests {

using namespace calendrical;

class observational_hebrew_holidays_test : public ::testing::Test {
protected:
    calendars::observational_hebrew_calendar m_calendar{};
    const holiday_csv& m_csv = holiday_csv::instance();
};

TEST_F(observational_hebrew_holidays_test, classical_passover_eve) {
    m_csv.verify_holiday("classical_passover_eve", [this](int32_t y) { return m_calendar.classical_passover_eve(y); });
}

TEST_F(observational_hebrew_holidays_test, first_of_nisan) {
    m_csv.verify_holiday("observ_hebrew_1_nisan", [this](int32_t y) { return m_calendar.first_of_nisan(y); });
}

} // namespace tests
