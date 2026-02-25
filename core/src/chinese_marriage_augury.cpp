#include "chinese_marriage_augury.hpp"
#include <sstream>

namespace calendrical {
    std::string to_string(const chinese_marriage_augury& augury)
    {
        std::ostringstream oss{};
        switch (augury)
        {
        case chinese_marriage_augury::double_bright: oss << "double_bright"; break;
        case chinese_marriage_augury::bright: oss << "bright"; break;
        case chinese_marriage_augury::blind: oss << "blind"; break;
        case chinese_marriage_augury::widow: oss << "widow"; break;
        default: oss << "chinese_marriage_augury{" << static_cast<int32_t>(augury) << '}'; break;
        }
        return oss.str();
    }
}

std::ostream& operator<<(std::ostream& out, const calendrical::chinese_marriage_augury& augury) {
    return out << to_string(augury);
}
