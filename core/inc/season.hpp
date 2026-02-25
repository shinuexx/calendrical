#pragma once

#include <string>
#include <iostream>

namespace calendrical {
    /**
     * @brief Cardinal seasonal markers represented as solar longitudes.
     *
     * Enum values are expressed in degrees: spring=0, summer=90, autumn=180,
     * winter=270.
     */
    enum class season {
        SPRING = 0,
        SUMMER = 90,
        AUTUMN = 180,
        WINTER = 270
    };

    /** @brief Returns the lowercase season name. */
    std::string to_string(const season& s);
}

/** @brief Stream output for @ref calendrical::season. */
std::ostream& operator<<(std::ostream& out, const calendrical::season& s);
