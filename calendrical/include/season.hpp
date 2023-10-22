#pragma once

#include <string>
#include <iostream>

namespace calendrical {
    enum class season {
        SPRING = 0,
        SUMMER = 90,
        AUTUMN = 180,
        WINTER = 270
    };

    std::string to_string(const season& s);
}

std::ostream& operator<<(std::ostream& out, const calendrical::season& s);