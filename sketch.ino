/*
 * Pametni sustav za upravljanje roletama
 * 
 * @brief Glavni program za upravljanje roletama na temelju senzora i RTC-a
 * 
 * Kratki opis:
 * Ovaj sustav automatski upravlja roletama na temelju svjetline, temperature
 * i doba dana. Također omogućuje ručno upravljanje preko tipki.
 */
 
#include <Wire.h>
#include <RTClib.h>
#include <DHT.h>
#include <Servo.h>

/**
 * @brief Konfiguracija pinova i pragova sustava
 * 
 * Ovdje se definiraju svi pinovi i parametri potrebni za rad sustava.
 * Potrebno je prilagoditi vrijednosti prema stvarnoj instalaciji.
 */
#define LDR_PIN A0       // Analogni pin za LDR senzor svjetline
#define DHT_PIN 2        // Digitalni pin za DHT22 senzor
#define SERVO_PIN 3      // PWM pin za servo motor
#define BUTTON_UP 5      // Pin za tipku za podizanje
#define BUTTON_DOWN 6    // Pin za tipku za spuštanje
#define BUTTON_AUTO 7    // Pin za prebacivanje auto/ručni način rada
#define DHT_TYPE DHT22   // Tip DHT senzora

#define LIGHT_THRESHOLD 500  // Prag svjetline za automatsko upravljanje
#define DAY_START 9          // 9:00 - početak dnevnog načina rada
#define NIGHT_START 22       // 22:00 - početak noćnog načina rada
#define TEMP_HIGH 25         // Gornji prag temperature (°C)
#define TEMP_LOW 18          // Donji prag temperature (°C)
#define SERVO_UP 0           // Pozicija servo motora za podignute rolete
#define SERVO_DOWN 90        // Pozicija servo motora za spuštene rolete

// Globalne varijable
DHT dht(DHT_PIN, DHT_TYPE);
RTC_DS3231 rtc;
Servo rollerServo;
bool autoMode = true;        // Početni način rada (automatski)
int currentPosition = 0;     // Trenutna pozicija: 0=podignuto, 90=spušteno

/**
 * @brief Inicijalizacija sustava
 * 
 * Funkcija koja se izvršava jednom pri pokretanju.
 * Postavlja sve potrebne parametre i inicijalizira uređaje.
 */
void setup() {
  Serial.begin(9600);
  delay(2000);
  // Inicijalizacija pinova
  rollerServo.attach(SERVO_PIN);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_AUTO, INPUT_PULLUP);
  
  // Inicijalizacija senzora
  dht.begin();
  
  // Inicijalizacija RTC sata
  if (!rtc.begin()) {
    Serial.println("RTC modul nije pronađen");
    while (1);
  }
  
  if (rtc.lostPower()) {
    Serial.println("RTC izgubio napajanje, postavljam vrijeme!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  // Postavljanje početne pozicije
  rollerServo.write(SERVO_UP);
  currentPosition = SERVO_UP;
  
  Serial.println("Sustav spreman");
}

/**
 * @brief Glavna programska petlja
 * 
 * Funkcija koja se kontinuirano izvršava.
 * Obrađuje podatke sa senzora i upravlja roletama.
 */
void loop() {
  // Očitavanje senzora
  int lightLevel = analogRead(LDR_PIN);
  float temperature = dht.readTemperature();
  DateTime now = rtc.now();
  
  // Očitavanje stanja tipki
  bool btnUp = !digitalRead(BUTTON_UP);
  bool btnDown = !digitalRead(BUTTON_DOWN);
  bool btnAuto = !digitalRead(BUTTON_AUTO);
  
  /**
   * @brief Provjera načina rada
   * 
   * Ako je pritisnuta tipka za automatski način rada,
   * mijenja se trenutni način rada.
   */
  if (btnAuto) {
    autoMode = !autoMode;
    Serial.print("Automatski način: ");
    Serial.println(autoMode ? "UKLJUČEN" : "ISKLJUČEN");
    delay(300); // Debounce za tipku
  }
  
  // Ručno upravljanje
  if (!autoMode) {
    if (btnUp) {
      raiseRoller();
    } else if (btnDown) {
      lowerRoller();
    }
  } 
  // Automatsko upravljanje
  else {
    bool shouldLower = false;
    bool shouldRaise = false;
    
    // Provjera vremena
    if (now.hour() >= NIGHT_START || now.hour() < DAY_START) {
      shouldLower = true; // Noć - spusti rolete
    } else {
      // Dan - provjera svjetline i temperature
      if (lightLevel < LIGHT_THRESHOLD && temperature < TEMP_HIGH+2) {
        shouldRaise = true; // Slaba svjetlost i nije prevruće - podigni
      } else if (temperature > TEMP_HIGH+5 || lightLevel > LIGHT_THRESHOLD*2) {
        shouldLower = true; // Previsoka temperatura ili jako svijetlo - spusti
      }
    }
    
    // Upravljanje roletama
    if (shouldRaise && currentPosition != SERVO_UP) {
      raiseRoller();
    } else if (shouldLower && currentPosition != SERVO_DOWN) {
      lowerRoller();
    }
  }
  
  // Ispis statusa
  Serial.print("Vrijeme: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print("| Svjetlina: ");
  Serial.print(lightLevel);
  Serial.print("| Temp: ");
  Serial.print(temperature);
  Serial.print("°C | Način rada: ");
  Serial.print(autoMode ? "Auto" : "Ručno");
  Serial.print(" Pozicija rolete: ");
  Serial.println(currentPosition == SERVO_UP ? "Podignuta" : "Spuštena");
  
  delay(1000);
}

/**
 * @brief Podizanje roleta
 * 
 * Funkcija koja postavlja servo motor u poziciju
 * za podignute rolete i ažurira trenutno stanje.
 */
void raiseRoller() {
  rollerServo.write(SERVO_UP);
  currentPosition = SERVO_UP;
  Serial.println("Rolete podignute");
}

/**
 * @brief Spuštanje roleta
 * 
 * Funkcija koja postavlja servo motor u poziciju
 * za spuštene rolete i ažurira trenutno stanje.
 */
void lowerRoller() {
  rollerServo.write(SERVO_DOWN);
  currentPosition = SERVO_DOWN;
  Serial.println("Rolete spuštene");
}
