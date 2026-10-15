# Holiday Rules and Design Notes

This index covers ten documents extracted for [issue #5](https://github.com/shinuexx/calendrical/issues/5).
The source set comes from branch `wip/akan-mayan-cyclic-and-interactive-cli`,
commit `d6fe7f6952fd7ada2c433402569f13f6fd7cd060`. Implementation descriptions were
checked against `master` at commit `ea0790376041c48680a20b28229efb5745fabd0c`,
after [PR #3](https://github.com/shinuexx/calendrical/pull/3) was merged. The Hebrew document
was updated for public `first_of_nisan()` after [PR #11](https://github.com/shinuexx/calendrical/pull/11).
When the code changes, update the relevant rule and its status.

## Documents

| Topic | Document | Main content |
| --- | --- | --- |
| Classical Armenian | [Simple-calendar holidays](armenian-simple-holidays-pseudocode.md) | Navasard and the generic provider's scope |
| Bahá’í | [Holidays and variants](bahai-holidays-pseudocode.md) | Classic New Year, astronomical holidays, birthdays, proposed series |
| Balinese Pawukon | [Calendar and holidays](balinese-pawukon-holidays-pseudocode.md) | 210-day cycle, Kajeng Keliwon, Tumpek |
| Coptic / Ethiopic | [Months and holidays](coptic-months-notes.md) | Numeric months, fixed holidays, working aliases |
| Hebrew | [Holidays and rules](hebrew-holidays-pseudocode.md) | Postponements, diaspora, series, supporting Coptic rules |
| Hindu lunar | [Holidays and tithi](hindu-lunar-holidays-pseudocode.md) | Month/day rules, time of day, multi-day observances |
| Hindu solar | [Solar holidays](hindu-solar-holidays-pseudocode.md) | Sowramana Ugadi, Ayyappa Jyothi Darshanam, Mesha Samkranti |
| Islamic | [Months and holidays](islamic-months-notes.md) | Tabular month lengths, holidays, backend variants |
| Persian | [Holidays and year mapping](persian-holidays-pseudocode.md) | Nowruz, fixed dates, Wednesday before Nowruz |
| Samaritan | [Holidays and moveable rules](samaritan-holidays-pseudocode.md) | Day ranges, Pentecost and Ṣimmut |
| Tibetan | [Holidays and leap month/day](tibetan-holidays-pseudocode.md) | Losar, ranges, current leap-flag policy |

This is not a complete index of all holiday providers. `gregorian`, `julian` and
`chinese` / `korean` / `vietnamese` have no dedicated document here; their API rules
are documented in the Doxygen comments of the [Gregorian](../core/inc/holiday_providers/gregorian_calendar_holiday_provider.hpp),
[Julian](../core/inc/holiday_providers/julian_calendar_holiday_provider.hpp) and
[Chinese-family](../core/inc/holiday_providers/chinese_calendar_holiday_provider.hpp)
provider headers. The `us` provider (Independence Day, Labor Day, Memorial Day,
Election Day and DST start/end, CC 2.32–2.40) is documented in its
[provider header](../core/inc/holiday_providers/us_holiday_provider.hpp).

## Other calendar documentation

- [Maya Long Count](mayan-long-count.md): correlation, canonical fields, numeric bounds,
  pre-epoch arithmetic, CLI formats and reference tests; separate from the holiday documents above.

## Meaning of statuses

- **implemented** — the function or rule exists in the linked code. The API name,
  calculation and backend follow the implementation; the status does not mean
  independent confirmation of correctness for every tradition.
- **proposed** — an extension idea absent from the described API. It is not an
  agreed implementation commitment or a finished specification.
- **needs decision** — a confirmed source, variant choice or contract decision is
  missing. If the code already uses a specific variant, the document separately
  describes it as implemented and leaves the intended convention as an open question.

A status applies to a particular holiday or rule, not the entire calendar.
Pseudocode labeled “implemented” summarizes the current algorithm; it is not a new
specification. The `gyear` argument means a Gregorian year. Providers return RD dates
(`fixed_date_t`), not observance start times, unless a supporting function returning
`moment_t` is explicitly identified.

## Sources and verification limits

Implementation descriptions are based on linked headers, calendar/provider sources
and tests. The original notes also referred to images and text excerpts not included
in this source set. Names, historical dates and ideas retained from those materials
are marked as working notes or needing a decision, not as a religious specification.

The [`all-holidays.csv` dataset](../all-holidays.csv) contains dates for 2000–2103
and a `Func., page` column referring to *Calendrical Calculations*. Numbers quoted
in the documents are verifiable as metadata of that file. The book's edition and
text have not been confirmed here; do not assign these numbers to another edition
without checking. Entries without such references rely on evidence in the code,
not invented book function numbers.

[Holiday tests](../tests/src/holidays/) and the [CSV loader](../tests/inc/holiday_csv_loader.hpp)
provide different levels of coverage:

- a CSV comparison checks the specified method for the years in the file;
- for a vector result, the loader compares only the **first date** or `none`;
- a nonempty-result / sorting test does not independently confirm the holiday date,
  full series length, postponement rule or regional variant.

Preserve this distinction in further work and add sources and tests appropriate
to the specific rule. These documents describe the code; changes to rules belong
in the implementation and tests.

## API migration

- [Explicit fixed-date conversion](fixed-date-migration.md): integer day arithmetic,
  scalar access with `value()` and fractional-day calculations using `moment_t`.
