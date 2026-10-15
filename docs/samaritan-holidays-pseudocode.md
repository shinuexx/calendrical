# Samaritan Calendar Holidays

The provider `holiday_providers::samaritan_calendar_holiday_provider` already
exists, and the CLI registers it as `samaritan`. The backend
`calendars::samaritan_calendar` uses `simple_date{year, month, day}` with numeric months.
[Status legend and source policy](README.md).

## Holiday status

All functions are used by `in_year(gyear)`; the argument is a Gregorian year.

| Holiday | Status | Exact function name | Result and rule |
| --- | --- | --- | --- |
| Passover | **implemented** | `passover` | List: month 1 / day 14 |
| Festival of the Unleavened Bread | **implemented** | `festival_of_unleavened_bread` | List: 1 / 15–21 |
| Festival of Pentecost | **implemented** | `festival_of_pentecost` | Single RD: first Sunday strictly after `passover(gyear).front()` + 49 days |
| Festival of the Seventh Month | **implemented** | `festival_of_the_seventh_month` | List: 7 / 1 |
| Day of Atonement | **implemented** | `day_of_atonement` | List: 7 / 10 |
| Festival of Tabernacles | **implemented** | `festival_of_tabernacles` | List: 7 / 15–21 |
| The Eighth Day | **implemented** | `eighth_day` | List: 7 / 22 |
| Ṣimmut of Passover | **implemented** | `simmut_of_passover` | Single RD: Saturday on or before the first Passover − 49 days |
| Ṣimmut of Tabernacles | **implemented** | `simmut_of_tabernacles` | Single RD: Saturday on or before the first Tabernacles − 49 days |

Do not use nonexistent shortened names from the old pseudocode (`unleavened_bread`,
`pentecost`, `seventh_month_festival`, `tabernacles`) as provider API names.

## Mapping and ranges

**implemented**: `in_gregorian(smonth, sday, gyear)` converts `y-1`, `y`, `y+1`,
where `y` comes from the backend at the start of the Gregorian range, then filters
results. The private `fixed_range_in_gregorian()` combines lists for each day number
in an inclusive range, sorts and deduplicates. Do not assume one occurrence per year
for every list-returning method.

Pseudocode of the **implemented** moveable rules:

```text
festival_of_pentecost(gyear):
  p = passover(gyear)
  if p is empty: throw runtime_error
  return sunday.after(p.front()) + 49 days

simmut_of_passover(gyear):
  p = passover(gyear)
  if p is empty: throw runtime_error
  return saturday.on_or_before(p.front() - 49 days)

simmut_of_tabernacles(gyear):
  t = festival_of_tabernacles(gyear)
  if t is empty: throw runtime_error
  return saturday.on_or_before(t.front() - 49 days)
```

Moveable methods use the first element of the anchor list. They do not filter the
calculated date again by Gregorian year; the description above does not promise such filtering.

## Open decisions

- **needs decision**: external confirmation of “8th Sunday after Passover” — the
  code takes the first Sunday strictly after, followed by seven more weeks.
- **needs decision**: Sabbath selection for Ṣimmut;
  `on_or_before(holiday - 49 days)` is existing behavior, not a resolution of every
  interpretation of the text.
- **needs decision**: moveable-method policy when the anchor list is empty or has
  multiple entries, and when the result lies outside the query year. Current types
  and exceptions are described above.
- **needs decision**: any preceding-evening start time; the provider returns civil
  RD dates, not moments.
- **proposed**: an independent reference dataset and tests of the specific counting rules.

## Verifiable references

- [Provider API](../core/inc/holiday_providers/samaritan_calendar_holiday_provider.hpp)
  and [implementation](../core/src/holiday_providers/samaritan_calendar_holiday_provider.cpp).
- [Backend API](../core/inc/calendars/samaritan_calendar.hpp)
  and [conversion](../core/src/calendars/samaritan_calendar.cpp).
- [CLI registration](../cli/src/holiday_provider_provider_builder.cpp).
- [Tests](../tests/src/holidays/samaritan_simple_holidays.cc): `samaritan_holidays_test`
  checks nonempty / nonzero function results and sorting in 2000–2010.
  It does not contain independent reference dates or settle the open conventions.
