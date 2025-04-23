#include <Wire.h>
#include <RTClib.h>
#include <DHT.h>
#include <Servo.h>

// Definiranje pinova
#define LDR_PIN A0
#define DHT_PIN 2
#define SERVO_PIN 3
#define BUTTON_UP 5
#define BUTTON_DOWN 6
#define BUTTON_AUTO 7
#define DHT_TYPE DHT22

// Pragovi
#define LIGHT_THRESHOLD 500
#define DAY_START 7  // 7:00
#define NIGHT_START 20 // 20:00
#define TEMP_HIGH 25  // °C
#define TEMP_LOW 18   // °C
#define SERVO_UP 0    // Servo pozicija za podignute rolete
#define SERVO_DOWN 90 // Servo pozicija za spuštene rolete

// Globalne varijable
DHT dht(DHT_PIN, DHT_TYPE);
RTC_DS3231 rtc;
Servo rollerServo;
bool autoMode = true;
int currentPosition = 0; // 0=podignuto, 90=spušteno

void setup() {
  Serial.begin(9600);
  
  // Inicijalizacija pinova
  rollerServo.attach(SERVO_PIN);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_AUTO, INPUT_PULLUP);
  
  // Inicijalizacija senzora
  dht.begin();
  
  // Inicijalizacija RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  // Postavi početnu poziciju
  rollerServo.write(SERVO_UP);
  currentPosition = SERVO_UP;
  
  Serial.println("System ready");
}

void loop() {
  // Čitanje senzora
  int lightLevel = analogRead(LDR_PIN);
  float temperature = dht.readTemperature();
  DateTime now = rtc.now();
  
  // Čitanje tipkala
  bool btnUp = !digitalRead(BUTTON_UP);
  bool btnDown = !digitalRead(BUTTON_DOWN);
  bool btnAuto = !digitalRead(BUTTON_AUTO);
  
  // Provjera automatskog načina rada
  if (btnAuto) {
    autoMode = !autoMode;
    Serial.print("Auto mode ");
    Serial.println(autoMode ? "ON" : "OFF");
    delay(300); // Debounce
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
    // Logika za automatsko upravljanje
    bool shouldLower = false;
    bool shouldRaise = false;
    
    // Provjera vremena dana
    if (now.hour() >= NIGHT_START || now.hour() < DAY_START) {
      shouldLower = true; // Noć - spusti rolete
    } else {
      // Dan - provjeri svjetlinu i temperaturu
      if (lightLevel > LIGHT_THRESHOLD && temperature < TEMP_HIGH) {
        shouldRaise = true; // Svijetlo i nije previše toplo - podigni
      } else if (temperature > TEMP_HIGH || lightLevel < LIGHT_THRESHOLD/2) {
        shouldLower = true; // Previše toplo ili jako mračno - spusti
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
  Serial.print("Time: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(" Light: ");
  Serial.print(lightLevel);
  Serial.print(" Temp: ");
  Serial.print(temperature);
  Serial.print("C Mode: ");
  Serial.print(autoMode ? "Auto" : "Manual");
  Serial.print(" Position: ");
  Serial.println(currentPosition == SERVO_UP ? "UP" : "DOWN");
  
  delay(1000);
}

void raiseRoller() {
  rollerServo.write(SERVO_UP);
  currentPosition = SERVO_UP;
  Serial.println("Roller raised");
}

void lowerRoller() {
  rollerServo.write(SERVO_DOWN);
  currentPosition = SERVO_DOWN;
  Serial.println("Roller lowered");
}
