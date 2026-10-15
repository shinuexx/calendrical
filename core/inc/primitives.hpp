#pragma once

#include "moment.hpp"

namespace calendrical::primitives {

    /** @brief Julian Day epoch offset in library `moment_t` coordinates. */
    inline constexpr moment_t JD_EPOCH{ -1721424.5 };

    /** @brief Converts Julian Day number to library moment. */
    inline constexpr moment_t moment_from_jd(double jd) {
        return jd + JD_EPOCH;
    }

    /** @brief Converts library moment to Julian Day number. */
    inline constexpr double jd_from_moment(moment_t t) {
        return (t - JD_EPOCH).days();
    }

    /** @brief Modified Julian Day epoch offset in library `fixed_date_t` coordinates. */
    inline constexpr fixed_date_t MJD_EPOCH{ 678576 };

    /** @brief Converts Modified Julian Day number to fixed date. */
    inline constexpr fixed_date_t fixed_from_mjd(int64_t mjd) {
        return mjd + MJD_EPOCH;
    }

    /** @brief Converts fixed date to Modified Julian Day number. */
    inline constexpr int64_t mjd_from_fixed(fixed_date_t date) {
        return date - MJD_EPOCH;
    }

    /** @brief Unix epoch (1970-01-01) in fixed-date coordinates. */
    inline constexpr fixed_date_t UNIX_EPOCH{ 719163 };

    /** @brief Converts Unix seconds since 1970-01-01T00:00:00Z to moment. */
    inline constexpr moment_t moment_from_unix(double s) {
        return moment_t{ UNIX_EPOCH } + s / (24 * 60 * 60);
    }
    /** @brief Converts a moment to Unix seconds since 1970-01-01T00:00:00Z. */
    inline constexpr double unix_from_moment(moment_t t) {
        return 24 * 60 * 60 * (t - UNIX_EPOCH).days();
    }

    /** @brief Converts Julian Day number to fixed date using `moment_t` rounding rules. */
    inline fixed_date_t fixed_from_jd(double jd) {
        return moment_from_jd(jd).to_fixed_date();
    }
}
