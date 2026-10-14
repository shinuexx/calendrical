# Coptic / Ethiopic Months and Holidays

Backend: `calendars::coptic_based_calendar`; the `coptic_calendar` and
`ethiopic_calendar` instances have different epochs. Provider:
`holiday_providers::coptic_based_calendar_holiday_provider`.
[Status legend and source policy](README.md).

## Months and parsing

**implemented**: dates use `simple_date` with a numeric month; months 1–12 have
30 days each, month 13 has 5 or 6 days, and `is_valid_date()` checks these ranges.
A leap year is determined by `mod(year, 4) == 3`.

**needs decision**: the transliterations below come from the original note about
an image that is not included in this source set. They are a working list of names,
not a parser specification. Function comments confirm only the names and numbers
used in those comments.

| Number | Working name |
| --- | --- |
| 1 | Thoout |
| 2 | Paope |
| 3 | Athor |
| 4 | Koiak |
| 5 | Tōbe / Tobe |
| 6 | Meshir |
| 7 | Paremotep |
| 8 | Parmoute |
| 9 | Pashons |
| 10 | Paōne / Paone |
| 11 | Epēp / Epep |
| 12 | Mesorē / Mesore |
| 13 | Epagomenē / Epagomene |

**proposed**: a month-name parser. **needs decision**: canonical spelling, ASCII
aliases, normalization of diacritics and alternative transliterations, a separate
Ethiopic vocabulary, and support for compact and structured formats. The current
`calendar_traits::datetype_string_converter<simple_date>` parses the month as a number.

## Implemented holidays

Each function belongs to `coptic_based_calendar_holiday_provider` and returns
`std::vector<fixed_date_t>` for Gregorian `gyear`. `in_year()` includes all the
holidays below. The rules apply to the supplied backend, including the Ethiopic instance.

| Holiday | Status | Function | Month / day in code |
| --- | --- | --- | --- |
| Christmas | **implemented** | `christmas` | 4 / 29 |
| Building of the Cross | **implemented** | `building_of_the_cross` | 1 / 17 (Thoout) |
| Jesus's Circumcision | **implemented** | `jesus_circumcision` | 5 / 6 (Tobe) |
| Epiphany | **implemented** | `epiphany` | 5 / 11 (Tobe) |
| Mary's Announcement | **implemented** | `mary_announcement` | 7 / 29 (Paremotep) |
| Jesus's Transfiguration | **implemented** | `jesus_transfiguration` | 12 / 13 (Mesore) |

**implemented**: `in_gregorian(cmonth, cday, gyear)` starts with the backend year
on January 1, converts years `y`, `y+1`, `y+2` and filters by the Gregorian range.
It does not replace conversion with fixed Gregorian dates.

## Extensions requiring decisions

- **proposed**: a separate set of Coptic / Ethiopic moveable fasts and feasts.
- **needs decision**: their list, tradition and offsets from Easter.
  `julian_calendar_holiday_provider::orthodox_easter()` already exists, but this
  does not mean that the Coptic provider implements a complete liturgical calendar.

## Verifiable references

- [Provider API](../core/inc/holiday_providers/coptic_based_calendar_holiday_provider.hpp)
  and [implementation](../core/src/holiday_providers/coptic_based_calendar_holiday_provider.cpp).
- [Calendar API](../core/inc/calendars/coptic_based_calendar.hpp)
  and [implementation / validation](../core/src/calendars/coptic_based_calendar.cpp).
- [Numeric-date parser](../core/inc/calendar_traits.hpp).
- [Tests](../tests/src/holidays/coptic_julian_holidays.cc): CSV comparison for `christmas()`,
  nonempty results for the other functions and sorting in 2000–2010.
  The Coptic fixture is not an independent test of Ethiopic rules.
- [CSV](../all-holidays.csv) assigns the `Christmas (Coptic)` row a reference
  to *Calendrical Calculations* `(4.9), 93`; this is dataset metadata, not confirmation
  of numbering in a particular edition of the book.
