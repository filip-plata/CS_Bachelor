## Opis rozwiązania

Główna część rozwiązania w project.m.
Schemat Rungego-Kutty wydzielony jest do osobnego pliku - rungeKutta.m.

Równanie różniczkowe wybucha dla t trochę większego od 14 - prędkość
i położenie zbiegają do minus nieskończoność.
Potwierdza to funkcja lsode.

Krok był stale zwiększany, aż wyniki stały się wizualnie powtarzalne.

Te same rezultaty zostały osiągnięte za pomocą schematu Eulera. Wystarczy zmienić nazwę funkcji używanej do obliczania rozwiązań równania.

### Poprawność dla zerowych A, B

Dla A = 0, B = 0 równanie można rozwiązać - otrzymujemy równanie

x'' + a*x' + b * x = 0

które można rozwiązać analitycznie. W szczególności można znaleść je w internecie.
Wtedy dokładność rozwiązania można sprawdzać porównując z rozwiązaniem dokładnym.

Ostatnia część wykresu to porównanie rozwiązania dokładnego z numerycznym.

Rozwiązanie dokładne zostały uzyskane ze strony projektu Wolfram.

Aby ujrzeć różnicę należy znacznie zwiększyć rozdzielczość (Ctrl + kółko na myszce), gdyż różnice są rzędu 0.00004 pomiędzy oboma wykresami.
