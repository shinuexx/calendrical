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
        NEW = 0,
        FIRST_QUARTER = 90,
        FULL = 180,
        LAST_QUARTER = 270
    };

    /** @brief Returns the lowercase lunar phase name. */
    std::string to_string(const lunar_phase& phase);
}

/** @brief Stream output for @ref calendrical::lunar_phase. */
std::ostream& operator<<(std::ostream& out, const calendrical::lunar_phase& phase);
