# Bahá’í Holidays in a Gregorian Year

Provider: `holiday_providers::bahai_based_calendar_holiday_provider`.
It uses `calendars::bahai_calendar` (classic) and
`calendars::astro_bahai_calendar` (astronomical) backends.
[Status legend and source policy](README.md).

## Holiday status

All functions below take Gregorian `gyear`, return a single RD date and are
already included by `in_year()`. They are not a future “Batch A”.

| Holiday | Status | Provider function | Rule in code |
| --- | --- | --- | --- |
| Classic New Year | **implemented** | `new_year` | Gregorian March 21 |
| Astronomical Naw-Rúz | **implemented** | `naw_ruz` | `m_astro->new_year_on_or_before(gregorian.get_new_year(gyear + 1))` |
| Riḍván, day 1 | **implemented** | `feast_of_ridvan` | `naw_ruz(gyear) + 31` |
| Riḍván, day 9 | **implemented** | `ridvan_9` | Astronomical date 3 / 2 (Jamal) |
| Riḍván, day 12 | **implemented** | `ridvan_12` | 3 / 5 (Jamal) |
| Declaration of the Bāb | **implemented** | `declaration_of_the_bab` | 4 / 8 (Azamat) |
| Ascension of Bahā’u’llāh | **implemented** | `ascension_of_bahaullah` | 4 / 13 (Azamat) |
| Martyrdom of the Bāb | **implemented** | `martyrdom_of_the_bab` | 6 / 17 (Rahmat) |
| Birth of the Bāb | **implemented** | `birth_of_the_bab` | New moon after the Naw-Rúz anchor and comparison with sunset |
| Birth of Bahā’u’llāh | **implemented** | `birth_of_bahaullah` | `birth_of_the_bab(gyear) + 1` |
| Day of the Covenant | **implemented** | `day_of_the_covenant` | 14 / 4 (Qawl) |
| Ascension of ‘Abdu’l-Bahá | **implemented** | `ascension_of_abdul_baha` | 14 / 6 (Qawl) |

**implemented**: the private `bahai_date_in_gregorian_year(gyear, month, day)`
takes the `naw_ruz()` date, recovers `major`, `cycle`, `year` using the astronomical
backend, and converts the full `bahai_date{major, cycle, year, month, day}`.
It does not use the shortened three-field constructor from the original pseudocode.

**implemented**: `calendar()` without an argument returns the classic backend,
while `calendar(bool astro)` selects a backend. `in_year()` includes both classic
`new_year()` and astronomical `naw_ruz()`; this is not a switch between two complete
historical holiday sets.

## Birthday rule in the current implementation

Pseudocode of the **implemented** `birth_of_the_bab()` function:

```text
ny = naw_ruz(gyear)
set1 = astro.bahai_sunset(ny)
m1 = new_moon_on_or_after(set1)
m8 = new_moon_on_or_after(m1 + 190 days)
day = floor_to_rd(m8)
set8 = astro.bahai_sunset(day)
return day + (m8 < set8 ? 1 : 2)
```

`birth_of_bahaullah()` adds one day to this result, so it uses the same anchor rule.
**needs decision**: further confirmation of sunset boundaries and the interpretation
of the eighth month against a specific source for the tradition; the existence of a
function and test does not confirm all practices.

## Months and names

**implemented**: `bahai_date` has numeric months 1–19; intercalation is encoded as
`bahai_based_calendar::ayyam_i_ha == 0`. Ordinary months have 19 days;
conversion of month 19 and intercalary days depends on the backend.

**needs decision**: full transliterations / translations from the original text excerpt.
The list below preserves working names; it is not a parser vocabulary.

| Code | Working name | Code | Working name |
| --- | --- | --- | --- |
| 1 | Bahā’ | 11 | Mashīyyat |
| 2 | Jalāl | 12 | ‘Ilm |
| 3 | Jamāl | 13 | Qudrat |
| 4 | ‘Aẓamat | 14 | Qawl |
| 5 | Nūr | 15 | Masā’il |
| 6 | Raḥmat | 16 | Sharaf |
| 7 | Kalimāt | 17 | Sulṭān |
| 8 | Kamāl | 18 | Mulk |
| 9 | Asmā’ | 19 | ‘Alā’ |
| 10 | ‘Izzat | 0 | Ayyām-i-Hā |

**proposed**: named month aliases once spelling is agreed.
The current `bahai_date` parser parses the month as a number.

## Historical variants and observance series

The entries below preserve questions from the original notes. Do not treat these dates
as an approved historical specification: the original text excerpt is unavailable.

| Observance / idea | Status | Scope to verify |
| --- | --- | --- |
| Classic holiday set beyond New Year | **proposed** | Separate mode or `classic_*` API; does not exist today |
| Classic Birth of the Bāb / Birth of Bahā’u’llāh | **needs decision** | Working dates from the notes: October 20 / November 12 |
| Classic Riḍván: days 1, 9, 12 | **needs decision** | Working dates: April 21, April 29, May 2 |
| Classic Declaration of the Bāb | **needs decision** | Working date: May 24 |
| Classic Ascension of Bahā’u’llāh | **needs decision** | Working date: May 29 |
| Classic Martyrdom of the Bāb | **needs decision** | Working date: July 10 |
| Classic Day of the Covenant / Ascension of ‘Abdu’l-Bahá | **needs decision** | Working dates: November 26 / 28 |
| Separate helper for day 4 of Ayyām-i-Hā | **proposed** | No provider method |
| Historical rule for day 4 of Ayyām-i-Hā | **needs decision** | The note gave March 1 / February 29; verify backend and leap year before adopting it |
| Nineteen Day Feasts — month starts | **proposed** | List of occurrences within a Gregorian year |
| Fast days — all days of month ‘Alā’ | **proposed** | Separate list, not a single holiday |
| Including these series in `in_year()` | **needs decision** | Optionality and list scope; do not expand it automatically |

## Verifiable references

- [Provider API](../core/inc/holiday_providers/bahai_based_calendar_holiday_provider.hpp)
  and [implementation](../core/src/holiday_providers/bahai_based_calendar_holiday_provider.cpp).
- [Shared calendar API](../core/inc/calendars/bahai_based_calendar.hpp),
  [classic conversion](../core/src/calendars/bahai_calendar.cpp),
  [astronomical conversion and sunset](../core/src/calendars/astro_bahai_calendar.cpp),
  [parser](../core/inc/calendar_traits.hpp).
- [Tests](../tests/src/holidays/bahai_holidays.cc): CSV comparisons for `new_year`,
  `naw_ruz`, `feast_of_ridvan`, `birth_of_the_bab`; `in_year` checks a nonempty, sorted
  list in 2000–2010. The other dates have no separate reference comparison here.
- [CSV](../all-holidays.csv): CC metadata for Bahá’í New Year `(16.10), 277`,
  Naw-Rúz `(16.11), 277`, Riḍván `(16.12), 277`, Birth of the Bāb `(16.13), 278`.
