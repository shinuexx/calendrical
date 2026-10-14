# Święta Bahá’í w roku gregoriańskim

Provider: `holiday_providers::bahai_based_calendar_holiday_provider`.
Używa backendów `calendars::bahai_calendar` (klasyczny) i
`calendars::astro_bahai_calendar` (astronomiczny).
[Legenda stanów i zasady źródeł](README.md).

## Stan świąt

Wszystkie funkcje poniżej przyjmują gregoriański `gyear`, zwracają pojedyncze RD
i są już dodawane przez `in_year()`. Nie są to przyszłe „Batch A”.

| Święto | Stan | Funkcja providera | Reguła w kodzie |
| --- | --- | --- | --- |
| Klasyczny Nowy Rok | **zaimplementowane** | `new_year` | Gregoriański 21 marca |
| Astronomiczny Naw-Rúz | **zaimplementowane** | `naw_ruz` | `m_astro->new_year_on_or_before(gregorian.get_new_year(gyear + 1))` |
| Riḍván, dzień 1 | **zaimplementowane** | `feast_of_ridvan` | `naw_ruz(gyear) + 31` |
| Riḍván, dzień 9 | **zaimplementowane** | `ridvan_9` | Astronomiczna data 3 / 2 (Jamal) |
| Riḍván, dzień 12 | **zaimplementowane** | `ridvan_12` | 3 / 5 (Jamal) |
| Declaration of the Bāb | **zaimplementowane** | `declaration_of_the_bab` | 4 / 8 (Azamat) |
| Ascension of Bahā’u’llāh | **zaimplementowane** | `ascension_of_bahaullah` | 4 / 13 (Azamat) |
| Martyrdom of the Bāb | **zaimplementowane** | `martyrdom_of_the_bab` | 6 / 17 (Rahmat) |
| Birth of the Bāb | **zaimplementowane** | `birth_of_the_bab` | Nów po kotwicy Naw-Rúz i porównanie z zachodem Słońca |
| Birth of Bahā’u’llāh | **zaimplementowane** | `birth_of_bahaullah` | `birth_of_the_bab(gyear) + 1` |
| Day of the Covenant | **zaimplementowane** | `day_of_the_covenant` | 14 / 4 (Qawl) |
| Ascension of ‘Abdu’l-Bahá | **zaimplementowane** | `ascension_of_abdul_baha` | 14 / 6 (Qawl) |

**Zaimplementowane:** prywatne `bahai_date_in_gregorian_year(gyear, month, day)`
bierze datę `naw_ruz()`, odzyskuje `major`, `cycle`, `year` przez backend astronomiczny
i konwertuje pełne `bahai_date{major, cycle, year, month, day}`. Nie używa skróconego
trzypolowego konstruktora z pierwotnego pseudokodu.

**Zaimplementowane:** `calendar()` bez argumentu zwraca backend klasyczny,
a `calendar(bool astro)` pozwala wybrać backend. `in_year()` obejmuje zarówno
klasyczny `new_year()`, jak i astronomiczny `naw_ruz()`; nie jest to przełącznik
między dwoma kompletnymi historycznymi zestawami świąt.

## Reguła urodzin w obecnej implementacji

Pseudokod **zaimplementowanej** funkcji `birth_of_the_bab()`:

```text
ny = naw_ruz(gyear)
set1 = astro.bahai_sunset(ny)
m1 = new_moon_on_or_after(set1)
m8 = new_moon_on_or_after(m1 + 190 days)
day = floor_to_rd(m8)
set8 = astro.bahai_sunset(day)
return day + (m8 < set8 ? 1 : 2)
```

`birth_of_bahaullah()` dodaje jeden dzień do tego wyniku, więc korzysta z tej samej
reguły kotwicy. **Wymaga ustalenia:** dodatkowe potwierdzenie zgodności granic
zachodu Słońca i interpretacji ósmego miesiąca z konkretnym źródłem tradycji;
obecność funkcji i testu nie jest potwierdzeniem wszystkich praktyk.

## Miesiące i nazwy

**Zaimplementowane:** `bahai_date` ma liczbowy miesiąc 1–19, a interkalacja jest
kodowana jako `bahai_based_calendar::ayyam_i_ha == 0`. Zwykłe miesiące mają 19 dni;
przeliczenie miesiąca 19 i interkalacji zależy od backendu.

