#pragma once

#include <string>
#include <iostream>

namespace calendrical {
    /**
     * @brief Traditional Chinese marriage augury categories.
     */
    enum class chinese_marriage_augury {
        /** @brief Most auspicious category in the traditional classification. */
        double_bright = 3,
        /** @brief Favorable category in the traditional classification. */
        bright = 2,
        /** @brief Unfavorable "blind" category in the traditional classification. */
        blind = 1,
        /** @brief Unfavorable "widow" category in the traditional classification. */
        widow = 0
    };

    /** @brief Returns the lowercase augury category name. */
    std::string to_string(const chinese_marriage_augury& augury);

    /** @brief Stream output for @ref chinese_marriage_augury. */
    std::ostream& operator<<(std::ostream& out, const chinese_marriage_augury& augury);
}
