#pragma once

#include "bahai_based_calendar.hpp"

namespace calendrical::calendars {
    using namespace calendrical::datetypes;

    class bahai_calendar :
        public bahai_based_calendar {

    protected:
        virtual int32_t get_years(const fixed_date_t& date) const override;

    public:
        bahai_calendar() = default;

        virtual ~bahai_calendar() = default;

        virtual bahai_date to_date(const fixed_date_t& date) const override;
        virtual fixed_date_t to_fixed(const bahai_date& date) const override;
    };
}