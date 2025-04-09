/**
 * @file Lab2.ino
 * @brief Primjer Arduino koda za sleep mode i buđenje prekidom
 * 
 * Demonstrira korištenje sleep moda i buđenje prekidom s tipkala.
 * LED se pali nakon buđenja ili nakon isteka vremena (8 sekundi).
 */

const int buttonPin = 2;     ///< Pin na kojem je spojeno tipkalo (INT5)
const int ledPin = 13;       ///< Pin na kojem je spojena LED dioda
volatile bool wakeUp = false; ///< Zastavica za prekid (postavlja se u ISR)
unsigned long sleepStartTime; ///< Vrijeme ulaska u sleep mode
bool inSleepMode = false;     ///< Status sleep moda (true = u sleep modu)

/**
 * @brief Inicijalizacija sustava
 * 
 * Postavlja pinove, serijsku komunikaciju i prekid.
 * Paljenje LED-a na 5 sekundi pri pokretanju.
 */
void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(buttonPin), wakeUpISR, FALLING);
  
  Serial.println("Sustav pokrenut. LED upaljen na 5 sekundi...");
  digitalWrite(ledPin, HIGH);
  delay(5000);
  enterSleepMode();
}

/**
 * @brief Glavna petlja programa
 * 
 * Provjerava je li potrebno probuditi sustav (prekidom ili istekom vremena)
 * i pali LED na 5 sekundi pri buđenju.
 */
void loop() {
  if (wakeUp || (inSleepMode && (millis() - sleepStartTime) > 8000)) {
    if (wakeUp) {
      Serial.println("Prekid detektiran! LED upaljen na 5 sekundi...");
    } else {
      Serial.println("Tajmer istekao! LED upaljen na 5 sekundi...");
    }
    
    digitalWrite(ledPin, HIGH);
    delay(5000);
    enterSleepMode();
    wakeUp = false;
  }
}

/**
 * @brief Prekidna rutina (ISR) za buđenje
 * 
 * Postavlja zastavicu wakeUp na true ako je proteklo dovoljno vremena
 * od zadnjeg prekida (debounce).
 */
void wakeUpISR() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  
  if (interruptTime - lastInterruptTime > 200) {
    wakeUp = true;
  }
  lastInterruptTime = interruptTime;
}

/**
 * @brief Ulazak u sleep mode
 * 
 * Gasenje LED-a, bilježenje vremena ulaska i postavljanje statusa.
 */
void enterSleepMode() {
  Serial.println("Ulazim u sleep mode (LED ugašen)...");
  digitalWrite(ledPin, LOW);
  sleepStartTime = millis();
  inSleepMode = true;
}
