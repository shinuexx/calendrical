#pragma once

#include <string>
#include <iostream>
#include "types.hpp"

namespace calendrical {
    /**
     * @brief Gregorian month enumeration.
     *
     * Values follow the conventional 1-based month numbering.
     */
    enum class month
    {
        january = 1,
        february,
        march,
        april,
        may,
        june,
        july,
        august,
        september,
        october,
        november,
        december
    };

    /** @brief Returns the lowercase month name. */
    std::string to_string(const month& m);

    template<>
    struct from_parser<month> {
        /** @brief Parses a lowercase English month name. */
        static month from_string(const std::string &s) {
            if (s == "january") return month::january;
            if (s == "february") return month::february;
            if (s == "march") return month::march;
            if (s == "april") return month::april;
            if (s == "may") return month::may;
            if (s == "june") return month::june;
            if (s == "july") return month::july;
            if (s == "august") return month::august;
            if (s == "september") return month::september;
            if (s == "october") return month::october;
            if (s == "november") return month::november;
            if (s == "december") return month::december;
            return static_cast<month>(-1);
        }
    };

}

/** @brief Stream output for @ref calendrical::month. */
std::ostream& operator<<(std::ostream& out, const calendrical::month& m);
