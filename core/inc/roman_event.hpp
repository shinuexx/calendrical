#pragma once

#include <string>
#include <iostream>

namespace calendrical {
    /**
     * @brief Roman calendrical reference points used in Roman date notation.
     */
    enum class roman_event {
        /** @brief Kalends (the first day of the Roman month). */
        kalends = 1,
        /** @brief Nones (the early-month reference day in Roman notation). */
        nones = 2,
        /** @brief Ides (the mid-month reference day in Roman notation). */
        ides = 3
    };

    /** @brief Returns the lowercase Roman event name. */
    std::string to_string(const roman_event& event);
}

/** @brief Stream output for @ref calendrical::roman_event. */
std::ostream& operator<<(std::ostream& out, const calendrical::roman_event& event);
