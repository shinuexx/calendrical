# Święta kalendarza perskiego

Provider: `holiday_providers::persian_based_calendar_holiday_provider`.
Backend: `calendars::persian_based_calendar`, z wariantami astronomicznym
`persian_calendar` i arytmetycznym `arithmetic_persian_calendar`.
[Legenda stanów i zasady źródeł](README.md).

## Miesiące

**Zaimplementowane:** miesiące 1–6 mają po 31 dni, 7–11 po 30, miesiąc 12 ma
29 lub 30 dni. Reguła nowego roku i przestępności zależy od backendu.
Miesiące w `simple_date` i parserze są liczbowe.

**Wymaga ustalenia:** pisownia i aliasy poniższego roboczego słownika z pierwotnych
notatek. **Proponowane:** parser nazw, po uzgodnieniu tego słownika.

| Numer | Robocza nazwa | Dni |
| --- | --- | --- |
| 1 | Farvardin | 31 |
| 2 | Ordibehesht | 31 |
| 3 | Khordad | 31 |
| 4 | Tir | 31 |
| 5 | Mordad | 31 |
| 6 | Shahrivar | 31 |
| 7 | Mehr | 30 |
| 8 | Aban | 30 |
| 9 | Azar | 30 |
| 10 | Dey | 30 |
| 11 | Bahman | 30 |
| 12 | Esfand | 29 lub 30 |

## Stan świąt i reguł

Wszystkie poniższe funkcje już istnieją i są używane przez `in_year(gyear)`.

| Święto / reguła | Stan | Funkcja | Zachowanie kodu |
| --- | --- | --- | --- |
| Nowruz | **zaimplementowane** | `nawruz` | `get_new_year(pyear)`; wynik pojedynczy |
| Sizdah Bedar | **zaimplementowane** | `sizdah_bedar` | Lista dla 1 / 13 |
| Chaharshanbe Suri | **zaimplementowane** | `chaharshanbe_suri` | `weekday::wednesday.before(nawruz(gyear))`; ściśle przed |
| Tirgan | **zaimplementowane** | `tirgan` | Lista dla 4 / 13 |
| Mehregan | **zaimplementowane** | `mehregan` | Lista dla 7 / 16 |
| Yalda Night | **zaimplementowane** | `yalda_night` | Lista cywilnych dat 9 / 30 |
| Sepandarmazgan | **zaimplementowane** | `sepandarmazgan` | Lista dla 12 / 5 |
| Mapowanie miesiąc/dzień | **zaimplementowane** | `in_gregorian` | Konwersja lat `y-1`, `y`, `y+1` i filtr roku gregoriańskiego |

Pisownia API to `nawruz`, mimo nazwy „Nowruz” w CSV. Nowy rok wyznacza:

```text
# zaimplementowane w nawruz(gyear)
pyear = gyear - GregorianYear(PersianEpoch) + 1
if pyear <= 0: pyear = pyear - 1
return persian.get_new_year(pyear)
```

Jest to poprawiony w PR #3 offset roku; nie używać starego wzoru bez `+1`.
Nie zastępować wyników przybliżonymi stałymi datami gregoriańskimi.

## Konwencje wymagające ustalenia

- **Wymaga ustalenia:** zewnętrzne potwierdzenie konwencji wieczoru dla Yalda Night
  oraz Chaharshanbe Suri i ewentualny model czasu rozpoczęcia.
  Nagłówek opisuje Yalda jako cywilny dzień Azar 30 z początkiem poprzedniego
  wieczoru, a Chaharshanbe Suri jako środę przed Nowruz; provider zwraca tylko RD.
- **Wymaga ustalenia:** warianty tradycji i wybór astronomicznego / arytmetycznego
  backendu dla nowych zestawów danych. CLI i obecny fixture używają `persian_calendar`.
- **Proponowane:** niezależne referencje dla pozostałych świąt i osobne testy wariantów.

## Weryfikowalne odniesienia

- [API providera](../core/inc/holiday_providers/persian_based_calendar_holiday_provider.hpp)
  i [implementacja](../core/src/holiday_providers/persian_based_calendar_holiday_provider.cpp).
- [API backendu](../core/inc/calendars/persian_based_calendar.hpp),
  [wspólne reguły](../core/src/calendars/persian_based_calendar.cpp),
  [astronomia](../core/src/calendars/persian_calendar.cpp),
  [wariant arytmetyczny](../core/src/calendars/arithmetic_persian_calendar.cpp).
- [Testy](../tests/src/holidays/persian_holidays.cc): CSV dla `nawruz()`, niepuste
  listy pozostałych stałych świąt i sortowanie w latach 2000–2010.
  Test sortowania nie potwierdza niezależnie reguły Chaharshanbe Suri.
- [CSV](../all-holidays.csv): `Nowruz` ma metadaną CC `(15.11), 265`.
