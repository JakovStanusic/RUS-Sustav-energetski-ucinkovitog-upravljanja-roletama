# Upravljanje Potrošnjom Energije Mikrokontrolera uz Korištenje Sleep Modova

## 1. Opis sustava
Sustav koristi Arduino mikrokontroler za upravljanje potrošnjom energije kroz:
  - Power-down sleep mode za minimalnu potrošnju
  - Watchdog timer (WDT) za automatsko buđenje nakon ~4 sekunde
  - Vanjski prekid na tipki za ručno buđenje

## 2. Funkcionalnosti

### 2.1.Upravljanje Sleep Modom
   - Koristi najdublji sleep mod (Power-down)
   - Isključuje sve nepotrebne periferije
   - Potrošnja struje pada na minimalne vrijednosti
   - Samo prekidi mogu probuditi mikrokontroler

### 2.2. Watchdog Timer (WDT)
 - Konfiguriran kao prekid (ne reset)
 - Automatski budi MCU nakon timeouta
 - Ne zahtijeva dodatne komponente
 - Osigurava pouzdano buđenje po vremenu
   
### 2.3. Button Prekid
  - vanjski prekid na tipki za buđenje mikrokontrolera iz sleep moda (PIN 2)
  - uključen interni pull-up otpornik
  - trenutno buđenje iz sleep moda
  - Vizualna povratna informacija preko LED (pin 13)

### 2.4. Izlazak iz sleep moda
Dvije primarne metode buđenja:

  ### Prekid tipke:

  - Ručno buđenje pritiskom na tipku

  - LED se upali odmah
  
  - Resetira WDT brojač

  ### Watchdog Timer:

  - Automatsko buđenje nakon ~4 sekunde

  - Nije potrebna korisnička interakcija

  - Sustav se vraća u sleep nakon obrade

### 2.5. Optimizacija potrošnje energije
- SLEEP_MODE_PWR_DOWN - minimalno stanje potrošnje

- Sve neiskorištene periferije isključene

- Kratki aktivni periodi (5s LED upaljen)

- Dugi periodi spavanja (do prekida)

- Struja pada sa ~20mA (aktivno) na ~5μA (sleep)


### 2.6. Realizacija i testiranje rada sustava
Sustav je u potpunosti realiziran na Arduino Mega 2560 platformi korištenjem sljedećih komponenti:

- Digitalni izlaz: Pin 13 za LED diodu s 220Ω ograničavajućim otpornikom

- Digitalni ulaz: Pin 2 za tipku s pull-up otpornikom

- Watchdog timer: Interni WDT konfiguriran na 4 sekunde
## 3. Funkcionalni zahtjevi

| ID   | Zahtjev | Implementacija u Kodu | Implementacija u Shemi |
|------|---------|-----------------------|------------------------|
| **FR-1**  | Detekcija pritiska tipke | `attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonWakeUp, FALLING)` | Tipka spojena na pin 2 (`btn1:1.r → mega:2`) |
| **FR-2**  | Buđenje i treptanje LED | `buttonPressed` flag i `digitalWrite(LED_PIN, HIGH)` | LED spojen na pin 13 (`led1:A → mega:13`) |
| **FR-3**  | LowPower biblioteka | `#include <avr/sleep.h>` i `set_sleep_mode(SLEEP_MODE_PWR_DOWN)` | Nema eksternih komponenti |
| **FR-4**  | WDT buđenje (4s) | `WDTCSR = (1<<WDIE)\|(1<<WDP3)` | Nema eksternih komponenti |
| **FR-5**  | Power-down mod | `SLEEP_MODE_PWR_DOWN` | Nema eksternih komponenti |
| **FR-6**  | Reakcija na tipku | `buttonWakeUp()` funkcija | Tipka spojena na GND (`btn1:2.r → mega:GND.1`) |
| **FR-7**  | Testiranje potrošnje | Serial monitor output | 220Ω otpornik za LED (`r1`) |
| **FR-8**  | Prioritet WDT | `if(!buttonPressed)` u `ISR(WDT_vect)` | Nema eksternih komponenti |
| **FR-9**  | Minimalna potrošnja | `sleep_cpu()` i BOD isključen | Nema nepotrebnih komponenti |
| **FR-10**  | Testiranje funkcionalnosti | Serial debug poruke (`Woke up by...`) | Kompletna shema u Wokwi |

## 4. Zaključak
Implementirani sustav upravljanja sleep modom na Arduino Mega 2560 platformi pokazao je izuzetnu učinkovitost u upravljanju potrošnjom energije, uz istovremeno održavanje pouzdane funkcionalnosti. 

Implementacija zadovoljava sve postavljene funkcionalne zahtjeve, s posebnim naglaskom na:
- Izuzetno nisku potrošnju u sleep modu

- Trenutno buđenje na korisnički input

- Visoku vremensku preciznost automatskog buđenja

- Potpunu stabilnost u produženom radu

  ### Link za projekt na wokwi stranici je: [https://wokwi.com/projects/427231197463397377](https://wokwi.com/projects/427749360394723329)
  ![lab2](https://github.com/user-attachments/assets/ebaa5bfd-e2d7-4732-a423-bea68e83ae6a)
