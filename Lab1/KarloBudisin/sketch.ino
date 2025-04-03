/**
 * @brief Program za demonstraciju prekida s različitim prioritetima
 * @author Karlo Budišin
 * @date 03.04.2025
 * 
 * Program demonstrira rad s prekidima na Arduino UNO platformi,
 * uključujući prekide od tipkala, tajmera i senzora udaljenosti.
 * 
 */

#include <Arduino.h>

// Pinovi
const int BUTTON_HIGH_PIN = 2;    ///< Pin za crveno tipkalo
const int BUTTON_MEDIUM_PIN = 3;  ///< Pin za plavo tipkalo
const int LED_HIGH_PIN = 13;      ///< Pin za Crveni LED 
const int LED_MEDIUM_PIN = 12;    ///< Pin za Plavi LED 
const int LED_SENSOR_PIN = 11;    ///< Pin za Zeleni LED - senzor
const int TRIG_PIN = 7;           ///< Trigger pin za HC-SR04
const int ECHO_PIN = 6;           ///< Echo pin za HC-SR04
const int TIMER_LED_PIN = 10;     ///< Pin za Žuti LED - timer

// Globalne varijable
volatile bool highPriorityEvent = false;   ///< Flag za visok prioritet
volatile bool mediumPriorityEvent = false; ///< Flag za srednji prioritet
volatile bool timerEvent = false;          ///< Flag za prekid tajmera
volatile unsigned long lastHighTime = 0;   ///< Vrijeme zadnjeg visokog prioriteta
volatile unsigned long lastMediumTime = 0; ///< Vrijeme zadnjeg srednjeg prioriteta
const int debounceDelay = 50;              ///< Debounce vrijeme u ms

// Varijable za LED upravljanje
unsigned long ledHighTimer = 0;           ///< Timer za Crveni LED
unsigned long ledMediumTimer = 0;         ///< Timer za Plavi LED
unsigned long ledSensorTimer = 0;         ///< Timer za Zeleni LED
unsigned long ledTimerTimer = 0;          ///< Timer za Žuti LED
const int blinkDuration = 1000;           ///< Trajanje LED upaljenosti u ms

// Varijable za senzor
unsigned long lastSensorCheck = 0;        ///< Vrijeme zadnje provjere senzora
const int sensorCheckInterval = 100;      ///< Interval provjere senzora u ms
bool objectDetected = false;              ///< Status detekcije objekta

// Timer varijable
volatile unsigned long timerCounter = 0;  ///< Brojač prekida tajmera
const unsigned long timerInterval = 1000; ///< Interval tajmera u ms

/**
 * @brief ISR za visok prioritet
 * 
 * Postavljanje flag-a za visok prioritet nakon debounce provjere
 */
void highPriorityISR() {
  if (millis() - lastHighTime > debounceDelay) {
    highPriorityEvent = true;
    lastHighTime = millis();
  }
}

/**
 * @brief ISR za srednji prioritet
 * 
 * Postavljanje flag-a za srednji prioritet nakon debounce provjere
 */
void mediumPriorityISR() {
  if (millis() - lastMediumTime > debounceDelay) {
    mediumPriorityEvent = true;
    lastMediumTime = millis();
  }
}

/**
 * @brief ISR za tajmer
 * 
 * Postavljanje flag-a za prekid tajmera svaku sekundu
 */
void timerISR() {
  static unsigned long lastTimerTime = 0;
  unsigned long currentTime = millis();
  
  if (currentTime - lastTimerTime >= timerInterval) {
    timerEvent = true;
    lastTimerTime = currentTime;
    timerCounter++;
  }
}

/**
 * @brief Mjeri udaljenost pomoću HC-SR04 senzora
 * @return float Udaljenost u cm
 */
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

void setup() {
  // Inicijalizacija pinova
  pinMode(BUTTON_HIGH_PIN, INPUT_PULLUP);
  pinMode(BUTTON_MEDIUM_PIN, INPUT_PULLUP);
  pinMode(LED_HIGH_PIN, OUTPUT);
  pinMode(LED_MEDIUM_PIN, OUTPUT);
  pinMode(LED_SENSOR_PIN, OUTPUT);
  pinMode(TIMER_LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Postavljanje prekida
  attachInterrupt(digitalPinToInterrupt(BUTTON_HIGH_PIN), highPriorityISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_MEDIUM_PIN), mediumPriorityISR, FALLING);

  // Postavljanje tajmera
  cli(); // Onemogući prekide
  TCCR1A = 0; // Resetiraj tajmer 1
  TCCR1B = 0;
  OCR1A = 15624; // Postavi compare registar za 1Hz
  TCCR1B |= (1 << WGM12); // CTC mod
  TCCR1B |= (1 << CS12) | (1 << CS10); 
  TIMSK1 |= (1 << OCIE1A); // Omogući compare interrupt
  sei(); // Omogući prekide

  // Test LED-ova
  digitalWrite(LED_HIGH_PIN, HIGH);
  digitalWrite(LED_MEDIUM_PIN, HIGH);
  digitalWrite(LED_SENSOR_PIN, HIGH);
  digitalWrite(TIMER_LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_HIGH_PIN, LOW);
  digitalWrite(LED_MEDIUM_PIN, LOW);
  digitalWrite(LED_SENSOR_PIN, LOW);
  digitalWrite(TIMER_LED_PIN, LOW);
}

/**
 * @brief Compare match interrupt za tajmer 1
 */
ISR(TIMER1_COMPA_vect) {
  timerISR();
}

void loop() {
  unsigned long currentMillis = millis();

  // Obrada prekida po prioritetima
  // 1. Timer (najviši prioritet)
  if (timerEvent) {
    timerEvent = false;
    digitalWrite(TIMER_LED_PIN, HIGH);
    ledTimerTimer = currentMillis;
  }

  // 2. Visoki prioritet (crveno tipkalo)
  if (highPriorityEvent && !timerEvent) {
    highPriorityEvent = false;
    digitalWrite(LED_HIGH_PIN, HIGH);
    digitalWrite(LED_SENSOR_PIN, LOW);
    ledHighTimer = currentMillis;
  }

  // 3. Srednji prioritet (plavo tipkalo)
  if (mediumPriorityEvent && !timerEvent && !highPriorityEvent) {
    mediumPriorityEvent = false;
    digitalWrite(LED_MEDIUM_PIN, HIGH);
    digitalWrite(LED_SENSOR_PIN, LOW);
    ledMediumTimer = currentMillis;
  }

  // 4. Senzor udaljenosti (najniži prioritet)
  if (!timerEvent && !highPriorityEvent && !mediumPriorityEvent) {
    if (currentMillis - lastSensorCheck > sensorCheckInterval) {
      lastSensorCheck = currentMillis;
      float distance = measureDistance();
      
      if (distance < 100.0) {
        objectDetected = true;
        digitalWrite(LED_SENSOR_PIN, HIGH);
        ledSensorTimer = currentMillis;
      } else {
        objectDetected = false;
        digitalWrite(LED_SENSOR_PIN, LOW);
      }
    }
  }

  // Gašenje LED-ova nakon određenog vremena
  if (currentMillis - ledHighTimer > blinkDuration) {
    digitalWrite(LED_HIGH_PIN, LOW);
  }
  if (currentMillis - ledMediumTimer > blinkDuration) {
    digitalWrite(LED_MEDIUM_PIN, LOW);
  }
  if (currentMillis - ledTimerTimer > blinkDuration) {
    digitalWrite(TIMER_LED_PIN, LOW);
  }
}
