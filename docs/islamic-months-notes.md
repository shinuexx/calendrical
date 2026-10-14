# Miesiące i święta islamskie

Provider: `holiday_providers::islamic_based_calendar_holiday_provider`.
Przyjmuje dowolny `calendars::islamic_based_calendar`; CLI wiąże `islamic_based`
z tablicowym `calendars::islamic_calendar`.
[Legenda stanów i zasady źródeł](README.md).

## Miesiące i warianty kalendarza

**Zaimplementowane:** w tablicowym `islamic_calendar` miesiące nieparzyste mają
30 dni, parzyste 29, a miesiąc 12 ma 30 dni w roku przestępnym.
`is_leap_year(year)` sprawdza `mod(14 + 11 * year, 30) < 11`.
Nie przenosić tej tabeli długości na backendy obserwacyjne.

**Wymaga ustalenia:** poniższe nazwy są roboczym słownikiem z pierwotnych notatek;
nie stanowią implementacji aliasów ani zweryfikowanej specyfikacji transliteracji.

| Numer | Robocza nazwa | Dni w backendzie tablicowym |
| --- | --- | --- |
| 1 | Muḥarram | 30 |
| 2 | Ṣafar | 29 |
| 3 | Rabī‘ I / Rabī‘ al-Awwal | 30 |
| 4 | Rabī‘ II / Rabī‘ al-Āḥir | 29 |
| 5 | Jumādā I / Jumādā al-Ūlā | 30 |
| 6 | Jumādā II / Jumādā al-Āḥira | 29 |
| 7 | Rajab | 30 |
| 8 | Sha‘bān | 29 |
| 9 | Ramaḍān | 30 |
| 10 | Shawwāl | 29 |
| 11 | Dhu al-Qa‘da | 30 |
| 12 | Dhu al-Ḥijja | 29 lub 30 |

**Proponowane:** parser nazw miesięcy. **Wymaga ustalenia:** pisownia wyjściowa,
aliasy ASCII, diakrytyki i warianty apostrofów. Obecny parser `simple_date` używa liczb.

## Święta w obecnym providerze

Wszystkie funkcje przyjmują gregoriański `gyear`, zwracają
`std::vector<fixed_date_t>` i są uwzględnione w `in_year()`.

| Święto | Stan | Funkcja | Reguła liczbowa |
| --- | --- | --- | --- |
| Islamski Nowy Rok | **zaimplementowane** | `islamic_new_year` | `in_gregorian(1, 1, gyear)` |
| ‘Ashūrā’ | **zaimplementowane** | `ashura` | 1 / 10 |
| Mawlid | **zaimplementowane** | `mawlid` | 3 / 12 |
| Lailat al-Mi‘rāj | **zaimplementowane** | `lailat_al_miraj` | 7 / 27 |
| Lailat al-Barā’a | **zaimplementowane** | `lailat_al_baraa` | 8 / 15 |
| Początek Ramadanu | **zaimplementowane** | `ramadan` | 9 / 1 |
| Lailat al-Qadr | **zaimplementowane** | `lailat_al_qadr` | 9 / 27 |
| Eid ul-Fitr | **zaimplementowane** | `eid_ul_fitr` | 10 / 1 |
| Eid ul-Adha | **zaimplementowane** | `eid_ul_adha` | 12 / 10 |

**Zaimplementowane:** `in_gregorian(imonth, iday, gyear)` wyznacza rok islamski
na początku zakresu gregoriańskiego, przelicza `y-1`, `y`, `y+1` i filtruje wyniki.
Wynik jest listą wystąpień, nie obietnicą jednej daty na rok gregoriański.

## Otwarte decyzje

- **Wymaga ustalenia:** warianty obchodów zależne od tradycji, kraju lub lokalnej
  obserwacji / proklamacji. Obecne stałe miesiąc/dzień opisują kod, nie wszystkie praktyki.
- **Wymaga ustalenia:** modelowanie początku obchodów poprzedniego wieczoru.
  Provider zwraca cywilne etykiety RD; nie wyznacza godziny rozpoczęcia święta.
- **Proponowane:** opcjonalna konfiguracja wariantów świąt, dopiero po wskazaniu źródeł.

## Weryfikowalne odniesienia

- [API providera](../core/inc/holiday_providers/islamic_based_calendar_holiday_provider.hpp)
  i [implementacja](../core/src/holiday_providers/islamic_based_calendar_holiday_provider.cpp).
- [Interfejs backendów](../core/inc/calendars/islamic_based_calendar.hpp),
  [backend tablicowy](../core/src/calendars/islamic_calendar.cpp),
  [backend obserwacyjny](../core/inc/calendars/observational_islamic_calendar.hpp),
  [parser](../core/inc/calendar_traits.hpp).
- [Testy](../tests/src/holidays/islamic_holidays.cc): CSV dla `mawlid()`, niepuste
  wyniki pozostałych świąt i sortowanie w latach 2000–2010; używają backendu tablicowego.
- [CSV](../all-holidays.csv): `Mawlid` ma metadaną CC `(7.6), 109`.
