# Upravljanje višestrukim prekidima i njihovim prioritetima  

# Cilj zadatka  
-  Proučiti koncept višestrukih prekida i njihovih prioriteta na odabranom procesoru  
-  Implementirati program koji efikasno upravlja različitim događajima koristeći odgovarajuće strategije rukovanja prekidima  

# Zadatak  

## 1. Implementacija višestrukih interrupta  
 ### Definirati i implementirati više prekidnih rutina (ISR – Interrupt Service Routines) koje će reagirati na različite događaje:  
  - Pritisak različitih tipkala
    ##### implementirane 2 prekidne rute za 2 različita tipkala
    ##### svaka rutina postavlja odgovarajuću zastavicu (highPriorityEvent ili   mediumPriorityEvent)
    ##### dodan debounce mehanizam kako bi se izbjegli lažni prekidi
    ##### prekidi se aktiviraju na silazni brid signala (FALLING)
    ```cpp
      attachInterrupt(digitalPinToInterrupt(BUTTON_HIGH_PIN), highPriorityISR, FALLING);
      attachInterrupt(digitalPinToInterrupt(BUTTON_MEDIUM_PIN), mediumPriorityISR, FALLING);
    ```
  - Očitavanje različitih senzora
     ##### senzor HC-SR04 koristi se za detekciju objekata
     ##### measureDistance() funkcija šalje ultrazvučni impuls i mjeri vrijeme povratka
     ##### vraća udaljenost u centimetrima (999.0 ako nema detekcije)
     ##### očitavanje se vrši periodički u glavnoj petlji, a ne kroz prekidnu rutinu
     ```cpp
      float measureDistance() {
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);
  
        long duration = pulseIn(ECHO_PIN, HIGH, 30000);
          if (duration == 0) {
            return 999.0;
        }
        return duration * 0.034 / 2;
    }
    ```

## 2. Postavljanje prioriteta prekida  
- Postaviti različite prioritete za prekide kako bi važniji događaji imali prednost pri obradi  
     ##### visokoprioritetni prekid (tipkalo na pin 2) ima apsolutni prioritet
     ##### srednjeprioritetni prekid (tipkalo na pin 3) čeka dok se ne obrade visokoprioritetni događaji
     ##### senzori se obrađuju samo kada nema aktivnih prekida s višim prioritetima
   ```cpp
       if (highPriorityEvent) {
    highPriorityEvent = false;
    digitalWrite(LED_HIGH_PIN, HIGH);
    digitalWrite(LED_SENSOR_PIN, LOW); 
    ledHighTimer = currentMillis;
  }

 
  if (mediumPriorityEvent && !highPriorityEvent) {
    mediumPriorityEvent = false;
    digitalWrite(LED_MEDIUM_PIN, HIGH);
    digitalWrite(LED_SENSOR_PIN, LOW); 
    ledMediumTimer = currentMillis;
  }
    ```

## 3. Efikasno upravljanje resursima  
-  Spriječiti konflikte pristupa resursima korištenjem odgovarajućih mehanizama (semafori, kritične sekcije, zastavice)
-  Minimizirati vrijeme izvršavanja ISR funkcija kako bi se izbjegle blokade drugih prekida
   #### korištenje volatile varijabli za sigurnu komunikaciju ISR-glavni program
   #### debounce mehanizam sprječava višestruke aktivacije prekida zbog titranja kontakata
   #### kratke ISR rutine minimiziraju vrijeme blokade drugih prekida
 ```cpp
    volatile bool highPriorityEvent = false;
    volatile bool mediumPriorityEvent = false;
    volatile unsigned long lastHighTime = 0;
    volatile unsigned long lastMediumTime = 0;
    const int debounceDelay = 50;
 ```

## 4. Demonstracija rada s vanjskim sklopovima  
-  Koristiti dodatne vanjske sklopove (senzore, tipkala, LED diode, serijske module, eksterne kontrolere)
   #### Ultrazvučni senzor HC-SR04 koji služi za mjerenje udaljenosti
   #### 2 tipkala  (Tipkalo 1 (INTO), Tipkalo 2 (INT1))
   #### 3 LED diode
   
-  Implementirati logiku koja pokazuje kako različiti prioriteti utječu na obradu događaja  
  1. `INT0` prekid  - Najviši prioritet
   - ima najviši prioritet, prekida sve ostale aktivnosti
   - obavezno se obrađuje odmah
  ```cpp
    attachInterrupt(digitalPinToInterrupt(BUTTON_HIGH_PIN), highPriorityISR, FALLING);
   ```
  - **Primjer:** Crvena dioda će uvijek treptati kada pritisnemo crveni gumb .
 2. `INT1` prekid  - Srednji prioritet
   - obrađuje se ako nema aktivnog Echo preida, ože prekinuti samo Timer1 prekid
   ```cpp
    attachInterrupt(digitalPinToInterrupt(BUTTON_MEDIUM_PIN), mediumPriorityISR, FALLIN
   ```
  - **Primjer:** Plava dioda će treptati samo ako nije crvena aktivirana .
 3. `Timer1`  prekid - Najniži prioritet
   - generira se svake sekunde (1HZ), može biti odgođen zbog prekida s višim prioritetom
   ```cpp
      if (!highPriorityEvent && !mediumPriorityEvent){
      //očitavanje senzora
    }
   ```
 - **Primjer:** Zelena dioda će treptati ako je unutar dobre distance , i ako se ne dogodi drugi prekid pošto je ona s najmanjim prioritetom.    
## 5. Dokumentacija i testiranje  
-  Jasno dokumentirati način rada programa, uključujući opis svakog prekida i njegovog prioriteta  
-  Provesti testove kako bi se osiguralo pravilno reagiranje sustava na različite događaje  

## 6. Dodatni zahtjevi (po izboru jedan)  
-  Implementirati mehanizam detekcije i rješavanja situacija gdje se višestruki prekidi javljaju istovremeno  
-  Koristiti DMA (Direct Memory Access) ako je dostupno kako bi se smanjio utjecaj prekida na glavni procesor  
