#pragma once

#include <string>
#include <iostream>

namespace calendrical {
    enum class month
    {
        january = 1,
        february,
        march,
        april,
        may,
        june,
        july,
        august,
        september,
        october,
        november,
        december
    };

    std::string to_string(const month& m);
}

std::ostream& operator<<(std::ostream& out, const calendrical::month& m);