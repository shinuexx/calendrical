# Święta hebrajskie w roku gregoriańskim

Provider: `holiday_providers::hebrew_based_calendar_holiday_provider`.
Przyjmuje `calendars::hebrew_based_calendar`; CLI używa arytmetycznego
`calendars::hebrew_calendar` jako backendu `hebrew_based`.
[Legenda stanów i zasady źródeł](README.md).

## Rok i nazwy miesięcy

**Zaimplementowane:** argument `year` / `gyear` wymienionych niżej funkcji świąt
jest rokiem gregoriańskim. Niektóre komentarze nagłówka nadal mówią „Hebrew year”;
poniższy opis wynika z implementacji. Wyjątek: `birthday()` i `yahrzeit()` przyjmują
docelowy rok hebrajski; ich warianty `_in_gregorian()` przyjmują gregoriański.

Dla `e = gregorian.get_year(hebrew.epoch())` reguły jesienne używają
`hyear = gyear - e + 1`, a wiosenne / letnie `hyear = gyear - e`.
`in_gregorian(hmonth, hday, gyear)` przelicza lata `y`, `y+1`, `y+2`, gdzie `y`
jest rokiem backendu na początku zakresu gregoriańskiego, i filtruje wyniki.
To odrębne reguły — nie zastępować wszystkich helperów jednym offsetem.

**Zaimplementowane:** nazwy API to elementy `hebrew_month`: `nisan`, `iyyar`,
`sivan`, `tammuz`, `av`, `elul`, `tishri`, `marheshvan`, `kislev`, `tevet`, `shevat`,
`adar`, `adarii`. Purim korzysta z `last_month_of_year(hyear)`.

## Święta i reguły już obecne w providerze

| Święto | Stan | Funkcja | Reguła w kodzie |
| --- | --- | --- | --- |
| Rosh ha-Shanah | **zaimplementowane** | `rosh_ha_shanah` | Tishri 1, rok jesienny |
| Rosh ha-Shanah, dzień 2 | **zaimplementowane** | `rosh_ha_shanah_day_2` | Pierwszy dzień + 1 |
| Yom Kippur | **zaimplementowane** | `yom_kippur` | Tishri 10, rok jesienny |
| Sukkot | **zaimplementowane** | `sukkot` | Tishri 15 |
| Sukkot, diaspora dzień 2 | **zaimplementowane** | `sukkot_day_2_diaspora` | Tishri 16 |
| Hoshana Rabba | **zaimplementowane** | `hoshana_rabba` | Tishri 21 |
| Shemini Atzeret | **zaimplementowane** | `shemini_atzeret` | Tishri 22 |
| Simhat Torah | **zaimplementowane** | `simhat_torah` | Tishri 23, konwencja diaspory |
| Passover | **zaimplementowane** | `passover` | Nisan 15, rok wiosenny |
| Passover, diaspora dzień 2 | **zaimplementowane** | `passover_day_2_diaspora` | Nisan 16 |
| Koniec Passover | **zaimplementowane** | `end_of_passover` | Nisan 21 |
| Passover, diaspora dzień 8 | **zaimplementowane** | `passover_day_8_diaspora` | Nisan 22 |
| Shavuot | **zaimplementowane** | `shavuot` | Sivan 6 |
| Shavuot, diaspora dzień 2 | **zaimplementowane** | `shavuot_day_2_diaspora` | Sivan 7 |
| Purim | **zaimplementowane** | `purim` | Dzień 14 ostatniego miesiąca roku |
| Shushan Purim | **zaimplementowane** | `shushan_purim` | Purim + 1; jeśli sobota, dodatkowo + 1 |
| Ta’anit Esther | **zaimplementowane** | `ta_anit_esther` | Purim − 3, jeśli Purim w niedzielę; inaczej − 1 |
| Tzom Gedaliah | **zaimplementowane** | `tzom_gedaliah` | Tishri 3; sobota przesuwana o dzień |
| Tzom Tammuz | **zaimplementowane** | `tzom_tammuz` | Tammuz 17; sobota przesuwana o dzień |
| Tishah be-Av | **zaimplementowane** | `tishah_be_av` | Av 9; sobota przesuwana o dzień |
| Yom ha-Shoah | **zaimplementowane** | `yom_ha_shoah` | Nisan 27; tylko niedziela przesuwana o dzień w obecnym kodzie |
| Yom ha-Zikkaron | **zaimplementowane** | `yom_ha_zikkaron` | Iyyar 4; czwartek/piątek → poprzednia środa; niedziela → poniedziałek |
| Tzom Tevet | **zaimplementowane** | `tzom_tevet` | Lista `in_gregorian(tevet, 10, gyear)` |
| Hanukkah, pierwsze dni | **zaimplementowane** | `hanukkah` | Lista `in_gregorian(kislev, 25, gyear)` |
| Hanukkah, rozwinięcie 8 dni | **zaimplementowane** | `hanukkah_days` | Starty z `hanukkah(gyear)`, offsety 0–7, filtr roku, sortowanie i deduplikacja |
| Dni pośrednie Sukkot | **zaimplementowane** | `intermediate_days_of_sukkot` | Sześć kolejnych dni od Sukkot + 1 (Tishri 16–21) |
| Dni pośrednie Passover | **zaimplementowane** | `intermediate_days_of_passover` | Pięć kolejnych dni od Passover + 1 (Nisan 16–20) |
| Rosh Hodesh | **zaimplementowane** | `rosh_hodesh` | Dzień 1 miesiąca i poprzedni dzień, gdy poprzedni miesiąc ma 30 dni; filtr roku |
| Sh’ela | **zaimplementowane** | `sh_ela` | Koptyjski miesiąc 3 / dzień 26 |
| Birkath ha-Hama | **zaimplementowane** | `birhath_ha_hama` | Koptyjski 7 / 30, filtr `mod(coptic_year, 28) == 17` |

