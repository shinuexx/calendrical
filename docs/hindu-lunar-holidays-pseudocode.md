# Holidays of the Modern Hindu Lunar Calendar

Provider: `holiday_providers::modern_hindu_lunar_calendar_holiday_provider`.
Backend: `calendars::modern_hindu_lunar_calendar`; the date type is
`lunar_date{year, month, leap_month, day, leap_day}`.
[Status legend and source policy](README.md).

## Holiday and rule status

All listed methods already exist. The `gyear` argument is Gregorian, and holiday
results are lists of RD dates. `in_year()` adds every holiday in the table except
a separate `diwali()` entry: the `diwali` label uses `diwali_days()`.

| Holiday | Status | Function | Current rule |
| --- | --- | --- | --- |
| Rama Navami | **implemented** | `rama` | `lunar_event(1, 9, moment_t{0.5}, gyear)` |
| Maha Shivaratri | **implemented** | `shiva` | `lunar_event(11, 29, moment_t{1}, gyear)` |
| Sacred Wednesdays | **implemented** | `sacred_wednesdays` | Wednesdays whose lunar day is 8 |
| Diwali, main day | **implemented** | `diwali` | `lunar_holiday(8, 1, gyear)` |
| Diwali, set of days | **implemented** | `diwali_days` | Union of `lunar_holiday(7, 29)`, `lunar_holiday(7, 30)` and `diwali`; sorting and deduplication |
| Janmashtami | **implemented** | `janmashtami` | `lunar_holiday(5, 23, gyear)` |
| Ganesha Chaturthi | **implemented** | `ganesha_chaturthi` | 6 / 4 |
| Durga Ashtami | **implemented** | `durga_ashtami` | 7 / 8 |
| Sarasvati Puja | **implemented** | `sarasvati_puja` | 7 / 9 |
| Dasra | **implemented** | `dasra` | 7 / 10 |
| Karthikai Deepam | **implemented** | `karthikai_deepam` | 8 / 15 |
| Vaikunta Ekadashi | **implemented** | `vaikunta_ekadashi` | 9 / 11 |
| Buddha’s Birthday | **implemented** | `buddha_birthday` | Simplified convention `lunar_holiday(2, 15, gyear)` |
| Varalakshmi Vratam | **implemented** | `varalakshmi_vratam` | Friday strictly before the date from `lunar_holiday(5, 15, gyear)` |
| Holi | **implemented** | `holi` | `lunar_event(12, 15, moment_t{0.75}, gyear)`; evening approximation |

## Months and day availability

**implemented**: functions and code comments use the following mapping:
1 Caitra, 2 Vaishakha, 5 Shravana, 6 Bhadrapada, 7 Ashvina, 8 Kartika,
9 Margashirsha, 11 Magha, 12 Phalguna. These are numbers in the API, not a month enum.
**needs decision**: canonical transliterations and parser aliases; this list does
not approve a complete vocabulary for all traditions.

**implemented**: `date_occur(lyear, lmonth, lday)` initially constructs a date with
`leap_month=false` and `leap_day=false`. It checks month and day consistency after
conversion and handles an expunged month or absent day. Thus `lunar_holiday()` is
not simply a lookup of a literal existing date.
`in_gregorian()` delegates to `lunar_holiday()`: two adjacent lunar years,
calculation via `date_occur()` and filtering by the Gregorian range.

**implemented**: `tithi_occur(lmonth, tithi, t, lyear)` finds the start of a tithi
and compares the phase against the time produced by `standard_from_sundial()` for
Ujjain. `lunar_event()` filters the results of two years. Parameters `0.5`, `0.75`,
`1` describe the current algorithm, not an arbitrary regional start time for a holiday.

**implemented**: `sacred_wednesdays_in_range(range)` enumerates Wednesdays in the
range and selects lunar day 8; `karana(n)` is a separate index helper, not a holiday
added to `in_year()`.

## Open decisions from the original notes

| Topic | Status | Current code and future question |
| --- | --- | --- |
| “Friday prior to Shravana 15” | **needs decision** | The code uses `weekday::friday.before(d)` (strictly before); confirmation of the source interpretation remains open |
| Multi-day Diwali | **needs decision** | The code combines three requested dates through `date_occur`; it does not separately test whether “Ashvina 30 exists” |
| Diwali: shared label or named sub-days | **needs decision** | Current `in_year()` labels all dates `diwali` |
| Buddha’s Birthday | **needs decision** | Vaiśākha 15 is a simplification; no tradition configuration |
| Holi: precise time / location | **needs decision** | The current method uses the `0.75` approximation and the Ujjain helper |
| Fasting day preceding Maha Shivaratri | **proposed** | No separate provider method / entry; the exact convention requires a source |
| Leap and expunged month/day policies for individual holidays | **needs decision** | Do not decide them solely from the generic helper |
| Tests of full series and regional variants | **proposed** | Separate references, especially for the extended set |

Existing functions are no longer “H-L1..H-L4” tasks. Open decisions do not change
their current implementation status and are not resolved here.

## Verifiable references

- [Provider API](../core/inc/holiday_providers/modern_hindu_lunar_calendar_holiday_provider.hpp)
  and [implementation](../core/src/holiday_providers/modern_hindu_lunar_calendar_holiday_provider.cpp).
- [Backend API](../core/inc/calendars/modern_hindu_lunar_calendar.hpp),
  [conversion](../core/src/calendars/modern_hindu_lunar_calendar.cpp),
  [base rules / Hindu Lunar New Year](../core/src/calendars/modern_hindu_based.cpp).
- [Tests](../tests/src/holidays/hindu_holidays.cc): CSV comparisons for `diwali`,
  Sacred Wednesday, `shiva`, `rama` and the calendar's `hindu_lunar_new_year`;
  `lunar_in_year` checks only a nonempty, sorted list in 2000–2010.
- [CSV](../all-holidays.csv): CC metadata Diwali `(20.57), 368`, Shiva `(20.60), 369`,
  Rama `(20.61), 369`, Sacred Wednesday `(20.65), 370`, Hindu Lunar New Year
  `(20.53), 365`. The [loader](../tests/inc/holiday_csv_loader.hpp) compares the first
  element of a vector result; the `diwali()` test does not validate all of `diwali_days()`.
