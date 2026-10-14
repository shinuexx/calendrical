#pragma once

#include <string>
#include <string_view>

#include "version_config.hpp"

/**
 * @file version.hpp
 * @brief Library version and build information.
 *
 * Two kinds of information are available:
 * - **Header version** (`version()`, `version_major()`, ...): constexpr values of the headers the
 *   caller was compiled against, also available as `CALENDRICAL_VERSION_*` macros.
 * - **Build information** (`library_version()`, `commit()`, `build_datetime()`, ...): recorded when
 *   the linked library itself was built. `library_version() != version()` reveals headers that do
 *   not match the binary.
 *
 * Values that could not be determined (no git checkout, timestamp disabled with
 * `CALENDRICAL_BUILD_TIMESTAMP=OFF`) are reported as `"unknown"`.
 */

namespace calendrical {
    /** @brief Header version as `"major.minor.patch"`. */
    [[nodiscard]] constexpr std::string_view version() noexcept { return CALENDRICAL_VERSION_STRING; }
    /** @brief Header major version. */
    [[nodiscard]] constexpr int version_major() noexcept { return CALENDRICAL_VERSION_MAJOR; }
    /** @brief Header minor version. */
    [[nodiscard]] constexpr int version_minor() noexcept { return CALENDRICAL_VERSION_MINOR; }
    /** @brief Header patch version. */
    [[nodiscard]] constexpr int version_patch() noexcept { return CALENDRICAL_VERSION_PATCH; }

    /** @brief Version of the linked library binary as `"major.minor.patch"`. */
    [[nodiscard]] std::string_view library_version() noexcept;
    /** @brief UTC time the library was built, ISO 8601 (`2026-10-14T12:34:56Z`), or `"unknown"`. */
    [[nodiscard]] std::string_view build_datetime() noexcept;
    /** @brief Full git commit hash the library was built from, or `"unknown"`. */
    [[nodiscard]] std::string_view commit() noexcept;
    /** @brief Git branch the library was built from (`"HEAD"` when detached), or `"unknown"`. */
    [[nodiscard]] std::string_view branch() noexcept;
    /** @brief Whether tracked files had uncommitted changes when the library was built. */
    [[nodiscard]] bool is_dirty() noexcept;
    /** @brief Build configuration (`Release`, `Debug`, ...), or `"unspecified"`. */
    [[nodiscard]] std::string_view build_type() noexcept;
    /** @brief Compiler that built the library, such as `GNU 13.2.0` or `MSVC 19.40.33811.0`. */
    [[nodiscard]] std::string_view compiler() noexcept;
    /** @brief Target platform of the build, such as `Linux x86_64`. */
    [[nodiscard]] std::string_view platform() noexcept;
    /** @brief Value of `__cplusplus` the library was compiled with, such as `202002`. */
    [[nodiscard]] long cxx_standard() noexcept;

    /**
     * @brief Multi-line, human-readable summary of all of the above.
     *
     * Example:
     * @code
     * calendrical 1.0.0
     *   commit:   0efb89fd0d31 (refactor/code-review-recommendations, dirty)
     *   built:    2026-10-14T12:34:56Z, Release
     *   compiler: GNU 13.2.0, C++ 202002
     *   platform: Linux x86_64
     * @endcode
     */
    [[nodiscard]] std::string libinfo();
}
