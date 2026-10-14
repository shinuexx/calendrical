# Święta współczesnego hinduskiego kalendarza księżycowego

Provider: `holiday_providers::modern_hindu_lunar_calendar_holiday_provider`.
Backend: `calendars::modern_hindu_lunar_calendar`, data to
`lunar_date{year, month, leap_month, day, leap_day}`.
[Legenda stanów i zasady źródeł](README.md).

## Stan świąt i reguł

Wszystkie wymienione metody już istnieją. Argument `gyear` jest gregoriański,
a wyniki świąt są listami RD. `in_year()` dodaje każde święto z tabeli, z wyjątkiem
osobnego wpisu `diwali()` — etykieta `diwali` korzysta z `diwali_days()`.

| Święto | Stan | Funkcja | Reguła obecnego kodu |
| --- | --- | --- | --- |
| Rama Navami | **zaimplementowane** | `rama` | `lunar_event(1, 9, moment_t{0.5}, gyear)` |
| Maha Shivaratri | **zaimplementowane** | `shiva` | `lunar_event(11, 29, moment_t{1}, gyear)` |
| Sacred Wednesdays | **zaimplementowane** | `sacred_wednesdays` | Środy, których dzień księżycowy wynosi 8 |
| Diwali, główny dzień | **zaimplementowane** | `diwali` | `lunar_holiday(8, 1, gyear)` |
| Diwali, zestaw dni | **zaimplementowane** | `diwali_days` | Złączenie `lunar_holiday(7, 29)`, `lunar_holiday(7, 30)` i `diwali`; sortowanie i deduplikacja |
| Janmashtami | **zaimplementowane** | `janmashtami` | `lunar_holiday(5, 23, gyear)` |
| Ganesha Chaturthi | **zaimplementowane** | `ganesha_chaturthi` | 6 / 4 |
| Durga Ashtami | **zaimplementowane** | `durga_ashtami` | 7 / 8 |
| Sarasvati Puja | **zaimplementowane** | `sarasvati_puja` | 7 / 9 |
| Dasra | **zaimplementowane** | `dasra` | 7 / 10 |
| Karthikai Deepam | **zaimplementowane** | `karthikai_deepam` | 8 / 15 |
| Vaikunta Ekadashi | **zaimplementowane** | `vaikunta_ekadashi` | 9 / 11 |
| Buddha’s Birthday | **zaimplementowane** | `buddha_birthday` | Uproszczona konwencja `lunar_holiday(2, 15, gyear)` |
| Varalakshmi Vratam | **zaimplementowane** | `varalakshmi_vratam` | Piątek ściśle przed datą z `lunar_holiday(5, 15, gyear)` |
| Holi | **zaimplementowane** | `holi` | `lunar_event(12, 15, moment_t{0.75}, gyear)`; przybliżenie wieczoru |

## Miesiące i dostępność dni

**Zaimplementowane:** funkcje oraz komentarze kodu używają następującego mapowania:
1 Caitra, 2 Vaishakha, 5 Shravana, 6 Bhadrapada, 7 Ashvina, 8 Kartika,
9 Margashirsha, 11 Magha, 12 Phalguna. Są to liczby w API, nie enum miesięcy.
**Wymaga ustalenia:** kanoniczne transliteracje i aliasy parsera; lista nie zatwierdza
pełnego słownika wszystkich tradycji.

**Zaimplementowane:** `date_occur(lyear, lmonth, lday)` tworzy początkowo datę
z `leap_month=false` i `leap_day=false`. Sprawdza zgodność miesiąca i dnia po
konwersji, obsługuje miesiąc usunięty oraz nieobecny dzień. Dlatego
`lunar_holiday()` nie jest wyłącznie odczytem literalnej, istniejącej daty.
`in_gregorian()` deleguje do `lunar_holiday()`: dwa sąsiednie lata księżycowe,
wyznaczenie `date_occur()` i filtr zakresu gregoriańskiego.

**Zaimplementowane:** `tithi_occur(lmonth, tithi, t, lyear)` wyszukuje początek tithi
i porównuje fazę z czasem otrzymanym przez `standard_from_sundial()` dla Ujjain.
`lunar_event()` filtruje wyniki dwóch lat. Parametry `0.5`, `0.75`, `1` opisują
obecny algorytm, nie dowolną regionalną godzinę rozpoczęcia święta.

**Zaimplementowane:** `sacred_wednesdays_in_range(range)` enumeruje środy w
zakresie i wybiera dzień księżycowy 8; `karana(n)` jest osobnym helperem indeksu,
a nie świętem dodawanym do `in_year()`.

## Otwarte decyzje z pierwotnych notatek

| Temat | Stan | Obecny kod i pytanie na przyszłość |
| --- | --- | --- |
| „Friday prior to Shravana 15” | **wymaga ustalenia** | Kod używa `weekday::friday.before(d)` (ściśle przed); potwierdzenie źródłowej interpretacji nadal otwarte |
| Wielodniowe Diwali | **wymaga ustalenia** | Kod łączy trzy żądane daty przez `date_occur`; nie wykonuje osobnego testu „Ashvina 30 istnieje” |
| Diwali: wspólna etykieta czy nazwy poddni | **wymaga ustalenia** | Obecne `in_year()` nadaje wszystkim datom nazwę `diwali` |
| Buddha’s Birthday | **wymaga ustalenia** | Vaiśākha 15 jest uproszczeniem; brak konfiguracji tradycji |
| Holi: precyzyjna pora / lokalizacja | **wymaga ustalenia** | Obecna metoda używa przybliżenia `0.75` i helpera Ujjain |
| Fasting day poprzedzający Maha Shivaratri | **proponowane** | Brak osobnej metody / wpisu providera; dokładna konwencja wymaga źródła |
| Polityka miesięcy/dni przestępnych i usuniętych dla poszczególnych świąt | **wymaga ustalenia** | Nie rozstrzygać wyłącznie na podstawie ogólnego helpera |
| Testy pełnych serii oraz wariantów regionalnych | **proponowane** | Oddzielne referencje, zwłaszcza dla rozszerzonego zestawu |

Istniejące funkcje nie są już zadaniami „H-L1..H-L4”. Otwarte decyzje nie zmieniają
ich obecnego statusu implementacyjnego i nie są tu rozstrzygane.

## Weryfikowalne odniesienia

- [API providera](../core/inc/holiday_providers/modern_hindu_lunar_calendar_holiday_provider.hpp)
  i [implementacja](../core/src/holiday_providers/modern_hindu_lunar_calendar_holiday_provider.cpp).
- [API backendu](../core/inc/calendars/modern_hindu_lunar_calendar.hpp),
  [konwersja](../core/src/calendars/modern_hindu_lunar_calendar.cpp),
  [reguły bazowe / Hindu Lunar New Year](../core/src/calendars/modern_hindu_based.cpp).
- [Testy](../tests/src/holidays/hindu_holidays.cc): CSV dla `diwali`, Sacred Wednesday,
  `shiva`, `rama` i kalendarzowego `hindu_lunar_new_year`; `lunar_in_year` sprawdza
  jedynie niepustą, posortowaną listę 2000–2010.
- [CSV](../all-holidays.csv): metadane CC Diwali `(20.57), 368`, Shiva `(20.60), 369`,
  Rama `(20.61), 369`, Sacred Wednesday `(20.65), 370`, Hindu Lunar New Year
  `(20.53), 365`. [Loader](../tests/inc/holiday_csv_loader.hpp) porównuje pierwszy
  element wyniku wektorowego; test `diwali()` nie weryfikuje całego `diwali_days()`.
