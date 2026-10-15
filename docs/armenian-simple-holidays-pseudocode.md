# Holidays of the Classical Armenian Calendar

Scope: `calendars::armenian_calendar`, an instance of `simple_calendar` with a
365-day cycle (12 months of 30 days plus 5 additional days). This is not the modern
Armenian liturgical calendar. [Status legend and source policy](README.md).

## Rule status

| Holiday or rule | Status | Implementation / scope |
| --- | --- | --- |
| Navasard, month 1 / day 1 | **implemented** | `simple_calendar_holiday_provider::navasard(gyear)` → `in_gregorian(1, 1, gyear)` |
| Mapping dates into a Gregorian year | **implemented** | `in_gregorian(cmonth, cday, gyear)` converts years `y-1`, `y`, `y+1` and filters by the year range |
| Holiday list | **implemented** | `in_year(gyear)` adds the label `navasard` and sorts by date |
| Restricting Navasard to the Armenian instance | **needs decision** | The original notes proposed an instance-identity check; the current code does not perform it |
| Separate provider for Armenian liturgical holidays | **proposed** | Outside the fixed civil-year model; requires a separate source of rules |
| Easter, Ascension and nearest-Sunday rules in that provider | **needs decision** | These notes do not establish a holiday set or conventions |

## Current behavior

The provider is generic: `navasard()` works with any supplied `simple_calendar`.
The CLI registers it as both `armenian` and `egyptian`; both entries receive the
label `navasard`. This describes the implementation, not historical confirmation
of the holiday name in every calendar. Introducing a restriction requires a separate decision.

Pseudocode for the **implemented** rule:

```text
in_year(gyear):
  result = [("navasard", d) for d in navasard(gyear)]
  sort_by_date(result)
  return result
```

## Verifiable references

- [Provider API](../core/inc/holiday_providers/simple_calendar_holiday_provider.hpp)
  and [implementation](../core/src/holiday_providers/simple_calendar_holiday_provider.cpp).
- [Calendar API](../core/inc/calendars/simple_calendar.hpp)
  and [implementation / epochs](../core/src/calendars/simple_calendar.cpp).
- [CLI registration](../cli/src/holiday_provider_provider_builder.cpp).
- [Tests](../tests/src/holidays/samaritan_simple_holidays.cc): `simple_holidays_test`
  checks nonempty results and sorting for the Armenian instance in 2000–2010.
  This is not independent validation of holiday dates or the Egyptian instance's behavior.
