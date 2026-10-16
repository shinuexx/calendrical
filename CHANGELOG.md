# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and the project
follows [Semantic Versioning](https://semver.org/spec/v2.0.0.html). While the major version is 0,
a minor version bump may contain breaking API changes; the installed CMake package only accepts
consumers that request the same minor version.

Entries up to 0.2.0 were reconstructed from the merged pull requests.

## Unreleased

### Fixed

- `sexagesimal_name::difference()` counted in the wrong direction for names whose stem and
  branch differ, so `chinese_based_calendar::day_name_on_or_before()` could return the wrong day.
  `a.difference(b)` now returns the number of days forward from `a` to `b` (1..60; equal names
  yield 60) ([#30](https://github.com/shinuexx/calendrical/issues/30),
  [#31](https://github.com/shinuexx/calendrical/pull/31)).

### Changed

- Internal cleanup of `static_cast`s on unit-type boundaries in the Hebrew and observational
  Hebrew calendars and the Hebrew-based holiday provider: raw-value casts were replaced with
  `value()` or typed `moment_t` arithmetic, redundant casts removed and the remaining scoped-enum
  casts documented. No API or result changes
  ([#28](https://github.com/shinuexx/calendrical/issues/28),
  [#32](https://github.com/shinuexx/calendrical/pull/32)).

### Documentation

- README section on explicit numeric conversions (`degrees()`, `value()`, `days()`) linking the
  migration guides; the package-consumer test now exercises `angle_t`, `fixed_date_t` and
  `moment_t` through the installed headers ([#22](https://github.com/shinuexx/calendrical/issues/22)).

## 0.2.0 - 2026-10-16

### Changed (breaking)

- `angle_t`, `fixed_date_t` and `moment_t` no longer convert implicitly to `double` / `int64_t`
  ([#4](https://github.com/shinuexx/calendrical/issues/4)). Mixing units, such as `10_rd + 20_deg`,
  no longer compiles. Read raw values explicitly with `angle_t::degrees()`, `fixed_date_t::value()`
  and `moment_t::days()` (or `static_cast`). See the migration guides for
  [angles](docs/angle-migration.md), [fixed dates](docs/fixed-date-migration.md) and
  [moments](docs/moment-migration.md).
  ([#24](https://github.com/shinuexx/calendrical/pull/24),
  [#25](https://github.com/shinuexx/calendrical/pull/25),
  [#26](https://github.com/shinuexx/calendrical/pull/26))
- `fixed_date_t` converts implicitly to `moment_t` (midnight of that day), so moments and dates
  can be compared and subtracted directly; `fixed_date_t::end_of_time` maps to
  `moment_t::end_of_time` ([#23](https://github.com/shinuexx/calendrical/pull/23)).
- `moment_t::to_fixed_date()` maps `moment_t::end_of_time` to `fixed_date_t::end_of_time` and throws
  `std::domain_error` for NaN, negative infinity and values outside the `int64_t` range instead of
  casting out of range ([#23](https://github.com/shinuexx/calendrical/pull/23)).

### Added

- Unit-preserving helpers: `mod`, `interval_modulus`, `floor_div`, `floor`, `ceil` and `abs` for
  `angle_t`; `floor`, `ceil`, `round` (returning `fixed_date_t`) and `abs` for `moment_t`;
  `angle_t::day_fraction()` / `day_fraction(angle_t)` for converting an angle to a fraction of a day
  ([#23](https://github.com/shinuexx/calendrical/pull/23),
  [#25](https://github.com/shinuexx/calendrical/pull/25)).
- Compile-time unit-safety tests (`tests/src/type_safety.cc`).
- Mayan Long Count calendar and CLI converters
  ([#17](https://github.com/shinuexx/calendrical/pull/17)).
- Balinese Pawukon calendar with Kajeng Keliwon and Tumpek
  ([#16](https://github.com/shinuexx/calendrical/pull/16)).
- Interactive CLI session mode (`-i`, `--interactive`)
  ([#15](https://github.com/shinuexx/calendrical/pull/15)).
- Akan calendar and a cyclic calendar interface
  ([#12](https://github.com/shinuexx/calendrical/pull/12)).
- US holiday provider; the observational Hebrew 1 Nisan is exposed publicly
  ([#11](https://github.com/shinuexx/calendrical/pull/11)).
- Holiday rules and design notes extracted into `docs/`
  ([#13](https://github.com/shinuexx/calendrical/pull/13),
  [#14](https://github.com/shinuexx/calendrical/pull/14)).

### Fixed

- Overflow in `pawukon_calendar::positions_in_range`
  ([#16](https://github.com/shinuexx/calendrical/pull/16)).
- Non-zero command exit codes are reported in the interactive session
  ([#15](https://github.com/shinuexx/calendrical/pull/15)).

## 0.1.0 - 2026-10-14

First versioned release ([#3](https://github.com/shinuexx/calendrical/pull/3)).

### Added

- `VERSION` file as the single source of the library version, and a build information API in
  `version.hpp` (version, commit, branch, build time, compiler and platform).
- Chinese calendar holiday provider, also registered for the Korean and Vietnamese calendars.
- Installable CMake package with a smoke test of the installed package; `BUILD_TESTING` is honoured.
- Unit test suite for CLI commands and the dispatcher; holiday tests against a reference dataset.
- Complete Doxygen comments across core and CLI.

### Changed

- Modernized CMake build: vcpkg removed, packaging and warning flags added.
- Calendar date validation moved from `calendar_traits` to the converters.
- `holiday_t` ordering is chronological; same-day holidays are listed by name in the CLI.
- CLI error and help handling unified without exceptions for control flow.

### Fixed

- `int64_t` overflow in `least_passing` / `last_passing`.
- Observational Hebrew dates validated against observed month lengths.
- Hindu lunar-day search window and solar equation of time.
- Nowruz calculation.
- Negative values accepted in the compact date format and in the `holidays` command.
- MSVC compatibility (missing includes, warnings C4459 and C4244).

### Earlier history

Before 0.1.0 the project was not versioned. Notable work in that period: the core calendar
implementations, removal of the Boost dependency and a command-based CLI
([#1](https://github.com/shinuexx/calendrical/pull/1)), Doxygen documentation
([#2](https://github.com/shinuexx/calendrical/pull/2)), and holiday providers for the Coptic,
Islamic, Hebrew, Bahá'í, Samaritan, Hindu, Persian, Tibetan and Armenian calendars.
