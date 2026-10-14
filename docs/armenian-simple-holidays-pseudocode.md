# Święta klasycznego kalendarza ormiańskiego

Zakres: `calendars::armenian_calendar`, czyli instancja `simple_calendar` z cyklem
365 dni (12 miesięcy po 30 dni i 5 dni dodatkowych). Nie jest to współczesny
ormiański kalendarz liturgiczny. [Legenda stanów i zasady źródeł](README.md).

## Stan reguł

| Święto lub reguła | Stan | Implementacja / zakres |
| --- | --- | --- |
| Navasard, miesiąc 1 / dzień 1 | **zaimplementowane** | `simple_calendar_holiday_provider::navasard(gyear)` → `in_gregorian(1, 1, gyear)` |
| Mapowanie dat do roku gregoriańskiego | **zaimplementowane** | `in_gregorian(cmonth, cday, gyear)` przelicza lata `y-1`, `y`, `y+1` i filtruje zakresem roku |
| Lista świąt | **zaimplementowane** | `in_year(gyear)` dodaje nazwę `navasard` i sortuje po dacie |
| Ograniczenie Navasard wyłącznie do instancji ormiańskiej | **wymaga ustalenia** | W pierwotnych notatkach proponowano test tożsamości instancji; obecny kod go nie wykonuje |
| Oddzielny provider ormiańskich świąt liturgicznych | **proponowane** | Poza modelem stałego roku cywilnego; wymaga osobnego źródła reguł |
| Wielkanoc, Wniebowstąpienie i reguły najbliższej niedzieli w takim providerze | **wymaga ustalenia** | Brak uzgodnionego zestawu świąt i konwencji w tych notatkach |

## Zachowanie obecnego kodu

Provider jest ogólny: `navasard()` działa na dowolnym przekazanym `simple_calendar`.
CLI rejestruje go zarówno jako `armenian`, jak i `egyptian`; oba wpisy otrzymują
nazwę `navasard`. To opis implementacji, a nie potwierdzenie historycznej nazwy
obchodów w każdym kalendarzu. Wprowadzenie ograniczenia wymaga osobnej decyzji.

Pseudokod **zaimplementowanej** reguły:

```text
in_year(gyear):
  result = [("navasard", d) for d in navasard(gyear)]
  sort_by_date(result)
  return result
```

## Weryfikowalne odniesienia

- [API providera](../core/inc/holiday_providers/simple_calendar_holiday_provider.hpp)
  i [implementacja](../core/src/holiday_providers/simple_calendar_holiday_provider.cpp).
- [API kalendarza](../core/inc/calendars/simple_calendar.hpp)
  i [implementacja / epoki](../core/src/calendars/simple_calendar.cpp).
- [Rejestracja w CLI](../cli/src/holiday_provider_provider_builder.cpp).
- [Testy](../tests/src/holidays/samaritan_simple_holidays.cc): `simple_holidays_test`
  sprawdza niepuste wyniki i sortowanie dla instancji ormiańskiej w latach 2000–2010.
  Nie jest to niezależna walidacja dat święta ani zachowania instancji egipskiej.
