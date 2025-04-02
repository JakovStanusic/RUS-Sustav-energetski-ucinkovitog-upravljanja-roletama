# Upravljanje višestrukim prekidima i prioritetima

## 1. Opis sustava
Sustav koristi prekide za obradu ulaznih signala s tipkala i senzora udaljenosti te generira vizualne indikacije pomoću LED dioda.

## 2. Funkcionalnosti
### 2.1. Rukovanje prekidima tipkala
Sustav mora detektirati pritiske triju tipkala (BUTTON0, BUTTON1, BUTTON2).
Svakom tipkalu mora biti dodijeljen prioritet:
BUTTON0 → Visoki prioritet (INT0)
BUTTON1 → Srednji prioritet (INT1)
BUTTON2 → Niski prioritet (INT2)
Kada se pritisne tipkalo, odgovarajuća LED (LED_INT0, LED_INT1, LED_INT2) mora titrati 1 sekundu.
Debounce mehanizam mora spriječiti višestruko aktiviranje prekida u kratkom vremenu.
### 2.2. Rukovanje prekidom tajmera
Tajmer (TIMER1) mora generirati prekid svake 1 sekunde.
Pri aktivaciji tajmera, LED_Timer mora svijetliti kratko kako bi signalizirala događaj.
### 2.3. Rukovanje senzorom udaljenosti (HC-SR04)
  Sustav mora mjeriti udaljenost pomoću senzora HC-SR04.
  Ako izmjerena udaljenost padne ispod 100 cm, LED_Sensor mora početi titrati svakih 200 ms.
  Mora se osigurati timeout ako pulseIn() ne vrati valjani rezultat.
### 2.4. Izvršavanje prioriteta prekida
Ako se istovremeno dogodi više prekida, mora se poštivati sljedeći prioritet:
Tajmer prekid (TIMER1) → Najviši prioritet
INT0 (BUTTON0) → Visoki prioritet
INT1 (BUTTON1) → Srednji prioritet
INT2 (BUTTON2) → Niski prioritet
Senzor udaljenosti (HC-SR04) → Najniži prioritet
### 2.5. Optimizacija performansi
Implementacija mora koristiti bezblokirajuće metode (millis()) umjesto delay(), gdje je moguće.
Svaka LED dioda mora imati zasebnu funkciju za treptanje unutar odgovarajuće ISR rutine.
#### 2.6. Realizacija i ispitivanje rada sustava
Sustav mora biti realiziran u Wokwi simulatoru.
Rad prekida mora biti snimljen i analiziran pomoću logičkog analizatora kako bi se potvrdilo ispravno izvršavanje prioriteta i pravovremena reakcija na događaje.

    
