# Reguły świąt i notatki projektowe

Ten indeks obejmuje dziesięć dokumentów wydzielonych w ramach [issue #5](https://github.com/shinuexx/calendrical/issues/5).
Zestaw źródłowy pochodzi z brancha `wip/akan-mayan-cyclic-and-interactive-cli`,
commit `d6fe7f6952fd7ada2c433402569f13f6fd7cd060`. Opisy implementacji zweryfikowano
względem `master` na commicie `ea0790376041c48680a20b28229efb5745fabd0c`,
po scaleniu [PR #3](https://github.com/shinuexx/calendrical/pull/3).
Przy kolejnych zmianach kodu należy zaktualizować odpowiednią regułę i jej stan.

## Dokumenty

| Temat | Dokument | Główna zawartość |
| --- | --- | --- |
| Klasyczny ormiański | [Święta simple calendar](armenian-simple-holidays-pseudocode.md) | Navasard i zakres ogólnego providera |
| Bahá’í | [Święta i warianty](bahai-holidays-pseudocode.md) | Klasyczny Nowy Rok, astronomiczne święta, urodziny, propozycje serii |
| Koptyjski / etiopski | [Miesiące i święta](coptic-months-notes.md) | Liczbowe miesiące, stałe święta, robocze aliasy |
| Hebrajski | [Święta i reguły](hebrew-holidays-pseudocode.md) | Przesunięcia, diaspora, serie, pomocnicze reguły koptyjskie |
| Hinduski księżycowy | [Święta i tithi](hindu-lunar-holidays-pseudocode.md) | Reguły miesiąc/dzień, pora dnia, wielodniowe obchody |
| Hinduski słoneczny | [Święta słoneczne](hindu-solar-holidays-pseudocode.md) | Sowramana Ugadi, Ayyappa Jyothi Darshanam, Mesha Samkranti |
| Islamski | [Miesiące i święta](islamic-months-notes.md) | Tablicowe długości miesięcy, święta, warianty backendów |
| Perski | [Święta i mapowanie lat](persian-holidays-pseudocode.md) | Nowruz, stałe daty, środa przed Nowruz |
| Samarytański | [Święta i reguły ruchome](samaritan-holidays-pseudocode.md) | Zakresy dni, Pentecost i Ṣimmut |
| Tybetański | [Święta i leap month/day](tibetan-holidays-pseudocode.md) | Losar, zakresy, obecna polityka flag przestępnych |

## Znaczenie stanów

- **zaimplementowane** — funkcja lub reguła istnieje w wskazanym kodzie. Nazwa API,
  sposób liczenia i backend wynikają z implementacji; stan nie oznacza niezależnego
  potwierdzenia poprawności wszystkich tradycji.
- **proponowane** — pomysł na rozszerzenie, którego nie ma w opisanym API. Nie jest
  uzgodnionym zobowiązaniem do implementacji ani gotową specyfikacją.
- **wymaga ustalenia** — brak potwierdzonego źródła, decyzji o wariancie lub kontrakcie.
  Jeżeli kod już przyjął konkretny wariant, dokument opisuje go oddzielnie jako
  zaimplementowany, a pytanie o docelową konwencję pozostawia otwarte.

Stan dotyczy konkretnego święta lub reguły, a nie całego kalendarza. Pseudokod
z etykietą „zaimplementowane” streszcza obecny algorytm; nie jest nową specyfikacją.
Argument `gyear` oznacza rok gregoriański. Providerzy zwracają daty RD
(`fixed_date_t`), a nie godziny rozpoczęcia obchodów, chyba że wyraźnie wskazano
pomocniczą funkcję zwracającą `moment_t`.

## Źródła i granice weryfikacji

Podstawą opisu implementacji są podlinkowane nagłówki, źródła kalendarzy i providerów
oraz testy. Oryginalne notatki odwoływały się także do obrazów i fragmentów tekstu,
których nie ma w tym zestawie. Zachowane z nich nazwy, historyczne daty i postulaty
są oznaczone jako robocze lub wymagające ustalenia, nie jako specyfikacja religijna.

[Zbiór `all-holidays.csv`](../all-holidays.csv) zawiera daty 2000–2103 i kolumnę
`Func., page` z odniesieniami do *Calendrical Calculations*. Numery przytoczone
w dokumentach są weryfikowalne jako metadane tego pliku. Nie ustalono tu wydania
książki ani nie potwierdzono jej tekstu; nie przypisywać tych numerów innemu wydaniu
bez sprawdzenia. Pozycje bez takiego odniesienia korzystają z dowodu w kodzie,
a nie z dopisanych numerów funkcji książki.

[Testy świąt](../tests/src/holidays/) i [loader CSV](../tests/inc/holiday_csv_loader.hpp)
mają różne poziomy pokrycia:

- porównanie CSV sprawdza wskazaną metodę dla lat z pliku;
- dla wyniku wektorowego loader porównuje tylko **pierwszą datę** albo `none`;
- test niepustości / sortowania nie potwierdza niezależnie daty święta, długości
  pełnej serii, reguły przesunięcia ani wariantu regionalnego.

Przy dalszych pracach należy zachować to rozróżnienie i dodawać źródła oraz testy
zgodne z konkretną regułą. Ten PR zmienia dokumentację, nie implementację ani testy.
