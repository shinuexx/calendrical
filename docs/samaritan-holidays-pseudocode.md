# Święta kalendarza samarytańskiego

Provider `holiday_providers::samaritan_calendar_holiday_provider` już istnieje,
a CLI rejestruje go jako `samaritan`. Backend `calendars::samaritan_calendar`
używa `simple_date{year, month, day}` z liczbowymi miesiącami.
[Legenda stanów i zasady źródeł](README.md).

## Stan świąt

Wszystkie funkcje są używane przez `in_year(gyear)`; argument jest rokiem gregoriańskim.

| Święto | Stan | Dokładna nazwa funkcji | Wynik i reguła |
| --- | --- | --- | --- |
| Passover | **zaimplementowane** | `passover` | Lista: miesiąc 1 / dzień 14 |
| Festival of the Unleavened Bread | **zaimplementowane** | `festival_of_unleavened_bread` | Lista: 1 / 15–21 |
| Festival of Pentecost | **zaimplementowane** | `festival_of_pentecost` | Pojedyncze RD: pierwsza niedziela ściśle po `passover(gyear).front()` + 49 dni |
| Festival of the Seventh Month | **zaimplementowane** | `festival_of_the_seventh_month` | Lista: 7 / 1 |
| Day of Atonement | **zaimplementowane** | `day_of_atonement` | Lista: 7 / 10 |
| Festival of Tabernacles | **zaimplementowane** | `festival_of_tabernacles` | Lista: 7 / 15–21 |
| The Eighth Day | **zaimplementowane** | `eighth_day` | Lista: 7 / 22 |
| Ṣimmut of Passover | **zaimplementowane** | `simmut_of_passover` | Pojedyncze RD: sobota na lub przed pierwszym Passover − 49 dni |
| Ṣimmut of Tabernacles | **zaimplementowane** | `simmut_of_tabernacles` | Pojedyncze RD: sobota na lub przed pierwszym Tabernacles − 49 dni |

Nie używać nieistniejących skróconych nazw z dawnego pseudokodu (`unleavened_bread`,
`pentecost`, `seventh_month_festival`, `tabernacles`) jako nazw API providera.

## Mapowanie i zakresy

**Zaimplementowane:** `in_gregorian(smonth, sday, gyear)` przelicza `y-1`, `y`, `y+1`,
gdzie `y` pochodzi z backendu na początku zakresu gregoriańskiego, i filtruje wyniki.
Prywatny `fixed_range_in_gregorian()` składa listy dla każdego numeru dnia
w zakresie domkniętym, sortuje i deduplikuje. Nie zakładać jednego wystąpienia
na rok dla wszystkich metod zwracających listę.

Pseudokod **zaimplementowanych** reguł ruchomych:

```text
festival_of_pentecost(gyear):
  p = passover(gyear)
  if p is empty: throw runtime_error
  return sunday.after(p.front()) + 49 days

simmut_of_passover(gyear):
  p = passover(gyear)
  if p is empty: throw runtime_error
  return saturday.on_or_before(p.front() - 49 days)

simmut_of_tabernacles(gyear):
  t = festival_of_tabernacles(gyear)
  if t is empty: throw runtime_error
  return saturday.on_or_before(t.front() - 49 days)
```

Metody ruchome używają pierwszego elementu listy kotwic. Nie filtrują ponownie
wyliczonej daty względem roku gregoriańskiego; powyższy opis nie obiecuje takiego filtra.

## Otwarte decyzje

- **Wymaga ustalenia:** zewnętrzne potwierdzenie interpretacji „8th Sunday after
  Passover” — kod przyjmuje pierwszą niedzielę ściśle po i siedem kolejnych tygodni.
- **Wymaga ustalenia:** wybór soboty dla Ṣimmut; `on_or_before(holiday - 49 days)`
  jest zachowaniem istniejącym, nie rozstrzygnięciem wszystkich interpretacji tekstu.
- **Wymaga ustalenia:** polityka metod ruchomych przy zerowej / wielokrotnej kotwicy
  i daty wynikowej poza rokiem zapytania. Obecne typy i wyjątki opisano wyżej.
- **Wymaga ustalenia:** ewentualna pora rozpoczęcia obchodów poprzedniego wieczoru;
  provider zwraca cywilne RD, nie momenty.
- **Proponowane:** niezależny zbiór referencyjny i testy konkretnych reguł liczenia.

## Weryfikowalne odniesienia

- [API providera](../core/inc/holiday_providers/samaritan_calendar_holiday_provider.hpp)
  i [implementacja](../core/src/holiday_providers/samaritan_calendar_holiday_provider.cpp).
- [API backendu](../core/inc/calendars/samaritan_calendar.hpp)
  i [konwersja](../core/src/calendars/samaritan_calendar.cpp).
- [Rejestracja CLI](../cli/src/holiday_provider_provider_builder.cpp).
- [Testy](../tests/src/holidays/samaritan_simple_holidays.cc): `samaritan_holidays_test`
  sprawdza niepuste / niezerowe wyniki funkcji oraz sortowanie w latach 2000–2010.
  Nie zawiera niezależnych dat referencyjnych ani rozstrzygnięcia otwartych konwencji.
