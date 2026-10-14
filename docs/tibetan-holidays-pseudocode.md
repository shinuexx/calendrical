# Święta kalendarza tybetańskiego

Provider `holiday_providers::tibetan_calendar_holiday_provider` już istnieje.
Backend to `calendars::tibetan_calendar`, data to
`lunar_date{year, month, leap_month, day, leap_day}`. CLI rejestruje provider `tibetan`.
[Legenda stanów i zasady źródeł](README.md).

## Zaimplementowane święta

Wszystkie funkcje przyjmują gregoriański `gyear`, zwracają listę RD i trafiają do `in_year()`.

| Święto | Stan | Funkcja providera | Reguła |
| --- | --- | --- | --- |
| Losar | **zaimplementowane** | `losar` | Deleguje do `tibetan_calendar::new_year(gyear)` |
| Monlam Chenmo | **zaimplementowane** | `monlam_chenmo` | Miesiąc 1, dni 4–11 |
| Chotrul Duchen | **zaimplementowane** | `chotrul_duchen` | 1 / 15 |
| Saga Dawa Duchen | **zaimplementowane** | `saga_dawa_duchen` | 4 / 15 |
| Chokhor Duchen | **zaimplementowane** | `chokhor_duchen` | 6 / 4 |
| Lhabab Duchen | **zaimplementowane** | `lhabab_duchen` | 9 / 22 |
| Ganden Ngamcho | **zaimplementowane** | `ganden_ngamcho` | 10 / 25 |

## Mapowanie i powtórzone daty

**Zaimplementowane:** `in_gregorian(tmonth, tday, gyear)` używa konwencji
`leap_month=false`, `leap_day=false`, przelicza lata `tyear-1` do `tyear+2` włącznie,
filtruje gregoriański zakres, sortuje i usuwa duplikaty.
Prywatne `fixed_range_in_gregorian()` składa listy dla kolejnych numerów dni.

```text
# zaimplementowane w in_gregorian
range = gregorian.get_year_range(gyear)
tyear = tibetan.to_date(range.min).year
candidates = [tibetan.to_fixed({y, month, false, day, false})
              for y in tyear-1 .. tyear+2]
return sorted_unique(filter(range, candidates))
```

**Zaimplementowane — wyjątek Losar:** kalendarzowe `losar(tyear)` wybiera
`is_leap_month(tyear, 1)`; `new_year(gyear)` bierze dwa sąsiednie lata i filtruje
wyniki. Nie opisywać Losar jako zwykłego `in_gregorian(1, 1, gyear)` z flagą `false`.

## Otwarte decyzje

- **Wymaga ustalenia:** docelowe zasady obchodów w miesiącach przestępnych,
  dniach powtórzonych i pominiętych. Obecne flagi oraz konwersja opisują implementację,
  ale nie potwierdzają wszystkich tradycji.
- **Wymaga ustalenia:** czy rozszerzone API ma zwracać alternatywne wystąpienia
  oraz jak interpretować zakres Monlam Chenmo przy pominiętych dniach.
- **Wymaga ustalenia:** początek obchodów o określonej porze; obecne wyniki są
  etykietami cywilnych dat, a nie czasami rozpoczęcia.
- **Proponowane:** referencje dla świąt innych niż Losar i testy polityki leap month/day.

## Weryfikowalne odniesienia

- [API providera](../core/inc/holiday_providers/tibetan_calendar_holiday_provider.hpp)
  i [implementacja](../core/src/holiday_providers/tibetan_calendar_holiday_provider.cpp).
- [API kalendarza](../core/inc/calendars/tibetan_calendar.hpp)
  i [konwersja / reguła Losar](../core/src/calendars/tibetan_calendar.cpp).
- [Rejestracja CLI](../cli/src/holiday_provider_provider_builder.cpp).
- [Testy](../tests/src/holidays/tibetan_holidays.cc): Losar porównany z CSV;
  pozostałe funkcje sprawdzane na niepuste wyniki w latach 2000–2010, lista na sortowanie.
- [CSV](../all-holidays.csv): `Losar` ma metadaną CC `(21.9), 381`.
