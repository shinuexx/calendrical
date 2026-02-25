#pragma once

#include <string>
#include <iostream>

namespace calendrical {
    /**
     * @brief Roman calendrical reference points used in Roman date notation.
     */
    enum class roman_event {
        KALENDS = 1,
        NONES = 2,
        IDES = 3
    };

    /** @brief Returns the lowercase Roman event name. */
    std::string to_string(const roman_event& event);
}

/** @brief Stream output for @ref calendrical::roman_event. */
std::ostream& operator<<(std::ostream& out, const calendrical::roman_event& event);
