#include "chinese_marriage_augury.hpp"
#include <sstream>

namespace calendrical {
    std::string to_string(const chinese_marriage_augury& augury)
    {
        std::ostringstream oss{};
        switch (augury)
        {
        case chinese_marriage_augury::DOUBLE_BRIGHT: oss << "DOUBLE_BRIGHT"; break;
        case chinese_marriage_augury::BRIGHT: oss << "BRIGHT"; break;
        case chinese_marriage_augury::BLIND: oss << "BLIND"; break;
        case chinese_marriage_augury::WIDOW: oss << "WIDOW"; break;
        default: oss << "chinese_marriage_augury{" << static_cast<int32_t>(augury) << '}'; break;
        }
        return oss.str();
    }
}

std::ostream& operator<<(std::ostream& out, const calendrical::chinese_marriage_augury& augury) {
    return out << to_string(augury);
}