Pisownia funkcji to `birhath_ha_hama`, choć klucz CSV/testu używa
`birkath_ha_hama`. Nie wprowadzać drugiej nazwy API w pseudokodzie.
`in_year()` dodaje wszystkie powyższe obchody, używając `hanukkah_days()` zamiast
samej listy początków. Sortuje po dacie; nazwy etykiet odpowiadają kodowi.

## Reguły pomocnicze i ograniczenia

- **Zaimplementowane:** `rosh_hodesh()` odwiedza lata `hy-1` do `hy+2`, wyznacza
  dzień 1 oraz ewentualny poprzedni dzień, filtruje, sortuje i deduplikuje.
- **Zaimplementowane:** `birthday`, `yahrzeit` i ich warianty `_in_gregorian`
  obsługują rocznice z wejściową datą osoby; nie trafiają do ogólnego `in_year()`.
- **Zaimplementowane:** `samuel_season_in_gregorian` i `add_season_in_gregorian`
  zwracają momenty sezonów; nie są listą dodatkowych świąt.
- **Zaimplementowane:** `observational_hebrew_calendar::classical_passover_eve`
  jest funkcją kalendarza, nie tego providera; ma osobny test CSV.
- **Wymaga ustalenia:** czy pełne serie Hanukkah mają uwzględniać dni styczniowe
  ze startu w poprzednim roku gregoriańskim. Obecna funkcja bierze tylko starty
  znalezione przez `hanukkah(gyear)`; sam filtr nie uzupełnia brakującego startu.
- **Wymaga ustalenia:** docelowe warianty Izrael / diaspora i nakładanie się nazw
  na tych samych datach. Obecny provider dodaje wpisy diaspory bez przełącznika;
  zakresy dni pośrednich to powyższe zakresy kodu.
- **Wymaga ustalenia:** niezależne potwierdzenie wszystkich współczesnych reguł
  przesunięć, m.in. pełnej polityki Yom ha-Shoah. Opis obecnego warunku nie jest
  zapewnieniem kompletności zasad religijnych lub administracyjnych.
- **Wymaga ustalenia:** znaczenie lokalnego początku obchodów i wariantów
  obserwacyjnych. Provider zwraca RD, nie godzinę zachodu Słońca; nie zakładać
  identyczności reguł miesięcy backendów arytmetycznego i obserwacyjnego.
- **Proponowane:** testy brzegów roku i danych referencyjnych dla rozszerzonych świąt.

## Weryfikowalne odniesienia

- [API providera](../core/inc/holiday_providers/hebrew_based_calendar_holiday_provider.hpp)
  i [implementacja](../core/src/holiday_providers/hebrew_based_calendar_holiday_provider.cpp).
- [Nazwy miesięcy](../core/inc/hebrew_month.hpp),
  [wspólne API kalendarzy](../core/inc/calendars/hebrew_based_calendar.hpp),
  [backend arytmetyczny](../core/src/calendars/hebrew_calendar.cpp),
  [backend obserwacyjny](../core/src/calendars/observational_hebrew_calendar.cpp).
- [Testy główne](../tests/src/holidays/hebrew_holidays.cc): CSV dla Yom Kippur,
  Passover, Purim, Ta’anit Esther, Tishah be-Av, Yom ha-Zikkaron, Sh’ela, początków
  Hanukkah i Birkath ha-Hama; dodatkowo niepusta, posortowana lista 2000–2010.
- [Test obserwacyjny](../tests/src/holidays/observational_hebrew_holidays.cc)
  dotyczy `classical_passover_eve()`.
- [CSV](../all-holidays.csv): metadane CC `(8.30)`–`(8.38)` dla odpowiednich wierszy
  powyższych pojedynczych reguł, `(8.43)` dla pierwszego dnia Hanukkah i `(18.25)`
  dla Classical Passover Eve. Te odniesienia nie walidują całej listy ani wszystkich
  wariantów regionalnych. [Loader](../tests/inc/holiday_csv_loader.hpp) porównuje
  tylko pierwszy element wyniku wektorowego z datą w CSV.
