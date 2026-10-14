#pragma once

#include "bahai_based_calendar.hpp"
#include "location.hpp"

namespace calendrical::calendars{

    /**
     * @brief Astronomical Baha'i calendar implementation (Tehran sunset-based).
     */
    class astro_bahai_calendar final : public bahai_based_calendar{
    protected:
        /** @brief Returns elapsed Baha'i years from epoch for a fixed date. */
        [[nodiscard]] int32_t get_years(const fixed_date_t& date) const override;

    public:
        /** @brief Baha'i sunset at the reference location for a fixed date. */
        [[nodiscard]] moment_t bahai_sunset(const fixed_date_t& date) const;
        /** @brief Baha'i New Year on or before a fixed date. */
        [[nodiscard]] fixed_date_t new_year_on_or_before(const fixed_date_t& date) const;

        /** @brief Reference location used by the astronomical Baha'i calendar. */
        static constexpr location bahai_location{35.696111_deg, 51.423056_deg, 0, moment_t::hms(3.5)};

        /** @brief Constructs an astronomical Baha'i calendar instance. */
        astro_bahai_calendar() = default;

        ~astro_bahai_calendar() override = default;

        /** @brief Converts a fixed date to astronomical Baha'i date components. */
        [[nodiscard]] bahai_date to_date(const fixed_date_t& date) const override;
        /** @brief Converts astronomical Baha'i date components to fixed-date representation. */
        [[nodiscard]] fixed_date_t to_fixed(const bahai_date& date) const override;
    };
}
