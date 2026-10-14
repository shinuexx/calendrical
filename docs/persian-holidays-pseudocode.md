# Persian Calendar Holidays

Provider: `holiday_providers::persian_based_calendar_holiday_provider`.
Backend: `calendars::persian_based_calendar`, with the astronomical
`persian_calendar` and arithmetic `arithmetic_persian_calendar` variants.
[Status legend and source policy](README.md).

## Months

**implemented**: months 1–6 have 31 days each, 7–11 have 30, and month 12 has
29 or 30 days. New Year and leap-year rules depend on the backend.
Months in `simple_date` and its parser are numeric.

**needs decision**: spelling and aliases for the working vocabulary below from
the original notes. **proposed**: a name parser after that vocabulary has been agreed.

| Number | Working name | Days |
| --- | --- | --- |
| 1 | Farvardin | 31 |
| 2 | Ordibehesht | 31 |
| 3 | Khordad | 31 |
| 4 | Tir | 31 |
| 5 | Mordad | 31 |
| 6 | Shahrivar | 31 |
| 7 | Mehr | 30 |
| 8 | Aban | 30 |
| 9 | Azar | 30 |
| 10 | Dey | 30 |
| 11 | Bahman | 30 |
| 12 | Esfand | 29 or 30 |

## Holiday and rule status

All functions below already exist and are used by `in_year(gyear)`.

| Holiday / rule | Status | Function | Current behavior |
| --- | --- | --- | --- |
| Nowruz | **implemented** | `nawruz` | `get_new_year(pyear)`; single result |
| Sizdah Bedar | **implemented** | `sizdah_bedar` | List for 1 / 13 |
| Chaharshanbe Suri | **implemented** | `chaharshanbe_suri` | `weekday::wednesday.before(nawruz(gyear))`; strictly before |
| Tirgan | **implemented** | `tirgan` | List for 4 / 13 |
| Mehregan | **implemented** | `mehregan` | List for 7 / 16 |
| Yalda Night | **implemented** | `yalda_night` | List of civil dates 9 / 30 |
| Sepandarmazgan | **implemented** | `sepandarmazgan` | List for 12 / 5 |
| Month/day mapping | **implemented** | `in_gregorian` | Conversion of years `y-1`, `y`, `y+1` and filtering by Gregorian year |

The API spelling is `nawruz`, despite the name “Nowruz” in the CSV. New Year is calculated as:

```text
# implemented in nawruz(gyear)
pyear = gyear - GregorianYear(PersianEpoch) + 1
if pyear <= 0: pyear = pyear - 1
return persian.get_new_year(pyear)
```

This is the year offset corrected in PR #3; do not use the old formula without `+1`.
Do not replace the results with approximate fixed Gregorian dates.

## Conventions requiring decisions

- **needs decision**: external confirmation of the evening convention for Yalda Night
  and Chaharshanbe Suri, and any model of their start times.
  The header describes Yalda as the civil date Azar 30 beginning the preceding
  evening, and Chaharshanbe Suri as the Wednesday before Nowruz; the provider returns only RD.
- **needs decision**: tradition variants and the choice of astronomical / arithmetic
  backend for new datasets. The CLI and current fixture use `persian_calendar`.
- **proposed**: independent references for the other holidays and separate tests of variants.

## Verifiable references

- [Provider API](../core/inc/holiday_providers/persian_based_calendar_holiday_provider.hpp)
  and [implementation](../core/src/holiday_providers/persian_based_calendar_holiday_provider.cpp).
- [Backend API](../core/inc/calendars/persian_based_calendar.hpp),
  [shared rules](../core/src/calendars/persian_based_calendar.cpp),
  [astronomy](../core/src/calendars/persian_calendar.cpp),
  [arithmetic variant](../core/src/calendars/arithmetic_persian_calendar.cpp).
- [Tests](../tests/src/holidays/persian_holidays.cc): CSV comparison for `nawruz()`,
  nonempty lists for the other fixed holidays and sorting in 2000–2010.
  The sorting test does not independently confirm the Chaharshanbe Suri rule.
- [CSV](../all-holidays.csv): `Nowruz` has CC metadata `(15.11), 265`.
