---
Zadanie raport
---

Opis implementacji
---
Są to dwie oddzielne części: algorytm Brandesa - uwzględniający współbieżne modyfikowanie obliczonych wartości oraz template równoległej pętli for.
Wspomniany template parametryzowany jest ilością wątków i przydziela na każdy wątek tą samą liczbę wierzchołków (z dokłądnością do 1) i je uruchamia.
Dostęp do mapy przechowującej wyniki algorytmu Brandesa chroniony jest przez mutex, każdorazowo opuszczany przed medyfikacjami.

Przyspieszenie
---
Ilość wątków:

2: 1.437

3: 1.550

4: 1.674

5: 1.513

6: 1.608

7: 1.588

8: 1.491

Wzrost przyspieszenia dla ilości wątków do 4 nie dziwi, gdyż obliczenia były wykonywane na komputerze z 4 rdzeniami. Dalsze brak większych zmian czasu wykonania też nie dziwi: nie miało co wykonywać kod.
