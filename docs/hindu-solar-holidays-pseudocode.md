# Holidays of the Modern Hindu Solar Calendar

Provider: `holiday_providers::modern_hindu_solar_calendar_holiday_provider`,
backend: `calendars::modern_hindu_solar_calendar`.
[Status legend and source policy](README.md).

## Rule status

| Holiday / rule | Status | Function | Current behavior |
| --- | --- | --- | --- |
| Solar New Year / Sowramana Ugadi | **implemented** | `sowramana_ugadi(gyear)` | `to_fixed({gyear - 78, 1, 1})`, Mesha 1 |
| Ayyappa Jyothi Darshanam (Pongal in the code comment) | **implemented** | `ayyappa_jyothi_darshanam(gyear)` | Makara 1: `to_fixed({gyear - 79, 10, 1})` and the preceding day |
| Mapping month/day into a Gregorian year | **implemented** | `in_gregorian(smonth, sday, gyear)` | Years `y-1`, `y`, `y+1` and range filtering |
| Holiday list | **implemented** | `in_year(gyear)` | `sowramana ugadi` and both days of `ayyappa jyothi darshanam` |
| Solar ingress into Mesha | **implemented** | `modern_hindu_base::mesha_samkranti(gyear)` | Returns a moment; not a separate entry in this provider |
| Additional solar observances | **proposed** | No new API agreed | Require a source and convention |

The names Mesha and Makara and the era offsets come from current comments and formulas.
Holiday methods convert backend dates; they do not calculate a new equinox or
solstice moment in the provider.

## Pseudocode of existing functions

```text
# implemented
sowramana_ugadi(gyear):
  return solar.to_fixed({gyear - 78, 1, 1})

ayyappa_jyothi_darshanam(gyear):
  pongal_date = solar.to_fixed({gyear - 79, 10, 1})
  return [pongal_date - 1, pongal_date]
```

`pongal_date` is a local variable, not a public `pongal()` function.
`in_gregorian()` filters by year, whereas these two functions use the given era offsets directly.

## Open decisions

- **needs decision**: identifying these observances with “the day after the equinox”
  or “the winter solstice” as a strict physical rule. These descriptions came from
  an unverifiable text excerpt; the current backend is sidereal.
- **needs decision**: the extent to which Pongal is identified with Ayyappa Jyothi
  Darshanam, and regional variants. This document describes names used in the code,
  not a resolution of traditions.
- **proposed**: reference tests for both holidays using independent data.

## Verifiable references

- [Provider API](../core/inc/holiday_providers/modern_hindu_solar_calendar_holiday_provider.hpp)
  and [implementation](../core/src/holiday_providers/modern_hindu_solar_calendar_holiday_provider.cpp).
- [Backend API](../core/inc/calendars/modern_hindu_solar_calendar.hpp),
  [calendar conversion](../core/src/calendars/modern_hindu_solar_calendar.cpp),
  [base calculations](../core/src/calendars/modern_hindu_based.cpp).
- [Tests](../tests/src/holidays/hindu_holidays.cc): `solar_observances` checks
  nonzero / nonempty results in 2000–2010; `solar_in_year` checks sorting and a nonempty list.
  A separate CSV test covers `mesha_samkranti()`, not `sowramana_ugadi()`.
- [CSV](../all-holidays.csv): `Mesha Saṃkrānti (date)` has CC metadata `(20.51), 364`.
