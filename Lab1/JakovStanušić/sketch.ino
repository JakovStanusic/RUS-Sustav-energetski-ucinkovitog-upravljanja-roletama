#include <Arduino.h>

const int BUTTON0_PIN = 2;    // INT0 (Najviši prioritet)
const int BUTTON1_PIN = 3;    // INT1 (Srednji prioritet)
const int BUTTON2_PIN = 4;    //Polling (Najniži prioritet)

const int LED_INT0 = 13;
const int LED_INT1 = 12;
const int LED_INT2 = 11;
const int LED_TIMER = 10;
const int LED_SENSOR = 9;
const int POTENTIOMETER_PIN = A0;  // Potenciometar na A0

// Button variables with debounce
volatile bool button0Pressed = false;
volatile bool button1Pressed = false;
bool button2Pressed = false;
unsigned long lastButton0Time = 0;
unsigned long lastButton1Time = 0;
unsigned long lastButton2Time = 0;
const int debounceDelay = 50;

// LED control variables
unsigned long ledTimer0 = 0;
unsigned long ledTimer1 = 0;
unsigned long ledTimer2 = 0;
unsigned long ledTimerSensor = 0;
const int blinkDuration = 1000;
const int sensorBlinkInterval = 200;

// Timer control
volatile bool timerFlag = false;
unsigned long previousMillis = 0;
const long timerInterval = 1000;

// Potenciometar control
unsigned long lastPotCheck = 0;
const int potCheckInterval = 100;
const int potThreshold = 500;
bool thresholdCrossed = false;

// Interrupt Service Routines
void button0ISR() {
  if (millis() - lastButton0Time > debounceDelay) {
    button0Pressed = true;
    lastButton0Time = millis();
  }
}

void button1ISR() {
  if (millis() - lastButton1Time > debounceDelay) {
    button1Pressed = true;
    lastButton1Time = millis();
  }
}

ISR(TIMER1_COMPA_vect) {
  timerFlag = true;
}

void setup() {
  pinMode(BUTTON0_PIN, INPUT_PULLUP);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(LED_INT0, OUTPUT);
  pinMode(LED_INT1, OUTPUT);
  pinMode(LED_INT2, OUTPUT);
  pinMode(LED_TIMER, OUTPUT);
  pinMode(LED_SENSOR, OUTPUT);
  pinMode(POTENTIOMETER_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON0_PIN), button0ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), button1ISR, FALLING);


  // Configure Timer1
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  interrupts();

  // Startup sequence
  digitalWrite(LED_INT0, HIGH);
  digitalWrite(LED_INT1, HIGH);
  digitalWrite(LED_INT2, HIGH);
  digitalWrite(LED_TIMER, HIGH);
  digitalWrite(LED_SENSOR, HIGH);
  delay(200);
  digitalWrite(LED_INT0, LOW);
  digitalWrite(LED_INT1, LOW);
  digitalWrite(LED_INT2, LOW);
  digitalWrite(LED_TIMER, LOW);
  digitalWrite(LED_SENSOR, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  // Handle timer interrupt
  if (timerFlag) {
    timerFlag = false;
    digitalWrite(LED_TIMER, HIGH);
    delay(50);
    digitalWrite(LED_TIMER, LOW);
  }

  // Handle button 2
  if (digitalRead(BUTTON2_PIN) == LOW && currentMillis - lastButton2Time > debounceDelay) {
    button2Pressed = true;
    lastButton2Time = currentMillis;
  }

  // Process button presses
  if (button0Pressed) {
    button0Pressed = false;
    ledTimer0 = currentMillis;
    digitalWrite(LED_INT0, HIGH);
        delay(300);
  }else if (button1Pressed) {
    button1Pressed = false;
    ledTimer1 = currentMillis;
    digitalWrite(LED_INT1, HIGH);
        delay(300);
  }else if (currentMillis - lastPotCheck > potCheckInterval) {
    lastPotCheck = currentMillis;
    int potValue = analogRead(POTENTIOMETER_PIN);
    
    if (potValue > potThreshold) {
      if (!thresholdCrossed) {
        thresholdCrossed = true;
        ledTimerSensor = currentMillis;
        digitalWrite(LED_SENSOR, HIGH);
        delay(300);
      } else if (currentMillis - ledTimerSensor > sensorBlinkInterval) {
        ledTimerSensor = currentMillis;
        digitalWrite(LED_SENSOR, !digitalRead(LED_SENSOR));
      }
    } else {
      thresholdCrossed = false;
      digitalWrite(LED_SENSOR, LOW);
    }
  }else if (button2Pressed) {
    button2Pressed = false;
    ledTimer2 = currentMillis;
    digitalWrite(LED_INT2, HIGH);
        delay(300);
  }

  // Handle LED blinking
  if (currentMillis - ledTimer0 < blinkDuration && currentMillis - ledTimer0 > 100) {
    digitalWrite(LED_INT0, LOW);
  }

  if (currentMillis - ledTimer1 < blinkDuration && currentMillis - ledTimer1 > 100) {
    digitalWrite(LED_INT1, LOW);
  }

  if (currentMillis - ledTimer2 < blinkDuration && currentMillis - ledTimer2 > 100) {
    digitalWrite(LED_INT2, LOW);
  }

  // Handle potenciometar
  
}