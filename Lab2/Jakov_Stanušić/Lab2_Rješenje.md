# 1. Inicijalni zadatak
U ovome zadatku potrebno je implementirati i prikazati korištenje sleep mode funkcija na arduino MEGA sustavu.
Također ću istražiti različite vrste sleep modova i testirati ograničenja svakoga.

Na kraju izrade projekta trebao bi imati sustav u kojemu je moguće vidjeti i lako prikazati kako radi sleep mode i njegovo paljenje/gašenje.

# 2. Opis projekta
Ovaj projekt implementira pametni sustav za upravljanje energijom koristeći sleep modeove na Arduino Mega platformi. 
Projekt je zamišljen tako da kada se pokrene sustav se prvo upali LED dioda na pinu 13. Nakon 5 sekundi sustav zatim prelazi u energetski efikasni sleep mode.
Sustav ostaje u "modu spavanja" sve dok se ne pritisne tipkalo koje je spojeno na pin 2 ili dok ne prođe 8 sekundi od početka sleep moda.

Izlazak iz sleep moda je implementiran kao prekid na način da kada se pritisne tipkalo ili istekne 8 sekundi "zastavica" se postavi u 'True' te se prekida sleep mode i šalje nazad u void loop() gdje se zatim u if petlji provjerava da je se prekid dogodio i pali se LED dioda na 5 sekundi. Nakon tih 5 sekundi ponovno se LED dioda gasi i pali se sleep mode.

# 3. Funkcionalnosti

## Opis funkcionalnosti projekta

| ID    | Opis funkcionalnog zahtjeva           | Detalji implementacije | Način provjere |
|-------|---------------------------------------|------------------------|----------------|
| FR-1  | Upravljanje LED diodom (pin 13)       | `pinMode(ledPin, OUTPUT)`, `digitalWrite()` | Vizualna provjera stanja LED |
| FR-2  | Detekcija pritiska tipkala (INT5, pin 2) | `attachInterrupt()` s FALLING okidanjem | Test pritiska tipkala |
| FR-3  | Funkcionalnost sleep moda             | Funkcija `enterSleepMode()` s mjerenjem vremena | Ispis Serial monitora |
| FR-4  | Automatsko buđenje nakon 8 sekundi    | Usporedba `millis()` u glavnoj petlji | Mjerenje vremena |
| FR-5  | Ručno buđenje (prekid tipkala)        | Prekidna rutina `wakeUpISR()` | Test reakcije na prekid |
| FR-6  | Debounce mehanizam za tipkalo         | Debounce 200ms u ISR rutini | Test višestrukih brzih pritisaka |
| FR-7  | Praćenje stanja sustava preko Serijala | Ispisi `Serial.println()` | Provjera Serial monitora |
| FR-8  | Period aktivacije LED od 5 sekundi    | Pozivi `delay(5000)` | Vremenska provjera |
| FR-9  | Indikacija stanja (LED ON/OFF)        | Vizualna povratna informacija | Vizualna inspekcija |
| FR-10 | Reset stanja nakon buđenja            | Postavljanje zastavice `wakeUp = false` | Ponovno paljenje sleep moda |

# 4. Ograničenja pojedinog sleep mode-a

| Vrsta Sleep Mode              | Oznaka                   | Tipična potrošnja | Uređaji koji se "uspavljuju" | Načini buđenja                     | Ograničenja |
|-------------------------|--------------------------|-------------------|---------------------|------------------------------------|-------------|
| **Idle**               | `SLEEP_MODE_IDLE`       | ~15 mA           | Svi osim CPU        | Svi prekidi                        | Najmanja ušteda energije |
| **ADC Noise Reduction**| `SLEEP_MODE_ADC`       | ~10 mA           | ADC                 | ADC prekid                         | Samo ADC može buditi |
| **Power-down**         | `SLEEP_MODE_PWR_DOWN`  | ~5 μA            | WDT, pin prekidi    | Eksterni reset, WDT, INT/PCINT     | Najdublji sleep, ograničeni izvori buđenja |
| **Power-save**         | `SLEEP_MODE_PWR_SAVE`  | ~7 μA            | Timer2, async timer | Timer2, INT/PCINT                  | Kompleksnija konfiguracija |
| **Standby**            | `SLEEP_MODE_STANDBY`   | ~10 μA           | Crystal oscilator   | Eksterni reset, WDT                | Potreban eksterni oscilator |
| **Extended Standby**   | `SLEEP_MODE_EXT_STANDBY`| ~12 μA          | Crystal, async timer| Timer, INT/PCINT                   | Najveća potrošnja od "dubokih" sleepova |


# 5. Što sam naučio radeći ovaj projekt
### - Integrirati prekida na Arduino MEGA
### - Integraciju sleep moda u sustav
### - Detektirati i koristiti različite vrste sleep mode-a kao što su idle, power down, power save, standby...
### - Implementirati unutarnji tajmer kako bi se pratilo vrijeme koje sustav provede u sleep modu
### - Korištenje prekida za izlazak iz sleep mode-a
