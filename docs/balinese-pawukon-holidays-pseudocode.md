# Balinese Pawukon Calendar and Holidays

Scope: `calendars::pawukon_calendar`, the 210-day Balinese Pawukon cycle of
*Calendrical Calculations* (CC), chapter 12, and
`holiday_providers::pawukon_calendar_holiday_provider` with Kajeng Keliwon and
Tumpek. The Balinese lunisolar Saka calendar is out of scope.
[Status legend and source policy](README.md).

## Rule status

| Holiday or rule | Status | Implementation / scope |
| --- | --- | --- |
| Epoch `bali-epoch` = fixed date of JD 146 = RD −1721279 | **implemented** | `pawukon_calendar::epoch()` |
| Position in the cycle, `bali-day-from-fixed` | **implemented** | `pawukon_calendar::day_from_fixed(date)` |
| Ten concurrent weeks, `bali-pawukon-from-fixed` | **implemented** | `pawukon_calendar::to_date(date)` → `pawukon_date::from_day(date - epoch)` |
| Last occurrence on or before a date, `bali-on-or-before` | **implemented** | `pawukon_calendar::on_or_before(date, reference)` |
| Occurrences in a range, `positions-in-range` | **implemented** | `pawukon_calendar::positions_in_range(day, period, range)` and `in_range(date, range)` |
| Kajeng Keliwon (triwara 3 and pancawara 5, every 15 days) | **implemented** | `kajeng_keliwon(gyear)` → `positions_in_range(8, 15, year)` |
| Tumpek (pancawara 5 and saptawara 7, every 35 days) | **implemented** | `tumpek(gyear)` → `positions_in_range(13, 35, year)` |
| Holiday list | **implemented** | `in_year(gyear)` returns every occurrence with labels `kajeng keliwon` and `tumpek`, sorted |
| Galungan, Kuningan and other named Pawukon days | **proposed** | Not in CC's holiday list; they are fixed positions in the cycle and could use `in_range` |
| Date conversion in the CLI | **needs decision** | A Pawukon date needs a reference date, as for Akan; only the holiday provider is registered |

## Date type

`pawukon_date` stores all ten components, as CC's `balinese-date` does:
luang (boolean), dwiwara 1–2, triwara 1–3, caturwara 1–4, pancawara 1–5,
sadwara 1–6, saptawara 1–7, asatawara 1–8, sangawara 1–9 and dasawara 0–9.
Only 210 combinations occur. Pancawara, sadwara and saptawara alone determine
the position in the cycle (`day()`), so a date is valid when every component is
in range and the date equals `from_day(day())`.

Pseudocode for the **implemented** rules (`d` is the position in the cycle, 0–209):

```text
from_day(d):
  pancawara = amod(d + 2, 5)
  saptawara = (d mod 7) + 1
  dasawara  = (1 + [5, 9, 7, 4, 8][pancawara - 1] + [5, 4, 3, 7, 8, 6, 9][saptawara - 1]) mod 10
  asatawara = (max(6, 4 + ((d - 70) mod 210)) mod 8) + 1
  sangawara = (max(0, d - 3) mod 9) + 1
  return (luang = dasawara is even, dwiwara = amod(dasawara, 2), triwara = (d mod 3) + 1,
          caturwara = amod(asatawara, 4), pancawara, sadwara = (d mod 6) + 1,
          saptawara, asatawara, sangawara, dasawara)

day(date):
  a5 = pancawara - 1; a6 = sadwara - 1; b7 = saptawara - 1
  b35 = (a5 + 14 + 15 * (b7 - a5)) mod 35
  return (a6 + 36 * (b35 - a6)) mod 210

on_or_before(date, reference):
  return reference - ((reference - epoch - day(date)) mod 210)

positions_in_range(day, period, range):     # period divides 210
  first = range.min + ((day - (range.min - epoch)) mod period)
  return [first, first + period, ...] filtered by range

in_year(gyear):
  year = gregorian_year_range(gyear)
  result = [("kajeng keliwon", x) for x in positions_in_range(8, 15, year)]
         + [("tumpek", x) for x in positions_in_range(13, 35, year)]
  sort(result)
  return result
```

## Verifiable references

- CC reference code `calendar.l` (CALENDRICA 4.0), section "Balinese Calendar":
  `bali-epoch`, `bali-pawukon-from-fixed`, `bali-on-or-before`, `kajeng-keliwon`
  and `tumpek`.
- [Date type](../core/inc/datetypes/pawukon_date.hpp),
  [calendar API](../core/inc/calendars/pawukon_calendar.hpp) and
  [implementation](../core/src/calendars/pawukon_calendar.cpp).
- [Provider API](../core/inc/holiday_providers/pawukon_calendar_holiday_provider.hpp)
  and [implementation](../core/src/holiday_providers/pawukon_calendar_holiday_provider.cpp).
- [CLI registration](../cli/src/holiday_provider_provider_builder.cpp) as `pawukon`.
- [Calendar tests](../tests/src/calendars/pawukon_calendar.cc) compare the 33 CC
  sample dates against values computed with the CC reference code, and check the
  full cycle, periodicity, negative RD, `on_or_before`, ranges and validation.
  Galungan 2024 (28 February and 25 September) is checked as Buda Keliwon of
  week 11, an independent check of the epoch.
- [Holiday tests](../tests/src/holidays/pawukon_holidays.cc) compare the first
  occurrence with `Kajeng Keliwon (first)` and `Tumpek (first)` in
  `all-holidays.csv` for 2000–2103, and check that every occurrence in a year is
  returned. [CLI tests](../tests/src/cli_pawukon.cc) cover the registration.
