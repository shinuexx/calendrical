# Hebrew Holidays in a Gregorian Year

Provider: `holiday_providers::hebrew_based_calendar_holiday_provider`.
It accepts `calendars::hebrew_based_calendar`; the CLI uses the arithmetic
`calendars::hebrew_calendar` as the `hebrew_based` backend.
[Status legend and source policy](README.md).

## Year and month names

**implemented**: the `year` / `gyear` argument of the holiday functions listed below
is Gregorian. Some header comments still say “Hebrew year”; the description below
follows the implementation. Exceptions: `birthday()` and `yahrzeit()` take a target
Hebrew year; their `_in_gregorian()` variants take a Gregorian year.

For `e = gregorian.get_year(hebrew.epoch())`, autumn rules use
`hyear = gyear - e + 1`, while spring / summer rules use `hyear = gyear - e`.
`in_gregorian(hmonth, hday, gyear)` converts years `y`, `y+1`, `y+2`, where `y`
is the backend year at the start of the Gregorian range, then filters the results.
These are distinct rules; do not replace all helpers with one offset.

**implemented**: API names are the `hebrew_month` elements `nisan`, `iyyar`,
`sivan`, `tammuz`, `av`, `elul`, `tishri`, `marheshvan`, `kislev`, `tevet`, `shevat`,
`adar`, `adarii`. Purim uses `last_month_of_year(hyear)`.

## Holidays and rules already in the provider

| Holiday | Status | Function | Rule in code |
| --- | --- | --- | --- |
| Rosh ha-Shanah | **implemented** | `rosh_ha_shanah` | Tishri 1, autumn year |
| Rosh ha-Shanah, day 2 | **implemented** | `rosh_ha_shanah_day_2` | First day + 1 |
| Yom Kippur | **implemented** | `yom_kippur` | Tishri 10, autumn year |
| Sukkot | **implemented** | `sukkot` | Tishri 15 |
| Sukkot, diaspora day 2 | **implemented** | `sukkot_day_2_diaspora` | Tishri 16 |
| Hoshana Rabba | **implemented** | `hoshana_rabba` | Tishri 21 |
| Shemini Atzeret | **implemented** | `shemini_atzeret` | Tishri 22 |
| Simhat Torah | **implemented** | `simhat_torah` | Tishri 23, diaspora convention |
| Passover | **implemented** | `passover` | Nisan 15, spring year |
| Passover, diaspora day 2 | **implemented** | `passover_day_2_diaspora` | Nisan 16 |
| End of Passover | **implemented** | `end_of_passover` | Nisan 21 |
| Passover, diaspora day 8 | **implemented** | `passover_day_8_diaspora` | Nisan 22 |
| Shavuot | **implemented** | `shavuot` | Sivan 6 |
| Shavuot, diaspora day 2 | **implemented** | `shavuot_day_2_diaspora` | Sivan 7 |
| Purim | **implemented** | `purim` | Day 14 of the last month of the year |
| Shushan Purim | **implemented** | `shushan_purim` | Purim + 1; if Saturday, another + 1 |
| Ta’anit Esther | **implemented** | `ta_anit_esther` | Purim − 3 if Purim is Sunday; otherwise − 1 |
| Tzom Gedaliah | **implemented** | `tzom_gedaliah` | Tishri 3; Saturday postponed by one day |
| Tzom Tammuz | **implemented** | `tzom_tammuz` | Tammuz 17; Saturday postponed by one day |
| Tishah be-Av | **implemented** | `tishah_be_av` | Av 9; Saturday postponed by one day |
| Yom ha-Shoah | **implemented** | `yom_ha_shoah` | Nisan 27; only Sunday is postponed by one day in current code |
| Yom ha-Zikkaron | **implemented** | `yom_ha_zikkaron` | Iyyar 4; Thursday/Friday → preceding Wednesday; Sunday → Monday |
| Tzom Tevet | **implemented** | `tzom_tevet` | List from `in_gregorian(tevet, 10, gyear)` |
| Hanukkah, first days | **implemented** | `hanukkah` | List from `in_gregorian(kislev, 25, gyear)` |
| Hanukkah, 8-day expansion | **implemented** | `hanukkah_days` | Starts from `hanukkah(gyear)`, offsets 0–7, year filter, sorting and deduplication |
| Intermediate days of Sukkot | **implemented** | `intermediate_days_of_sukkot` | Six consecutive days from Sukkot + 1 (Tishri 16–21) |
| Intermediate days of Passover | **implemented** | `intermediate_days_of_passover` | Five consecutive days from Passover + 1 (Nisan 16–20) |
| Rosh Hodesh | **implemented** | `rosh_hodesh` | Day 1 of the month and the preceding day if the preceding month has 30 days; year filter |
| Sh’ela | **implemented** | `sh_ela` | Coptic month 3 / day 26 |
| Birkath ha-Hama | **implemented** | `birhath_ha_hama` | Coptic 7 / 30, filter `mod(coptic_year, 28) == 17` |

