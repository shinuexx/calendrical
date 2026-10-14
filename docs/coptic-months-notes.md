# Miesiące i święta koptyjskie / etiopskie

Backend: `calendars::coptic_based_calendar`; instancje `coptic_calendar` oraz
`ethiopic_calendar` mają różne epoki. Provider:
`holiday_providers::coptic_based_calendar_holiday_provider`.
[Legenda stanów i zasady źródeł](README.md).

## Miesiące i parser

**Zaimplementowane:** daty używają `simple_date` z liczbowym miesiącem, miesiące
1–12 mają po 30 dni, miesiąc 13 ma 5 lub 6 dni; `is_valid_date()` sprawdza te zakresy.
Rok przestępny jest określany przez `mod(year, 4) == 3`.

**Wymaga ustalenia:** poniższe transliteracje pochodzą z pierwotnej notatki o obrazie,
którego nie ma w tym zestawie źródeł. Są roboczym słownikiem nazw, nie specyfikacją
parsera. Komentarze przy funkcjach potwierdzają tylko użyte tam nazwy i numery.

| Numer | Robocza nazwa |
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

**Proponowane:** parser nazw miesięcy. **Wymaga ustalenia:** kanoniczna pisownia,
aliasy ASCII, normalizacja diakrytyków i alternatywnych transliteracji, osobny słownik
etiopski oraz obsługa formatów kompaktowego i strukturalnego. Obecny
`calendar_traits::datetype_string_converter<simple_date>` parsuje miesiąc jako liczbę.

## Zaimplementowane święta

Każda funkcja należy do `coptic_based_calendar_holiday_provider` i zwraca
`std::vector<fixed_date_t>` dla gregoriańskiego `gyear`. `in_year()` dodaje wszystkie
poniższe święta. Reguły stosuje się do przekazanego backendu, również etiopskiego.

| Święto | Stan | Funkcja | Miesiąc / dzień w kodzie |
| --- | --- | --- | --- |
| Boże Narodzenie | **zaimplementowane** | `christmas` | 4 / 29 |
| Building of the Cross | **zaimplementowane** | `building_of_the_cross` | 1 / 17 (Thoout) |
| Obrzezanie Jezusa | **zaimplementowane** | `jesus_circumcision` | 5 / 6 (Tobe) |
| Epifania | **zaimplementowane** | `epiphany` | 5 / 11 (Tobe) |
| Zwiastowanie Maryi | **zaimplementowane** | `mary_announcement` | 7 / 29 (Paremotep) |
| Przemienienie Jezusa | **zaimplementowane** | `jesus_transfiguration` | 12 / 13 (Mesore) |

**Zaimplementowane:** `in_gregorian(cmonth, cday, gyear)` zaczyna od roku backendu
na 1 stycznia, przelicza lata `y`, `y+1`, `y+2` i filtruje zakres gregoriański.
Nie zastępuje konwersji stałymi datami gregoriańskimi.

## Rozszerzenia wymagające decyzji

- **Proponowane:** osobny zestaw koptyjskich / etiopskich ruchomych postów i świąt.
- **Wymaga ustalenia:** ich lista, tradycja i reguły przesunięć względem Wielkanocy.
  `julian_calendar_holiday_provider::orthodox_easter()` już istnieje, ale ten fakt
  nie oznacza implementacji pełnego kalendarza liturgicznego w providerze koptyjskim.

## Weryfikowalne odniesienia

- [API providera](../core/inc/holiday_providers/coptic_based_calendar_holiday_provider.hpp)
  i [implementacja](../core/src/holiday_providers/coptic_based_calendar_holiday_provider.cpp).
- [API kalendarza](../core/inc/calendars/coptic_based_calendar.hpp)
  i [implementacja / walidacja](../core/src/calendars/coptic_based_calendar.cpp).
- [Parser dat liczbowych](../core/inc/calendar_traits.hpp).
- [Testy](../tests/src/holidays/coptic_julian_holidays.cc): CSV dla `christmas()`,
  niepuste wyniki pozostałych funkcji i sortowanie w latach 2000–2010.
  Fixture koptyjski nie jest niezależnym testem reguł etiopskich.
- [CSV](../all-holidays.csv) przypisuje wierszowi `Christmas (Coptic)` odniesienie
  do *Calendrical Calculations* `(4.9), 93`; jest to metadana zbioru, nie potwierdzenie
  numeracji konkretnego wydania książki.
