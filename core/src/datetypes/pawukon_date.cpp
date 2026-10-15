#include "datetypes/pawukon_date.hpp"
#include <sstream>

namespace calendrical::datetypes {
    std::string pawukon_date::to_string() const {
        std::ostringstream oss{};
        oss << "pawukon_date{luang:" << (luang ? "true" : "false")
            << ",dwiwara:" << dwiwara
            << ",triwara:" << triwara
            << ",caturwara:" << caturwara
            << ",pancawara:" << pancawara
            << ",sadwara:" << sadwara
            << ",saptawara:" << saptawara
            << ",asatawara:" << asatawara
            << ",sangawara:" << sangawara
            << ",dasawara:" << dasawara << '}';
        return oss.str();
    }

    std::ostream& operator<<(std::ostream& out, const pawukon_date& date) {
        return out << date.to_string();
    }
}
