#pragma once

#include <cstdint>
#include <string>

namespace calendrical::datetypes {
    struct sexagesimal_name {
        int32_t stem, branch;

        int32_t difference(const sexagesimal_name& name2) const;
    
        static sexagesimal_name from(int64_t n);

        std::string to_string() const;

        constexpr bool operator==(const sexagesimal_name& other) const = default;
    };
}

std::ostream& operator<<(std::ostream& out, const calendrical::datetypes::sexagesimal_name& name);