# Projekt na Programowanie Niskopoziomowe
### Gra w stylu Wolfensteina 3D
Wykorzystując język C i bibliotekę SDL mam zamiar stworzyć prototyp gry w stylu [Wolfensteina 3D](https://en.wikipedia.org/wiki/Wolfenstein_3D).
Będzie ona wykorzystywała technikę [ray castingu](https://pl.wikipedia.org/wiki/Ray_casting), w celu iluzji  trój wymiarowego świata na podstawie mapy 2D. 

Wybrałem ten konkretny projekt ponieważ interesują mnie zagadnienia związane z grafiką komputerową, więc dobrym wyzwaniem byłby zrozumienie algorytmów i próba odtworzenia tego jak była tworzona w dawniejszych czasach.

## Cele projektu
- program który będzie przedstawiał iluzję świata 3D przy wykorzystaniu algorytmu raycastingu
- wizualizacja przestrzeni zarówno w 3D jak i w "prawdziwym" 2D.
- oteksturowane ściany możliwe do wczytania z plików graficznych
- możliwość wczytywania mapy za pomocą pliku tekstowego


## Algorytm raycastingu
Podstawą algorytmu jest wypuszczanie promienia dla każdego piksela ekranu, sprawdzenie w jakim punkcie i w jakiej odległości od wirtualnej kamery napotkał on ścianę, a następnie narysowanie tej kolumny pikseli w wysokości zależnej od odległości. Im ściana jest bliżej, tym większą wertykalną część ekranu będzie zajmowała.

Naiwną implementacją sprawdzania czy promień dotarł do ściany byłaby ciągła inkrementacja jego długości o małe wartości tak długo aż nie znalazłby się w bloku mapy na którym stoi ściana, lub osiągnąłby jakąś maksymalną długość.

Jednak sposób ten wymaga wielu operacji dodawania i sprawdzania mapy dla każdego z setek lub nawet tysięcy promieni. Można ten proces mocno zoptymalizować korzystając z faktu, że świat jest przedstawiony za pomocą siatki dyskretnych bloków, przez to potencjalna ściana może się znajdować jedynie w określonych przez tą siatkę pozycjach. Dzięki temu promień może podróżować o długości jednostki tej siatki i sprawdzać istnienie potencjalnej ściany na granicy dwóch bloków.

Oprócz wyznaczenia koordynatów zetknięcia, ważnym krokiem jest obliczenie odległości tego punktu do gracza. Właśnie dzięki temu krokowi jest możliwe stworzenie iluzji perspektywy poprzez rysowanie dalszych ścian jako mniejsze, a bliższych większe.

## Tekstury
Żeby ściany ściany mogły zostać oteksturowane dla każdego promienia najpierw trzeba wyznaczyć koordynat X tekstury na podstawie koordynatu zetknięcia się z ścianą w świecie. Jako, że jeden promień odpowiada jednej kolumnie pixeli ekranu, będzie więc wykorzystywał tylko jeden koordynat X tekstury. Do tego wykorzystywany jest fakt, że świat jest stworzony na siatce. Po przeprowadzeniu operacji koordynat pozycji równoległy do kierunku ściany modulo rozmiar jednego pola otrzymamy wartość od 0 do maksymalnej wielkości pola, dla miejsca kontaktu. Wystarczy przeskalować tą wartość przez iloraz wielkości bloku do wielkości tekstury, i dostanie się koordynat x tekstury.

Dla większości promieni cała wysokość ściany będzie w widoku, więc koordynaty y tekstury będą rysowane od 0 do ostatniego. Jednak jeśli ściana jest na tyle blisko, że nie znajduje się cała w polu widzenia, trzeba obliczyć koordynat startowy y. 
Niezależnie od tego trzeba obliczyć stosunek wielkości tekstury do wysokości lini jaka będzie narysowana dla danego promienia na ekranie. ta wartość powie o ile mamy zwiększyć koordynat y tekstury na każdy piksel w dół ekranu.
Na koniec trzeba obliczyć różnicę między górą ekranu a początkiem rysowania lini, jest to połowa różnicy wysokości ekranu i wysokości lini.
Teraz wystarczy iterować przez tyle pikseli jak wysoka jest linia, pobrać kolor tekstury, narysować piksel w odpowiednich koordynatach i zaktualizować koordynat y textury.