# calendrical

C++20 library for calendrical and astronomical calculations (many calendar systems plus holiday providers), with a `calendrical_cli` executable and GoogleTest-based tests.

This README is also used as the Doxygen main page.

## Overview

The project provides:

- Calendar conversions to and from a fixed-date internal representation (`fixed_date_t`)
- Time and astronomy utilities (sunrise/sunset, lunar phase, solar longitude, etc.)
- Holiday providers for selected calendar systems
- A CLI for conversions and holiday queries

## Requirements

- CMake >= 3.20
- C++20 compiler (`g++`, `clang++`, or MSVC)
- Build generator (for example `Ninja` or Make)

The core library is written in pure C++20 and has **no external dependencies**. GoogleTest for the test suite is either found on the system or automatically fetched via CMake `FetchContent`.

## Configure With CMake

Using a preset:

```bash
cmake --preset ninja-debug
```

Manual configuration:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
```

## Build

```bash
cmake --build build -j4
```

Typical outputs:

- `build/core/libcalendrical.a`
- `build/cli/calendrical_cli`
- `build/tests/tests`

## Run Tests

Using CTest:

```bash
ctest --test-dir cmake-build-debug --output-on-failure
```

Or directly:

```bash
./cmake-build-debug/tests/tests
```

List registered GoogleTest tests:

```bash
./cmake-build-debug/tests/tests --gtest_list_tests
```

## Build API Documentation (Doxygen)

If `doxygen` is installed, the project exposes a `docs` target.

```bash
cmake --build cmake-build-debug --target docs
```

Generated HTML documentation is written to:

- `cmake-build-debug/docs/html/index.html`

## CLI Usage

Show help:

```bash
./cmake-build-debug/cli/calendrical_cli --help
```

Examples:

```bash
./cmake-build-debug/cli/calendrical_cli list commands
./cmake-build-debug/cli/calendrical_cli list calendars
./cmake-build-debug/cli/calendrical_cli list holiday-calendars
./cmake-build-debug/cli/calendrical_cli convert gregorian julian 2026-02-18
./cmake-build-debug/cli/calendrical_cli convert rd hebrew 750000
./cmake-build-debug/cli/calendrical_cli convert jd gregorian 2461090.5
./cmake-build-debug/cli/calendrical_cli convert unix gregorian 1700000000
./cmake-build-debug/cli/calendrical_cli holidays gregorian 2026
./cmake-build-debug/cli/calendrical_cli holidays --as-gregorian gregorian 2026
```

CLI syntax:

```text
calendrical_cli <command> [args]
calendrical_cli help
calendrical_cli --help

calendrical_cli convert <from> <to> <date>
calendrical_cli convert --help

calendrical_cli holidays [--as-gregorian] <provider> <year>
calendrical_cli holidays --help

calendrical_cli list [topic]
calendrical_cli list --help

calendrical_cli interactive
calendrical_cli -i
calendrical_cli --interactive
calendrical_cli interactive --help
```

`list [topic]` values:

- `commands` (default)
- `calendars`
- `holiday-calendars`

## Interactive Mode

Start an interactive REPL session to perform multiple conversions, holiday queries, and listings without restarting the application:

```bash
./cmake-build-debug/cli/calendrical_cli interactive
# or using aliases:
./cmake-build-debug/cli/calendrical_cli -i
./cmake-build-debug/cli/calendrical_cli --interactive
```

Within the session, enter subcommands at the prompt (`> `) without the `calendrical_cli` binary prefix:

```text
> convert gregorian julian 2026-02-18
> holidays --as-gregorian us 2026
> list calendars
> help
> exit
```

Arguments with spaces can be enclosed in single (`'...'`) or double (`"..."`) quotes. If a single command fails, an error is reported and the session continues. Type `exit` or `quit` (or press Ctrl+D / send EOF) to exit.

## Registered Convert Converters

- `rd`
- `jd`
- `mjd`
- `unix`
- `unix_timestamp`
- `arithmetic_french`
- `arithmetic_persian`
- `armenian`
- `astro_bahai`
- `astro_hindu_lunar`
- `astro_hindu_solar`
- `babylonian`
- `bahai`
- `chinese`
- `coptic`
- `egyptian`
- `ethiopic`
- `french`
- `gregorian`
- `hebrew`
- `icelandic`
- `islamic`
- `iso`
- `japanese`
- `julian`
- `korean`
- `modern_hindu_lunar`
- `modern_hindu_solar`
- `observational_hebrew`
- `observational_islamic`
- `old_hindu_lunar`
- `old_hindu_solar`
- `persian`
- `roman`
- `samaritan`
- `saudi_islamic`
- `tibetan`
- `vietnamese`

## Convert Date Input Formats

Depending on converter:

- `rd`, `mjd`: `N`
- `jd`: `N` or `N.frac`
- `unix`, `unix_timestamp`: `seconds_since_epoch`
- simple calendars: `YEAR-MONTH-DAY`
- `gregorian`, `julian`, `hebrew`, `observational_hebrew`: `YEAR-MONTH-DAY` (`MONTH` may be a name or number)
- `iso`: `YEAR-WEEK-DAY`
- `icelandic`: `YEAR-SEASON-WEEK-WEEKDAY`
- `bahai`, `astro_bahai`: `MAJOR-CYCLE-YEAR-MONTH-DAY`
- `chinese`, `japanese`, `korean`, `vietnamese`: `CYCLE-YEAR-MONTH-LEAP_MONTH-DAY`
- `astro_hindu_lunar`, `modern_hindu_lunar`, `tibetan`: `YEAR-MONTH-LEAP_MONTH-DAY-LEAP_DAY`
- `old_hindu_lunar`, `babylonian`: `YEAR-MONTH-LEAP_MONTH-DAY`
- `roman`: `YEAR-MONTH-EVENT-COUNT-LEAP`

## Registered Holidays Providers

- `gregorian`
- `us`
- `julian`
- `islamic_based`
- `hebrew_based`
- `bahai_based`
- `persian_based`
- `modern_hindu_lunar`
- `coptic`
- `ethiopic`
- `egyptian`
- `armenian`
- `pawukon` (Balinese Pawukon: every Kajeng Keliwon and Tumpek in the year)

Provider aliases supported by the `holidays` parser:

- `gregorian_calendar` -> `gregorian`
- `us_calendar` / `usa` -> `us`
- `julian_calendar` -> `julian`
- `islamic` / `islamic_based_calendar` -> `islamic_based`
- `hebrew` / `hebrew_based_calendar` -> `hebrew_based`
- `bahai` / `bahai_based_calendar` -> `bahai_based`
- `persian` / `persian_based_calendar` -> `persian_based`
- `modern_hindu_lunar_calendar` -> `modern_hindu_lunar`

## Library Usage Example

```cpp
#include <calendars/modern_hindu_solar_calendar.hpp>
#include <fixed_date.hpp>

