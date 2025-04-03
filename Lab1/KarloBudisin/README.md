# Upravljanje višestrukim prekidima i njihovim prioritetima  

## Sadržaj
* Opis zadatka
* Komponente 
* Funkcionalnosti 
* Kod

## Opis zadatka
Sustav demonstrira obradu više prekida sa različitim prioritetima na Arduino Uno platformi. Implementirano je upravljanje prekidima od dva tipkala (visok i srednji prioritet), senzora udaljenosti HC-SR04 (najniži prioritet) i timer prekida (najviši prioritet). Svaki prekid ima svoju LED indikaciju.

## Komponente 

Ovaj projekt koristi sljedeće komponente:

* Arduino UNO
* Tipkalo(x2): Generira prekid na pritisak. 
* LED dioda(x4): Vizualna indikacija prekida. 
* HC-SR04 Ultrasonic Distance Sensor: senzor za mjerenje udaljenosti.

| Komponenta        | Količina | Pin na Arduino UNO  | Boja       | Prioritet  |
| ----------------- | -------- | ------------------- |------------|----------- |
| Arduino UNO       | 1        | -                   |      -     |      -     |
| Tipkalo (visok)   | 1        | 2                   | Crvena     | Visok      |
| Tipkalo (srednji) | 1        | 3                   | Plava      | Srednji    |
| Crvena LED        | 1        | 13                  | Crvena     |      -     |
| Plava LED         | 1        | 12                  | Plava      |      -     |
| Zelena LED        | 1        | 11                  | Zelena     |      -     |
| Žuta LED          | 1        | 10                  | Žuta       |      -     |
| HC-SR04           | 1        | TRIG: 7, ECHO: 6    |      -     | Najniži    |


## Funkcionalnosti 

* Timer prekid (najviši prioritet): Generira se svake sekunde i pali žutu LED diodu na 1 sekundu
* Visok prioritet (INT0): Aktivira se pritiskom na crveno tipkalo, pali crvenu LED na 1 sekundu
* Srednji prioritet (INT1): Aktivira se pritiskom na plavo tipkalo, pali plavu LED na 1 sekundu
* Senzor udaljenosti (najniži prioritet): Kontinuirano mjeri udaljenost i pali zelenu LED kada je objekt bliži od 100 cm
* Prioriteti: Timer > Visok > Srednji > Senzor
* Debounce mehanizam: Sprječava višestruko okidanje prekida od tipkala
* Vizualna indikacija: LED diode jasno prikazuju koji prekid je aktivan

### 4.3. Tablica funkcionalnosti

| ID    | Opis funkcionalnosti                                                                                 |
| ----- | ---------------------------------------------------------------------------------------------------- |
| FR-1  | Upravljanje LED-icama: Crvena (INT0), Plava (INT1), Zelena (senzor), Žuta (timer).                   |
| FR-2  | Detekcija pritiska na tipkala (INT0, INT1) i generiranje prekida.                                    |
| FR-3  | Mjerenje udaljenosti pomoću HC-SR04 senzora i generiranje prekida ako je udaljenost manja od 100 cm. |
| FR-4  | Konfiguracija Timer1 za generiranje prekida svake sekunde (najviši prioritet).                       |
| FR-5  | Obrada prekida prema prioritetima: Timer > INT0 > INT1 > senzor udaljenosti.                         |
| FR-6  | Vizualizacija aktivnih prekida pomoću odgovarajućih LED-ica.                                         |
| FR-7  | Debounce mehanizam (50ms) za tipkala kako bi se izbjeglo višestruko okidanje prekida.                |
| FR-8  | Automatsko gašenje LED-ica nižeg prioriteta prilikom aktivacije prekida višeg prioriteta.             |
| FR-9  | Kontinuirano praćenje udaljenosti sa intervalom od 100ms.                                            |
| FR-10 | Test LED-ova pri pokretanju sustava.                                                                 |
| FR-11 | Bljeskajuća zelena LED kao indikacija blizine predmeta (<100cm).                                     |


### Link za projekt na wokwi stranici je: https://wokwi.com/projects/427231197463397377

![image](https://github.com/user-attachments/assets/afbe46e1-09c4-4fda-b3ad-b3e48757e5b1)


