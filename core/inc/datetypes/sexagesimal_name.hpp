#pragma once

#include <cstdint>
#include <string>

namespace calendrical::datetypes {
    struct sexagesimal_name {
        int32_t stem, branch;

        constexpr sexagesimal_name() = default;
        constexpr sexagesimal_name(int32_t stem, int32_t branch) : stem(stem), branch(branch) {}

        [[nodiscard]] int32_t difference(const sexagesimal_name &other) const;
        [[nodiscard]] static sexagesimal_name from(int64_t n);
        [[nodiscard]] std::string to_string() const;
        constexpr bool operator==(const sexagesimal_name &other) const = default;
    };
}

std::ostream& operator<<(std::ostream& out, const calendrical::datetypes::sexagesimal_name& odate);