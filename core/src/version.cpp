#include "version.hpp"

#include <sstream>

#ifndef CALENDRICAL_BUILD_TYPE
#define CALENDRICAL_BUILD_TYPE ""
#endif

namespace calendrical {
    std::string_view build_type() noexcept {
        constexpr std::string_view type = CALENDRICAL_BUILD_TYPE;
        return type.empty() ? std::string_view{"unspecified"} : type;
    }

    long cxx_standard() noexcept {
#if defined(_MSVC_LANG)
        return _MSVC_LANG;
#else
        return __cplusplus;
#endif
    }

    std::string libinfo() {
        const auto hash = commit();
        std::ostringstream out;
        out << "calendrical " << library_version() << '\n'
            << "  commit:   " << hash.substr(0, 12) << " (" << branch() << (is_dirty() ? ", dirty" : "") << ")\n"
            << "  built:    " << build_datetime() << ", " << build_type() << '\n'
            << "  compiler: " << compiler() << ", C++ " << cxx_standard() << '\n'
            << "  platform: " << platform() << '\n';
        return out.str();
    }
}
