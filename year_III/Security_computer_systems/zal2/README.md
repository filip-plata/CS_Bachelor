## Opis plików

W pliku 'packets_AES.data' znajdują się dane z pakietów wysłanych na adres IP 35.205.64.152 atakującego.
Plik został uzyskany Wiresharkiem z otrzymanego dumpa, jest posortowany po czasie wysłania. Ten plik otrzymałem ręcznie wyklikując, nie mam skryptu.

W solve.py jest oczywiście skrypt rozwiazujący, bazujący na danych z pakietów. Rozszyfrowuje on pliki, uprzednio rozpakowywujace je z folderu.
Zatem należy umieścić Moje dokumenty.tgz obok skryptu solve.py, a następnie:

python solve.py

Musi być to python2.

## Opis rozwiązania

Zauważamy, że klucze do szyfrowania generowane są w zbyt prosty sposób, mają postać:
* M + i
dla i od 0 do 22, gdzie M to pierwszy z nich.

Dysponujemy ich szyframi, dla tego klucza publicznego
e wynosi 3, zatem znamy
* (M + i) ^ 3
Wystarczy więc ułożyć tożsamość algebraiczną, aby poznać samo M, np. z pierwszych trzech kluczy.
W kodzie jest też test przez wyznaczanie M za pomocą pierwszej, drugiej i czwartej wiadomości.

Kolejność kluczy znamy, gdyż mamy kolejność wysyłania pakietów oraz czas ostatniej modyfikacji pliku (to drugie do deszyfracji).
