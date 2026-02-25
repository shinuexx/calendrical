#pragma once

#include "location.hpp"
#include "moment.hpp"

namespace calendrical::calendars {
    /**
     * @brief Shared astronomical helpers for Chinese-family lunisolar calendars.
     */
    class chinese_base{
    public:
        /** @brief Function type that returns the observational location for a moment. */
        using location_provider = std::function<location(const moment_t&)>;

        virtual ~chinese_base() = default;

        /** @brief Returns the location used for astronomical calculations at moment @p t. */
        [[nodiscard]] virtual location get_location(const moment_t& t) const = 0;

        /** @brief Default China location rule used by some Chinese-family calendars. */
        [[nodiscard]] static location chinese_location(const moment_t& t);

        /** @brief Current major solar term index on a fixed date. */
        [[nodiscard]] int32_t current_major_solar_term(const fixed_date_t& date) const;
        /** @brief First moment on or after @p t when solar longitude reaches @p l. */
        [[nodiscard]] moment_t solar_longitude_on_or_after(const angle_t& l, const moment_t& t) const;
        /** @brief Major solar term on or after a fixed date. */
        [[nodiscard]] moment_t major_solar_term_on_or_after(const fixed_date_t& date) const;
        /** @brief Minor solar term on or after a fixed date. */
        [[nodiscard]] moment_t minor_solar_term_on_of_after(const fixed_date_t& date) const;
        /** @brief Local midnight in the Chinese calendar observation location. */
        [[nodiscard]] moment_t midnight_in_china(const fixed_date_t& date) const;
        /** @brief Winter solstice date on or before @p date. */
        [[nodiscard]] fixed_date_t winter_solstice_on_or_before(const fixed_date_t& date) const;

        /** @brief New moon date on or after @p date. */
        [[nodiscard]] fixed_date_t new_moon_on_or_after(const fixed_date_t& date) const;
        /** @brief New moon date before @p date. */
        [[nodiscard]] fixed_date_t new_moon_before(const fixed_date_t& date) const;

        /** @brief Returns whether the lunar month starting at @p date has no major solar term. */
        [[nodiscard]] bool no_major_solar_term(const fixed_date_t& date) const;
        /** @brief Returns whether a prior leap month exists between @p m1 and @p m. */
        [[nodiscard]] bool is_prior_leap_month(const fixed_date_t& m1, const fixed_date_t& m) const;

        /** @brief Chinese New Year within the sui containing @p date. */
        [[nodiscard]] fixed_date_t new_year_in_sui(const fixed_date_t& date) const;
        /** @brief Chinese New Year on or before @p date. */
        [[nodiscard]] fixed_date_t new_year_on_or_before(const fixed_date_t& date) const;
    };
}
