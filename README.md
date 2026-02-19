# calendrical

Biblioteka C++20 do obliczeń kalendarzowych i astronomicznych (wiele systemów kalendarzowych + providery świąt), z binarką `calendrical_cli` i testami opartymi o GoogleTest.

## Krok 1: Wymagania

- CMake >= 3.10
- Kompilator C++20 (`g++`, `clang++` lub MSVC)
- `vcpkg` (do zależności)
- Generator builda (np. `Ninja`)

Zależności projektu (manifest `vcpkg.json`):
- `gtest`

## Krok 2: Konfiguracja środowiska

1. Ustaw `VCPKG_ROOT` na katalog z `vcpkg`.
2. Upewnij się, że masz dostępny generator builda (np. `ninja --version`).

Przykład (Linux/macOS):

```bash
export VCPKG_ROOT=/sciezka/do/vcpkg
```

## Krok 3: Konfiguracja CMake

Najprościej przez preset:

```bash
cmake --preset "Configure preset using toolchain file"
```

Alternatywnie ręcznie (często wygodniejsze w CI/skryptach):

```bash
cmake -S . -B cmake-build-debug \
  -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
```

## Krok 4: Build

```bash
cmake --build cmake-build-debug -j4
```

Po buildzie dostępne są m.in.:
- `cmake-build-debug/core/liblibcalendrical.a`
- `cmake-build-debug/cli/calendrical_cli`
- `cmake-build-debug/tests/tests`

## Krok 5: Testy

Uruchom testy przez CTest (root build-dir):

```bash
ctest --test-dir cmake-build-debug --output-on-failure
```

Lub bezpośrednio binarkę testową:

```bash
./cmake-build-debug/tests/tests
```

Przegląd zarejestrowanych testów GTest:

```bash
./cmake-build-debug/tests/tests --gtest_list_tests
```

## Krok 6: Uruchomienie CLI

```bash
./cmake-build-debug/cli/calendrical_cli --help
```

CLI działa w modelu komend:

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

Aktualna składnia CLI:

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
```

`list [topic]`:
- `commands` (domyślnie)
- `calendars`
- `holiday-calendars`

Konwertery (`convert`) aktualnie zarejestrowane w kodzie:
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

Format `date` dla `convert` (zależnie od konwertera):
- `rd`, `mjd`: `N`
- `jd`: `N` lub `N.frac`
- `unix`, `unix_timestamp`: `seconds_since_epoch`
- kalendarze proste: `YEAR-MONTH-DAY`
- `gregorian`, `julian`, `hebrew`, `observational_hebrew`: `YEAR-MONTH-DAY` (MONTH jako nazwa lub numer)
- `iso`: `YEAR-WEEK-DAY`
- `icelandic`: `YEAR-SEASON-WEEK-WEEKDAY`
- `bahai`, `astro_bahai`: `MAJOR-CYCLE-YEAR-MONTH-DAY`
- `chinese`, `japanese`, `korean`, `vietnamese`: `CYCLE-YEAR-MONTH-LEAP_MONTH-DAY`
- `astro_hindu_lunar`, `modern_hindu_lunar`, `tibetan`: `YEAR-MONTH-LEAP_MONTH-DAY-LEAP_DAY`
- `old_hindu_lunar`, `babylonian`: `YEAR-MONTH-LEAP_MONTH-DAY`
- `roman`: `YEAR-MONTH-EVENT-COUNT-LEAP`

Providery (`holidays`) aktualnie zarejestrowane:
- `gregorian`
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

Aliasy providerów wspierane przez parser `holidays`:
- `gregorian_calendar` -> `gregorian`
- `julian_calendar` -> `julian`
- `islamic` / `islamic_based_calendar` -> `islamic_based`
- `hebrew` / `hebrew_based_calendar` -> `hebrew_based`
- `bahai` / `bahai_based_calendar` -> `bahai_based`
- `persian` / `persian_based_calendar` -> `persian_based`
- `modern_hindu_lunar_calendar` -> `modern_hindu_lunar`

## Krok 7: Użycie biblioteki w kodzie

Przykładowy minimalny kod:

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

Nagłówki publiczne są w `core/inc`, implementacje w `core/src`.

## Krok 8: Struktura projektu

- `core/`:
  - `inc/` publiczne API biblioteki
  - `src/` implementacje
- `cli/`: mały program demonstracyjny
- `tests/`: testy GTest
- `vcpkg.json`, `vcpkg-configuration.json`: zarządzanie zależnościami
- `CMakeLists.txt`: konfiguracja top-level

## Krok 9: Zakres funkcjonalny (skrót)

- Kalendarze: m.in. gregoriański, juliański, islamskie, hebrajskie, perskie, chińskie, hinduskie, bahaickie, tybetańskie, ISO itd.
- Astronomia czasu: m.in. wschody/zachody, długości ekliptyczne, fazy księżyca, korekty efemeryd.
- Providery świąt dla wybranych kalendarzy.

## Krok 10: Workflow developerski

1. Zmieniasz kod.
2. Build:
   ```bash
   cmake --build cmake-build-debug -j4
   ```
3. Testy:
   ```bash
   ctest --test-dir cmake-build-debug --output-on-failure
   ```
4. Sprawdzasz diff i robisz commit.

## Krok 11: Ważne decyzje API (aktualny stan)

- `calendar_provider` został usunięty (cel: uproszczenie API i własności obiektów).
- `holiday_t` porównuje obiekty tylko po `date`:
  - sortowanie i porządek są czysto chronologiczne,
  - święta o tej samej dacie są traktowane jako równe.
- `leap_day_aware_calendar` wymaga logicznego `is_leap_day(...)` (`bool`).

## Krok 12: Typowe problemy

- `vcpkg lock / permission denied` podczas konfiguracji:
  - zamknij procesy, które trzymają lock,
  - ponów `cmake -S . -B ...`.
- `ctest` nie znajduje testów:
  - uruchamiaj `ctest --test-dir cmake-build-debug --output-on-failure` po poprawnej konfiguracji/buildzie.
