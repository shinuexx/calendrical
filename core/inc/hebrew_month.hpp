#pragma once

#include <string>
#include <iostream>
#include "types.hpp"

namespace calendrical {
    /**
     * @brief Hebrew calendar month enumeration.
     *
     * Values follow the numbering used by the library's Hebrew calendar
     * algorithms, with `adarii` representing Adar II in leap years.
     */
    enum class hebrew_month {
        nisan = 1,
        iyyar,
        sivan,
        tammuz,
        av,
        elul,
        tishri,
        marheshvan,
        kislev,
        tevet,
        shevat,
        adar,
        adarii
    };

    /** @brief Returns the lowercase transliterated Hebrew month name. */
    std::string to_string(const hebrew_month& m);

    /**
     * @brief Parser specialization for @ref calendrical::hebrew_month from transliterated text.
     */
    template<>
    struct from_parser<hebrew_month> {
        /** @brief Parses a lowercase transliterated month name (supports `adar2`). */
        static hebrew_month from_string(const std::string &s) {
            if (s == "nisan") return hebrew_month::nisan;
            if (s == "iyyar") return hebrew_month::iyyar;
            if (s == "sivan") return hebrew_month::sivan;
            if (s == "tammuz") return hebrew_month::tammuz;
            if (s == "av") return hebrew_month::av;
            if (s == "elul") return hebrew_month::elul;
            if (s == "tishri") return hebrew_month::tishri;
            if (s == "marheshvan") return hebrew_month::marheshvan;
            if (s == "kislev") return hebrew_month::kislev;
            if (s == "tevet") return hebrew_month::tevet;
            if (s == "shevat") return hebrew_month::shevat;
            if (s == "adar") return hebrew_month::adar;
            if (s == "adarii" || s == "adar2") return hebrew_month::adarii;
            return static_cast<hebrew_month>(-1);
        }
    };
}

/** @brief Stream output for @ref calendrical::hebrew_month. */
std::ostream& operator<<(std::ostream& out, const calendrical::hebrew_month& m);
