#pragma once

#include <string>
#include <iostream>

namespace calendrical {
    enum class chinese_marriage_augury {
        DOUBLE_BRIGHT = 3,
        BRIGHT = 2,
        BLIND = 1,
        WIDOW = 0
    };

    std::string to_string(const chinese_marriage_augury& augury);
}

std::ostream& operator<<(std::ostream& out, const calendrical::chinese_marriage_augury& augury);