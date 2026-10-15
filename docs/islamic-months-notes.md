# Islamic Months and Holidays

Provider: `holiday_providers::islamic_based_calendar_holiday_provider`.
It accepts any `calendars::islamic_based_calendar`; the CLI binds `islamic_based`
to the tabular `calendars::islamic_calendar`.
[Status legend and source policy](README.md).

## Months and calendar variants

**implemented**: in the tabular `islamic_calendar`, odd-numbered months have
30 days, even-numbered months have 29, and month 12 has 30 days in a leap year.
`is_leap_year(year)` checks `mod(14 + 11 * year, 30) < 11`.
Do not apply this month-length table to observational backends.

**needs decision**: the names below are a working vocabulary from the original notes;
they do not implement aliases or establish a verified transliteration specification.

| Number | Working name | Days in the tabular backend |
| --- | --- | --- |
| 1 | Muḥarram | 30 |
| 2 | Ṣafar | 29 |
| 3 | Rabī‘ I / Rabī‘ al-Awwal | 30 |
| 4 | Rabī‘ II / Rabī‘ al-Āḥir | 29 |
| 5 | Jumādā I / Jumādā al-Ūlā | 30 |
| 6 | Jumādā II / Jumādā al-Āḥira | 29 |
| 7 | Rajab | 30 |
| 8 | Sha‘bān | 29 |
| 9 | Ramaḍān | 30 |
| 10 | Shawwāl | 29 |
| 11 | Dhu al-Qa‘da | 30 |
| 12 | Dhu al-Ḥijja | 29 or 30 |

**proposed**: a month-name parser. **needs decision**: output spelling,
ASCII aliases, diacritics and apostrophe variants. The current `simple_date` parser uses numbers.

## Holidays in the current provider

All functions take Gregorian `gyear`, return `std::vector<fixed_date_t>`
and are included in `in_year()`.

| Holiday | Status | Function | Numeric rule |
| --- | --- | --- | --- |
| Islamic New Year | **implemented** | `islamic_new_year` | `in_gregorian(1, 1, gyear)` |
| ‘Ashūrā’ | **implemented** | `ashura` | 1 / 10 |
| Mawlid | **implemented** | `mawlid` | 3 / 12 |
| Lailat al-Mi‘rāj | **implemented** | `lailat_al_miraj` | 7 / 27 |
| Lailat al-Barā’a | **implemented** | `lailat_al_baraa` | 8 / 15 |
| Ramadan start | **implemented** | `ramadan` | 9 / 1 |
| Lailat al-Qadr | **implemented** | `lailat_al_qadr` | 9 / 27 |
| Eid ul-Fitr | **implemented** | `eid_ul_fitr` | 10 / 1 |
| Eid ul-Adha | **implemented** | `eid_ul_adha` | 12 / 10 |

**implemented**: `in_gregorian(imonth, iday, gyear)` determines the Islamic year
at the start of the Gregorian range, converts `y-1`, `y`, `y+1` and filters the results.
The result is a list of occurrences, not a promise of one date per Gregorian year.

## Open decisions

- **needs decision**: observance variants depending on tradition, country or local
  observation / proclamation. The current month/day constants describe the code, not all practices.
- **needs decision**: modeling the start of an observance on the preceding evening.
  The provider returns civil RD labels; it does not determine the holiday's start time.
- **proposed**: optional holiday-variant configuration, once sources have been identified.

## Verifiable references

- [Provider API](../core/inc/holiday_providers/islamic_based_calendar_holiday_provider.hpp)
  and [implementation](../core/src/holiday_providers/islamic_based_calendar_holiday_provider.cpp).
- [Backend interface](../core/inc/calendars/islamic_based_calendar.hpp),
  [tabular backend](../core/src/calendars/islamic_calendar.cpp),
  [observational backend](../core/inc/calendars/observational_islamic_calendar.hpp),
  [parser](../core/inc/calendar_traits.hpp).
- [Tests](../tests/src/holidays/islamic_holidays.cc): CSV comparison for `mawlid()`,
  nonempty results for the other holidays and sorting in 2000–2010; they use the tabular backend.
- [CSV](../all-holidays.csv): `Mawlid` has CC metadata `(7.6), 109`.
