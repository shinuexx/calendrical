#pragma once

#include <string>
#include <iostream>

namespace calendrical {
    /**
     * @brief Traditional Chinese marriage augury categories.
     */
    enum class chinese_marriage_augury {
        DOUBLE_BRIGHT = 3,
        BRIGHT = 2,
        BLIND = 1,
        WIDOW = 0
    };

    /** @brief Returns the lowercase augury category name. */
    std::string to_string(const chinese_marriage_augury& augury);
}

/** @brief Stream output for @ref calendrical::chinese_marriage_augury. */
std::ostream& operator<<(std::ostream& out, const calendrical::chinese_marriage_augury& augury);