int main() {
    calendrical::calendars::modern_hindu_solar_calendar cal{};
    const calendrical::fixed_date_t rd = 10_rd;
    auto d = cal.to_date(rd);
    auto back = cal.to_fixed(d);
    (void)back;
}
```

Public headers live in `core/inc`, implementations in `core/src`.

## Project Structure

- `core/`
  - `core/inc/` public library API
  - `core/src/` implementations
- `cli/` CLI application
- `tests/` GoogleTest-based tests
- `cmake/` package config and build-info templates
- `VERSION` library version (single source of truth)
- `CMakeLists.txt` top-level build configuration

## Installation and CMake Integration

To install the library, headers, CLI binary, and CMake package configuration:

```bash
cmake --install build --prefix /usr/local
```

Consuming `calendrical` in an external CMake project:

```cmake
find_package(calendrical CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE calendrical::calendrical)
```

Installed headers live in `<prefix>/include/calendrical/` and are included by bare name
(`#include "version.hpp"`) through the `calendrical::calendrical` target.

## Version and Build Information

The library version is read from the `VERSION` file; bump it there. `version.hpp` exposes it together
with information recorded when the library was built:

```cpp
#include "version.hpp"

calendrical::version();          // "0.1.0" - headers you compiled against (constexpr)
calendrical::library_version();  // "0.1.0" - the linked binary; differs if headers and library mismatch
calendrical::commit();           // full git hash, or "unknown" outside a git checkout
calendrical::branch();           // git branch, "HEAD" when detached
calendrical::is_dirty();         // uncommitted changes to tracked files at build time
calendrical::build_datetime();   // "2026-10-14T12:34:56Z" (UTC, honours SOURCE_DATE_EPOCH)
calendrical::build_time();       // the same as std::optional<std::chrono::sys_seconds>
calendrical::build_type();       // "Release", "Debug", ... or "unspecified"
calendrical::compiler();         // "GNU 13.3.0"
calendrical::platform();         // "Linux x86_64"
calendrical::cxx_standard();     // 202002
calendrical::libinfo();          // all of the above as a multi-line summary
```

The `CALENDRICAL_VERSION_MAJOR`, `_MINOR`, `_PATCH`, `_STRING` and `_NUMBER` macros allow
preprocessor checks. The CLI prints the summary with `calendrical_cli --version`.

Build information is refreshed on every build. Configure with `-DCALENDRICAL_BUILD_TIMESTAMP=OFF` to
leave the timestamp out; the build-info source is then recompiled only when the git state changes.

## Maya Long Count

The `mayan` / `mayan_long_count` converter supports canonical nine-field Long Count
and a five-field input shorthand, with the GMT correlation JDN 584283. For example:
`calendrical_cli convert mayan gregorian 13-0-0-0-0` gives 2012-12-21.
See [Long Count formats, bounds and reference dates](https://github.com/shinuexx/calendrical/blob/master/docs/mayan-long-count.md).

## Feature Scope (Short Summary)

- Calendars: Gregorian, Julian, Islamic variants, Hebrew variants, Persian variants, Chinese-family calendars, Hindu calendars, Baha'i calendars, Tibetan, ISO, and more
- Time and astronomy: sunrise/sunset, ecliptic longitudes, lunar phases, ephemeris corrections, etc.
- Holiday providers for selected calendars

## Holiday Rules and Design Notes

See [the holiday documentation index](https://github.com/shinuexx/calendrical/blob/master/docs/README.md) for implemented rules, proposed extensions,
open convention questions, and links to the matching providers and tests.

## Developer Workflow

1. Modify code.
2. Build:
   ```bash
   cmake --build build -j4
   ```
3. Test:
   ```bash
   ctest --test-dir build --output-on-failure
   ```
4. Review diff and commit.

## API Notes (Current State)

- `calendar_provider` has been removed to simplify API ownership/lifetimes.
- `holiday_t` compares objects by `date` only:
  - ordering is purely chronological
  - holidays on the same date compare equal
- `leap_day_aware_calendar` requires logical `is_leap_day(...)` (`bool`)

## Common Issues

- `ctest` finds no tests:
  - run `ctest --test-dir build --output-on-failure` after a successful configure/build
