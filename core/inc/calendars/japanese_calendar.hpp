#pragma once

#include "chinese_based_calendar.hpp"

namespace calendrical::calendars{
    class japanese_calendar : public chinese_based_calendar{
    public:
        japanese_calendar() = default;
        ~japanese_calendar() override = default;

        [[nodiscard]] location get_location(const moment_t& t) const override;
    };
}