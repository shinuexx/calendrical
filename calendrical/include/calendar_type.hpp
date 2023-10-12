#pragma once

#include <string>
#include <iostream>

namespace calendrical {
    enum class calendar_type
    {
        UNKNOWN,
        SOLAR,
        LUNAR,
        LUNISOLAR
    }; 

    std::string to_string(const calendrical::calendar_type& type);
}

std::ostream& operator<<(std::ostream& out, const calendrical::calendar_type& type);