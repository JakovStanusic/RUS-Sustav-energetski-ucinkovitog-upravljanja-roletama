/**
 * @file Lab2.ino
 * @brief Arduino program for sleep mode control with watchdog timer and button wake-up
 * @author Karlo Budišin
 * @date 09/04/2025
 */

#include <avr/sleep.h>     ///< Library for sleep modes
#include <avr/wdt.h>       ///< Library for Watchdog Timer control
#include <avr/interrupt.h> ///< Library for interrupt handling

#define LED_PIN 13         ///< Pin number for the LED
#define BUTTON_PIN 2       ///< Pin number for the wake-up button

volatile bool isSleeping = true;     ///< Flag to determine sleep/wake state
volatile bool buttonPressed = false; ///< Flag to track button press for waking up

/**
 * @brief Interrupt service routine for button wake-up
 * 
 * This function is called when the button is pressed (falling edge interrupt).
 * Sets flags to wake up from sleep mode.
 */
void buttonWakeUp() {
  buttonPressed = true;
  isSleeping = false;  ///< Set flag to exit sleep mode
}

/**
 * @brief Watchdog Timer interrupt service routine
 * 
 * This function is called when the watchdog timer expires.
 * Sets flag to wake up from sleep mode.
 */
ISR(WDT_vect) {
  isSleeping = false;  ///< Set flag to exit sleep mode
}

/**
 * @brief Enters sleep mode
 * 
 * Prepares the system for sleep, enables sleep mode, and waits for an interrupt.
 * Disables sleep mode after waking up.
 */
void enterSleep() {
  noInterrupts();          ///< Disable interrupts to prepare for sleep
  isSleeping = true;       ///< Reset the sleeping flag
  buttonPressed = false;   ///< Reset button press flag
  sleep_enable();          ///< Enable sleep mode
  interrupts();            ///< Re-enable interrupts before sleeping
  sleep_cpu();             ///< Put the CPU to sleep

  while (isSleeping) {}    ///< Wait here until an interrupt occurs

  sleep_disable();         ///< Disable sleep after waking up
}

/**
 * @brief Configures the Watchdog Timer
 * 
 * Sets up the watchdog timer to generate an interrupt after ~4 seconds (WDP3).
 * The timer can wake the system from sleep mode.
 */
void configureWatchdog() {
  cli();           ///< Disable interrupts
  wdt_reset();     ///< Reset Watchdog Timer

  // Set WDT for ~4 seconds (WDP3 bit)
  WDTCSR = (1 << WDCE) | (1 << WDE);  ///< Enable configuration changes
  WDTCSR = (1 << WDIE) | (1 << WDP3); ///< Enable interrupt mode with 4s timeout

  sei();           ///< Re-enable interrupts
}

/**
 * @brief Initial setup function
 * 
 * Configures hardware pins, interrupts, sleep mode, and watchdog timer.
 */
void setup() {
  Serial.begin(9600);  ///< Initialize serial communication

  pinMode(LED_PIN, OUTPUT);      ///< Set LED pin as output
  pinMode(BUTTON_PIN, INPUT_PULLUP);  ///< Set button pin as input with pull-up

  // Attach external interrupt for button press (falling edge)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonWakeUp, FALLING);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); ///< Set power-down sleep mode
  configureWatchdog(); ///< Set up the watchdog timer
  Serial.println("System ready - LED ON");
}

/**
 * @brief Main program loop
 * 
 * Controls LED blinking, sleep mode entry, and wake-up handling.
 * The LED stays on for 5 seconds, then the system enters sleep mode.
 * Can be woken by either button press or watchdog timer.
 */
void loop() {
  // LED stays on for 5 seconds
  digitalWrite(LED_PIN, HIGH); 
  delay(5000);  // LED ON for 5 seconds

  digitalWrite(LED_PIN, LOW);  // LED OFF
  Serial.println("Sleep mode");

  // Enter sleep mode
  enterSleep();

  // Handle wake-up source
  if (buttonPressed) {
    Serial.println("Woke up by button - LED ON");
    digitalWrite(LED_PIN, HIGH);  // Turn on LED again
    buttonPressed = false;
    wdt_reset(); ///< Reset watchdog timer
  }
  else {
    Serial.println("Woke up by watchdog!");
  }
}
