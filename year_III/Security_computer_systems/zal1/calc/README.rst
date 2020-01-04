Duże zadanie zaliczeniowe #1 i #1* -- pwn
=========================================


W zadaniu należy wykonać atak na program ``calc.c``, będący kalkulatorem obliczającym wartości prostych wyrażeń.

Prostsza wersja zadania działa na serwerze ``h4x.0x04.net``, port ``1337``.
Ponieważ ``calc-easy`` jest programem działającym na standardowym wejściu
i wyjściu, jest on uruchomiony na serwerze za pomocą programu ``socat``,
który akceptuje połączenia sieciowe, uruchamia program i przekierowuje
stdin/stdout/stderr tego programu na przychodzące połączenie.  Celem zadania
jest poznanie zawartości pliku ``flag.txt`` znajdującego się w bieżącym
katalogu serwera.  Prostsza wersja zadania jest warta 11 punktów.

Trudniejsza wersja zadania działa na serwerze ``h4x.0x04.net``, port
``31337``.  Program ``calc-hard`` zawiera własną pętlę akceptującą połączenia,
więc nie używa programu ``socat``.  Celem zadania również jest poznanie
zawartości pliku ``flag.txt`` w bieżacym katalogu serwera.  Ta wersja jest warta
18 punktów.

Obydwa programy działają na systemie identycznym z tym używanym
w laboratoriach.  Paczka z zadaniem zawiera kod źródłowy zadania (wraz z informacją o sposobie kompilacji), kopię
biblioteki libc z systemu docelowego oraz skompilowany kod zadania
identyczny z tym działającym na serwerze.

W ramach rozwiązania zadania należy napisać skrypt atakujący jeden z wyżej wymienionych portów na serwerze ``h4x.0x04.net`` (wypisujący zawartość
pliku ``flag.txt`` na ekran).

Punkty częściowe za niepełne rozwiązania:

- 3 punkty za wyznaczenie wartości kanarka
- 3 punkty za wyznaczenie adresu bazowego libc

Zachęcamy do dyskusji na temat błędów w kodzie i możliwych ataków, ale
przypominamy, że skrypt atakujący należy napisać samodzielnie.

Rozwiązania zadań należy wysłać prowadzącemu grupę laboratoryjną drogą mailową **w terminie do dn. 14.12.2017** (godz. 23:59), a następnie zaprezentować je podczas zajęć (co może nastąpić już po upływie ww. terminu).

W razie problemów z działaniem serwera prosimy o kontakt na adres ``mwk (at) mimuw.edu.pl``.