**Wymaga ustalenia:** pełna transliteracja / tłumaczenia z pierwotnego fragmentu
tekstu. Lista poniżej zachowuje robocze nazwy; nie jest słownikiem parsera.

| Kod | Robocza nazwa | Kod | Robocza nazwa |
| --- | --- | --- | --- |
| 1 | Bahā’ | 11 | Mashīyyat |
| 2 | Jalāl | 12 | ‘Ilm |
| 3 | Jamāl | 13 | Qudrat |
| 4 | ‘Aẓamat | 14 | Qawl |
| 5 | Nūr | 15 | Masā’il |
| 6 | Raḥmat | 16 | Sharaf |
| 7 | Kalimāt | 17 | Sulṭān |
| 8 | Kamāl | 18 | Mulk |
| 9 | Asmā’ | 19 | ‘Alā’ |
| 10 | ‘Izzat | 0 | Ayyām-i-Hā |

**Proponowane:** nazwane aliasy miesięcy po uzgodnieniu pisowni.
Obecny parser `bahai_date` parsuje miesiąc jako liczbę.

## Historyczne warianty i serie obchodów

Poniższe wpisy zachowują pytania z pierwotnych notatek. Dat nie traktować jako
zatwierdzonej specyfikacji historycznej: oryginalny fragment tekstu nie jest dostępny.

| Obchody / pomysł | Stan | Zakres do sprawdzenia |
| --- | --- | --- |
| Klasyczny zestaw świąt poza Nowym Rokiem | **proponowane** | Oddzielny tryb lub `classic_*` API; dziś nie istnieje |
| Klasyczne Birth of the Bāb / Birth of Bahā’u’llāh | **wymaga ustalenia** | Robocze daty z notatek: 20 października / 12 listopada |
| Klasyczny Riḍván: dni 1, 9, 12 | **wymaga ustalenia** | Robocze daty: 21 kwietnia, 29 kwietnia, 2 maja |
| Klasyczna Declaration of the Bāb | **wymaga ustalenia** | Robocza data: 24 maja |
| Klasyczna Ascension of Bahā’u’llāh | **wymaga ustalenia** | Robocza data: 29 maja |
| Klasyczny Martyrdom of the Bāb | **wymaga ustalenia** | Robocza data: 10 lipca |
| Klasyczny Day of the Covenant / Ascension of ‘Abdu’l-Bahá | **wymaga ustalenia** | Robocze daty: 26 / 28 listopada |
| Osobny helper czwartego dnia Ayyām-i-Hā | **proponowane** | Brak metody providera |
| Historyczna reguła czwartego dnia Ayyām-i-Hā | **wymaga ustalenia** | Notatka podawała 1 marca / 29 lutego; zweryfikować backend i rok przestępny przed przyjęciem |
| Nineteen Day Feasts — początki miesięcy | **proponowane** | Lista wystąpień w roku gregoriańskim |
| Fast days — wszystkie dni miesiąca ‘Alā’ | **proponowane** | Osobna lista, nie pojedyncze święto |
| Dodawanie tych serii do `in_year()` | **wymaga ustalenia** | Opcjonalność i zakres listy; nie zwiększać jej automatycznie |

## Weryfikowalne odniesienia

- [API providera](../core/inc/holiday_providers/bahai_based_calendar_holiday_provider.hpp)
  i [implementacja](../core/src/holiday_providers/bahai_based_calendar_holiday_provider.cpp).
- [Wspólne API kalendarza](../core/inc/calendars/bahai_based_calendar.hpp),
  [klasyczna konwersja](../core/src/calendars/bahai_calendar.cpp),
  [astronomiczna konwersja i zachód Słońca](../core/src/calendars/astro_bahai_calendar.cpp),
  [parser](../core/inc/calendar_traits.hpp).
- [Testy](../tests/src/holidays/bahai_holidays.cc): CSV dla `new_year`, `naw_ruz`,
  `feast_of_ridvan`, `birth_of_the_bab`; `in_year` sprawdza niepustą, posortowaną listę
  w latach 2000–2010. Pozostałe daty nie mają tu osobnego porównania referencyjnego.
- [CSV](../all-holidays.csv): metadane CC dla Bahá’í New Year `(16.10), 277`,
  Naw-Rúz `(16.11), 277`, Riḍván `(16.12), 277`, Birth of the Bāb `(16.13), 278`.
