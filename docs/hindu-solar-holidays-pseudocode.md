# Święta współczesnego hinduskiego kalendarza słonecznego

Provider: `holiday_providers::modern_hindu_solar_calendar_holiday_provider`,
backend: `calendars::modern_hindu_solar_calendar`.
[Legenda stanów i zasady źródeł](README.md).

## Stan reguł

| Święto / reguła | Stan | Funkcja | Zachowanie obecnego kodu |
| --- | --- | --- | --- |
| Solar New Year / Sowramana Ugadi | **zaimplementowane** | `sowramana_ugadi(gyear)` | `to_fixed({gyear - 78, 1, 1})`, Mesha 1 |
| Ayyappa Jyothi Darshanam (Pongal w komentarzu kodu) | **zaimplementowane** | `ayyappa_jyothi_darshanam(gyear)` | Makara 1: `to_fixed({gyear - 79, 10, 1})` i poprzedni dzień |
| Mapowanie miesiąc/dzień do roku gregoriańskiego | **zaimplementowane** | `in_gregorian(smonth, sday, gyear)` | Lata `y-1`, `y`, `y+1` i filtr zakresu |
| Lista świąt | **zaimplementowane** | `in_year(gyear)` | `sowramana ugadi` i oba dni `ayyappa jyothi darshanam` |
| Wejście Słońca w Mesha | **zaimplementowane** | `modern_hindu_base::mesha_samkranti(gyear)` | Zwraca moment; nie jest osobnym wpisem tego providera |
| Dodatkowe słoneczne obchody | **proponowane** | Brak nowego uzgodnionego API | Wymagają wskazania źródła i konwencji |

Nazwy Mesha i Makara oraz przesunięcia ery pochodzą z aktualnych komentarzy i wzorów.
Metody świąt przeliczają daty backendu, nie wyznaczają w providerze nowego momentu
równonocy lub przesilenia.

## Pseudokod istniejących funkcji

```text
# zaimplementowane
sowramana_ugadi(gyear):
  return solar.to_fixed({gyear - 78, 1, 1})

ayyappa_jyothi_darshanam(gyear):
  pongal_date = solar.to_fixed({gyear - 79, 10, 1})
  return [pongal_date - 1, pongal_date]
```

`pongal_date` jest zmienną lokalną, a nie publiczną funkcją `pongal()`.
`in_gregorian()` filtruje rok, natomiast te dwie funkcje korzystają bezpośrednio
z podanych przesunięć ery.

## Otwarte decyzje

- **Wymaga ustalenia:** identyfikowanie tych obchodów z „dniem po równonocy” lub
  „zimowym przesileniem” jako ścisła reguła fizyczna. Takie sformułowania pochodziły
  z nieweryfikowalnego fragmentu tekstu; obecny backend jest sideryczny.
- **Wymaga ustalenia:** zakres utożsamiania Pongal z Ayyappa Jyothi Darshanam
  i warianty regionalne. Dokument opisuje nazwy użyte w kodzie, nie rozstrzyga tradycji.
- **Proponowane:** referencyjne testy obu świąt na niezależnych danych.

## Weryfikowalne odniesienia

- [API providera](../core/inc/holiday_providers/modern_hindu_solar_calendar_holiday_provider.hpp)
  i [implementacja](../core/src/holiday_providers/modern_hindu_solar_calendar_holiday_provider.cpp).
- [API backendu](../core/inc/calendars/modern_hindu_solar_calendar.hpp),
  [konwersja kalendarza](../core/src/calendars/modern_hindu_solar_calendar.cpp),
  [obliczenia bazowe](../core/src/calendars/modern_hindu_based.cpp).
- [Testy](../tests/src/holidays/hindu_holidays.cc): `solar_observances` sprawdza
  wynik niezerowy / niepusty w latach 2000–2010, `solar_in_year` sortowanie i niepustą
  listę. Osobny test CSV dotyczy `mesha_samkranti()`, nie `sowramana_ugadi()`.
- [CSV](../all-holidays.csv): `Mesha Saṃkrānti (date)` ma metadaną CC `(20.51), 364`.
