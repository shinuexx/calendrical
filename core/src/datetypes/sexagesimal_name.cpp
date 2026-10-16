#include "datetypes/sexagesimal_name.hpp"
#include "helpers.hpp"
#include <sstream>

namespace calendrical::datetypes {
    int32_t sexagesimal_name::difference(const sexagesimal_name &other) const {
        const auto stem_difference = static_cast<int64_t>(other.stem - stem);
        const auto branch_difference = static_cast<int64_t>(other.branch - branch);
        return static_cast<int32_t>(
             adjusted_remainder(stem_difference + 25 * (branch_difference - stem_difference), 60));
    }
    sexagesimal_name sexagesimal_name::from(int64_t n) {
    return {
     static_cast<int32_t>(adjusted_remainder(n, 10)),
    static_cast<int32_t>(adjusted_remainder(n, 12)),
        };
    }
    std::string sexagesimal_name::to_string() const {
        std::ostringstream oss{};
        oss << "sexagesimal_name{stem:" << stem << ",branch:" << branch << '}';
        return oss.str();
    }

    std::ostream& operator<<(std::ostream& out, const sexagesimal_name& odate) {
        return out << odate.to_string();
    }
}