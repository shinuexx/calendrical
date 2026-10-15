# Maya Long Count

`calendars::mayan_calendar` converts between fixed dates (RD) and
`datetypes::mayan_long_count`. The implementation is a proleptic integer count,
not a cycle that resets at 13 baktuns. Haab, Tzolkin and Calendar Round are outside
its scope. The `cakabtun` spelling follows the source prototype's public field name.

## Correlation and reference dates

The GMT correlation constant is **JDN 584283**. In this library's JD/RD convention,
`floor(584283 - 1721424.5)` is **RD -1137142**. The integer epoch is stored directly;
conversion never passes large day counts through `double`. JDN 584283 labels the
Julian day whose noon has JD 584283; its civil midnight is JD 584282.5. Both fall
on the same RD day under the library's floor convention.

The [Smithsonian National Museum of the American Indian, Living Maya Time:
The Calendar System](https://maya.nmai.si.edu/calendar/calendar-system) provides
two independent reference pairs used by the tests:

| Gregorian date | Five-field Long Count | RD |
| --- | --- | --- |
| 2000-01-01 | 12.19.6.15.2 | 730120 |
| 2012-12-21 | 13.0.0.0.0 | 734858 |

The latter count is 1,872,000 days after the epoch. Subtracting that count yields
the proleptic Gregorian epoch -3113-08-11 (3114 BCE, astronomical year numbering).
Zero in this API means zero elapsed days; this does not attempt to reproduce the
mythological creation-date notation discussed by historical sources.

## Canonical fields and numeric domain

| Field | Days per unit | Accepted range |
| --- | --- | --- |
| alautun | 23,040,000,000 | -400319965 through 400319965 |
| kinchiltun | 1,152,000,000 | 0 through 19 |
| cakabtun | 57,600,000 | 0 through 19 |
| pictun | 2,880,000 | 0 through 19 |
| baktun | 144,000 | 0 through 19 |
| katun | 7,200 | 0 through 19 |
| tun | 360 | 0 through 19 |
| uinal | 20 | 0 through 17 |
| kin | 1 | 0 through 19 |

All fields are `int32_t`. Only the most significant field can be negative.
Out-of-range fields are **rejected**, not silently normalized. For example, a tun
contains 18 uinals, so uinal 18 is invalid; use the next tun instead.

The alautun bound is `INT64_MAX / days_per_alautun - 1`. It deliberately reserves
one full block of headroom so every weighted sum, epoch addition/subtraction and
floor/mod intermediate fits in `int64_t`, including negative counts. This selects
complete blocks instead of partly representable edge blocks. It is a numeric
representation limit, not a historical or astronomical validity claim.

The inclusive RD domain is exposed as `mayan_calendar::min_date` and `max_date`:

```text
min_date = epoch - max_alautun * days_per_alautun
max_date = epoch + max_alautun * days_per_alautun + days_per_alautun - 1
```

`is_valid_date()` checks field ranges without conversion. `to_fixed()` throws
`std::invalid_argument` for an invalid date; `to_date()` checks the RD domain before
subtracting the epoch and throws `std::out_of_range` outside it. Neither operation
has undefined overflow or narrowing within the supported domain.

Before the epoch, floor division and mathematical modulo keep lower fields
nonnegative. One day before zero is the nine-field value:

```text
-1-19-19-19-19-19-19-17-19
```

Round-trip `to_fixed(to_date(rd)) == rd` holds throughout the declared domain.

## Text formats and CLI

The CLI registers `mayan` and the alias `mayan_long_count`. Inputs are either:

- Nine dash-separated fields, most significant first:
  `ALAUTUN-KINCHILTUN-CAKABTUN-PICTUN-BAKTUN-KATUN-TUN-UINAL-KIN`.
- Five dash-separated fields: `BAKTUN-KATUN-TUN-UINAL-KIN`, with four leading
  zero fields implied. This shorthand cannot encode negative counts or baktun 20.

Dots are not an input delimiter; the dotted forms in reference tables are the
conventional historical display. A leading minus sign is allowed in the full
nine-field input. Other negative components are parsed but rejected by validation.

```bash
calendrical_cli convert mayan gregorian 13-0-0-0-0
calendrical_cli convert gregorian mayan_long_count 2012-12-21
calendrical_cli convert mayan rd -1-19-19-19-19-19-19-17-19
```

Like other calendar converters, output is a structured string, always containing
all nine fields. For example:

```text
mayan_long_count{alautun:0,kinchiltun:0,cakabtun:0,pictun:0,baktun:13,katun:0,tun:0,uinal:0,kin:0}
```

`calendar_traits::datetype_string_converter<mayan_long_count>` provides structured
parsing/formatting and compact parsing. Structured input requires all nine distinct
named integer fields. Parsers check syntax and `int32_t` representation; semantic
field/domain validation belongs to the calendar and is invoked by the CLI converter.
The CLI accepts the compact formats above, not the structured output string.
`to_string()` and the ADL stream operator live in `mayan_dates.cpp`.

## Verification and implementation

- [Calendar API](../core/inc/calendars/mayan_calendar.hpp) and
  [implementation](../core/src/calendars/mayan_calendar.cpp).
- [Date type](../core/inc/datetypes/mayan_dates.hpp) and
  [formatting](../core/src/datetypes/mayan_dates.cpp).
- [Calendar tests](../tests/src/calendars/mayan_calendar.cc): Smithsonian reference
  dates, every field carry, pre-epoch borrowing, invalid fields, inclusive endpoints,
  excluded `int64_t` extremes and 10,000 deterministic round-trips across the domain.
- [Parser and CLI tests](../tests/src/cli_mayan.cc): aliases, both input lengths,
  structured round-trip, malformed/out-of-range inputs, precision, help and conversions.
