#pragma once

#include <string>
#include <iostream>

namespace calendrical {
    /**
     * @brief Gregorian month enumeration.
     *
     * Values follow the conventional 1-based month numbering.
     */
    enum class month
    {
        /** @brief January (month 1). */
        january = 1,
        /** @brief February (month 2). */
        february,
        /** @brief March (month 3). */
        march,
        /** @brief April (month 4). */
        april,
        /** @brief May (month 5). */
        may,
        /** @brief June (month 6). */
        june,
        /** @brief July (month 7). */
        july,
        /** @brief August (month 8). */
        august,
        /** @brief September (month 9). */
        september,
        /** @brief October (month 10). */
        october,
        /** @brief November (month 11). */
        november,
        /** @brief December (month 12). */
        december
    };

    /** @brief Returns the lowercase month name. */
    std::string to_string(const month& m);

}

/** @brief Stream output for @ref calendrical::month. */
std::ostream& operator<<(std::ostream& out, const calendrical::month& m);
