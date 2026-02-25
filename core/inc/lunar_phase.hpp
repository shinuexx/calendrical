#pragma once

#include <string>
#include <iostream>

namespace calendrical {
    /**
     * @brief Principal lunar phases represented as phase angles in degrees.
     *
     * Values are 0=new moon, 90=first quarter, 180=full moon, 270=last
     * quarter.
     */
    enum class lunar_phase {
        /** @brief New moon phase (0° elongation). */
        new_moon = 0,
        /** @brief First quarter phase (90° elongation). */
        first_quarter = 90,
        /** @brief Full moon phase (180° elongation). */
        full_moon = 180,
        /** @brief Last quarter phase (270° elongation). */
        last_quarter = 270
    };

    /** @brief Returns the lowercase lunar phase name. */
    std::string to_string(const lunar_phase& phase);
}

/** @brief Stream output for @ref calendrical::lunar_phase. */
std::ostream& operator<<(std::ostream& out, const calendrical::lunar_phase& phase);
