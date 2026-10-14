# Tibetan Calendar Holidays

The provider `holiday_providers::tibetan_calendar_holiday_provider` already exists.
Its backend is `calendars::tibetan_calendar`; the date type is
`lunar_date{year, month, leap_month, day, leap_day}`. The CLI registers the provider as `tibetan`.
[Status legend and source policy](README.md).

## Implemented holidays

All functions take Gregorian `gyear`, return a list of RD dates and are included in `in_year()`.

| Holiday | Status | Provider function | Rule |
| --- | --- | --- | --- |
| Losar | **implemented** | `losar` | Delegates to `tibetan_calendar::new_year(gyear)` |
| Monlam Chenmo | **implemented** | `monlam_chenmo` | Month 1, days 4–11 |
| Chotrul Duchen | **implemented** | `chotrul_duchen` | 1 / 15 |
| Saga Dawa Duchen | **implemented** | `saga_dawa_duchen` | 4 / 15 |
| Chokhor Duchen | **implemented** | `chokhor_duchen` | 6 / 4 |
| Lhabab Duchen | **implemented** | `lhabab_duchen` | 9 / 22 |
| Ganden Ngamcho | **implemented** | `ganden_ngamcho` | 10 / 25 |

## Mapping and repeated dates

**implemented**: `in_gregorian(tmonth, tday, gyear)` uses the convention
`leap_month=false`, `leap_day=false`, converts years `tyear-1` through `tyear+2`
inclusive, filters by the Gregorian range, sorts and removes duplicates.
The private `fixed_range_in_gregorian()` combines lists for consecutive day numbers.

```text
# implemented in in_gregorian
range = gregorian.get_year_range(gyear)
tyear = tibetan.to_date(range.min).year
candidates = [tibetan.to_fixed({y, month, false, day, false})
              for y in tyear-1 .. tyear+2]
return sorted_unique(filter(range, candidates))
```

**implemented** — Losar exception: the calendar's `losar(tyear)` chooses
`is_leap_month(tyear, 1)`; `new_year(gyear)` considers two adjacent years and filters
the results. Do not describe Losar as ordinary `in_gregorian(1, 1, gyear)` with a `false` flag.

## Open decisions

- **needs decision**: the intended observance rules for leap months, repeated days
  and omitted days. Current flags and conversion describe the implementation,
  but do not confirm all traditions.
- **needs decision**: whether an extended API should return alternative occurrences,
  and how to interpret the Monlam Chenmo range when days are omitted.
- **needs decision**: observances starting at a particular time; current results are
  civil date labels, not start times.
- **proposed**: references for holidays other than Losar and tests of the leap month/day policy.

## Verifiable references

- [Provider API](../core/inc/holiday_providers/tibetan_calendar_holiday_provider.hpp)
  and [implementation](../core/src/holiday_providers/tibetan_calendar_holiday_provider.cpp).
- [Calendar API](../core/inc/calendars/tibetan_calendar.hpp)
  and [conversion / Losar rule](../core/src/calendars/tibetan_calendar.cpp).
- [CLI registration](../cli/src/holiday_provider_provider_builder.cpp).
- [Tests](../tests/src/holidays/tibetan_holidays.cc): Losar is compared with the CSV;
  other functions are checked for nonempty results in 2000–2010 and the list for sorting.
- [CSV](../all-holidays.csv): `Losar` has CC metadata `(21.9), 381`.
