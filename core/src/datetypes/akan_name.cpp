#include "datetypes/akan_name.hpp"
#include "helpers.hpp"
#include <sstream>

namespace calendrical::datetypes {
    int32_t akan_name::difference(const akan_name &other) const {
        const auto prefix_difference = static_cast<int64_t>(other.prefix) - prefix;
        const auto stem_difference = static_cast<int64_t>(other.stem) - stem;
        // 36 = 0 (mod 6) and 36 = 1 (mod 7), so the result is congruent to
        // prefix_difference modulo 6 and to stem_difference modulo 7.
        return static_cast<int32_t>(
            adjusted_remainder(prefix_difference + 36 * (stem_difference - prefix_difference), cycle_length));
    }
    std::string akan_name::to_string() const {
        std::ostringstream oss{};
        oss << "akan_name{prefix:" << prefix << ",stem:" << stem << '}';
        return oss.str();
    }

    std::ostream& operator<<(std::ostream& out, const akan_name& name) {
        return out << name.to_string();
    }
}
