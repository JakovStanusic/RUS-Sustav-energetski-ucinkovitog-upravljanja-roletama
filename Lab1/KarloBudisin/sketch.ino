#include <Arduino.h>


const int BUTTON_HIGH_PIN = 2;    
const int BUTTON_MEDIUM_PIN = 3; 
const int LED_HIGH_PIN = 13;      
const int LED_MEDIUM_PIN = 12;    
const int LED_SENSOR_PIN = 11;    
const int TRIG_PIN = 7;           
const int ECHO_PIN = 6;          


volatile bool highPriorityEvent = false;
volatile bool mediumPriorityEvent = false;
volatile unsigned long lastHighTime = 0;
volatile unsigned long lastMediumTime = 0;
const int debounceDelay = 50;


unsigned long ledHighTimer = 0;
unsigned long ledMediumTimer = 0;
unsigned long ledSensorTimer = 0;
const int blinkDuration = 1000;


unsigned long lastSensorCheck = 0;
const int sensorCheckInterval = 100;
bool objectDetected = false;


void highPriorityISR() {
  if (millis() - lastHighTime > debounceDelay) {
    highPriorityEvent = true;
    lastHighTime = millis();
  }
}

void mediumPriorityISR() {
  if (millis() - lastMediumTime > debounceDelay) {
    mediumPriorityEvent = true;
    lastMediumTime = millis();
  }
}


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
  
  pinMode(BUTTON_HIGH_PIN, INPUT_PULLUP);
  pinMode(BUTTON_MEDIUM_PIN, INPUT_PULLUP);
  pinMode(LED_HIGH_PIN, OUTPUT);
  pinMode(LED_MEDIUM_PIN, OUTPUT);
  pinMode(LED_SENSOR_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

 
  attachInterrupt(digitalPinToInterrupt(BUTTON_HIGH_PIN), highPriorityISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_MEDIUM_PIN), mediumPriorityISR, FALLING);

  
  digitalWrite(LED_HIGH_PIN, HIGH);
  digitalWrite(LED_MEDIUM_PIN, HIGH);
  digitalWrite(LED_SENSOR_PIN, HIGH);
  delay(200);
  digitalWrite(LED_HIGH_PIN, LOW);
  digitalWrite(LED_MEDIUM_PIN, LOW);
  digitalWrite(LED_SENSOR_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

 
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

 
  if (!highPriorityEvent && !mediumPriorityEvent) {
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

  
  if (currentMillis - ledHighTimer > blinkDuration) {
    digitalWrite(LED_HIGH_PIN, LOW);
  }
  if (currentMillis - ledMediumTimer > blinkDuration) {
    digitalWrite(LED_MEDIUM_PIN, LOW);
  }
}