The function spelling is `birhath_ha_hama`, although the CSV/test key uses
`birkath_ha_hama`. Do not introduce a second API name in pseudocode.
`in_year()` adds all the observances above, using `hanukkah_days()` instead of
just the list of starts. It sorts by date; output labels follow the code.

## Supporting rules and limitations

- **implemented**: `rosh_hodesh()` visits years `hy-1` through `hy+2`, determines
  day 1 and possibly the preceding day, filters, sorts and deduplicates.
- **implemented**: `birthday`, `yahrzeit` and their `_in_gregorian` variants handle
  personal anniversaries from an input date; they are not included in general `in_year()`.
- **implemented**: `samuel_season_in_gregorian` and `add_season_in_gregorian`
  return seasonal moments; they are not a list of additional holidays.
- **implemented**: `observational_hebrew_calendar::classical_passover_eve`
  is a calendar function, not a method of this provider; it has a separate CSV test.
- **implemented**: public static `observational_hebrew_calendar::first_of_nisan(gyear)`
  returns 1 Nisan of the observational calendar (CC 18.22): the first visible crescent
  on or after 14 days before the vernal equinox if the equinox precedes sunset at the
  reference location, otherwise 13 days. It is a calendar function, not a provider method.
- **needs decision**: whether full Hanukkah series should include January days
  from a start in the preceding Gregorian year. The current function uses only
  starts found by `hanukkah(gyear)`; filtering alone does not add a missing start.
- **needs decision**: intended Israel / diaspora variants and overlapping labels
  on the same dates. The current provider adds diaspora entries without a switch;
  intermediate-day ranges are the code ranges listed above.
- **needs decision**: independent confirmation of all modern postponement rules,
  including the complete Yom ha-Shoah policy. Describing the current condition is
  not a guarantee of complete religious or administrative rules.
- **needs decision**: local start times and observational variants. The provider
  returns RD, not sunset times; do not assume identical month rules for arithmetic
  and observational backends.
- **proposed**: year-boundary and reference-data tests for the extended holiday set.

## Verifiable references

- [Provider API](../core/inc/holiday_providers/hebrew_based_calendar_holiday_provider.hpp)
  and [implementation](../core/src/holiday_providers/hebrew_based_calendar_holiday_provider.cpp).
- [Month names](../core/inc/hebrew_month.hpp),
  [shared calendar API](../core/inc/calendars/hebrew_based_calendar.hpp),
  [arithmetic backend](../core/src/calendars/hebrew_calendar.cpp),
  [observational backend](../core/src/calendars/observational_hebrew_calendar.cpp).
- [Main tests](../tests/src/holidays/hebrew_holidays.cc): CSV comparisons for Yom Kippur,
  Passover, Purim, Ta’anit Esther, Tishah be-Av, Yom ha-Zikkaron, Sh’ela, Hanukkah
  starts and Birkath ha-Hama; also a nonempty, sorted list in 2000–2010.
- [Observational test](../tests/src/holidays/observational_hebrew_holidays.cc)
  covers `classical_passover_eve()` and `first_of_nisan()`.
- [CSV](../all-holidays.csv): CC metadata `(8.30)`–`(8.38)` for the corresponding
  single-rule rows above, `(8.43)` for Hanukkah's first day, `(18.22)` for Observ. Hebrew
  1 Nisan and `(18.25)` for Classical Passover Eve. These references do not validate
  the entire list or all regional variants. The [loader](../tests/inc/holiday_csv_loader.hpp) compares only the first
  element of a vector result with the CSV date.
