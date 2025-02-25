#pragma once

#include "moment.hpp"

namespace calendrical::primitives {

    static constexpr moment_t JD_EPOCH{ -1721424.5 };

    static constexpr moment_t moment_from_jd(double jd) {
        return jd + JD_EPOCH;
    }

    static constexpr double jd_from_moment(moment_t t) {
        return t - JD_EPOCH;
    }

    static constexpr fixed_date_t MJD_EPOCH{ 678576 };

    static constexpr fixed_date_t fixed_from_mjd(int64_t mjd) {
        return mjd + MJD_EPOCH;
    }

    static constexpr int64_t mjd_from_fixed(fixed_date_t date) {
        return date - MJD_EPOCH;
    }

    static constexpr fixed_date_t UNIX_EPOCH{ 719163 };

    static constexpr moment_t moment_from_unix(double s) {
        return UNIX_EPOCH + s / (24 * 60 * 60);
    }
    static constexpr double unix_from_moment(moment_t t) {
        return 24 * 60 * 60 * (t - UNIX_EPOCH);
    }

    static fixed_date_t fixed_from_jd(double jd) {
        return moment_from_jd(jd).to_fixed_date();
    }
}
