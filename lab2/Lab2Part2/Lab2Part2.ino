// Filename:    Lab2Part1Step2.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        06/25/24
// Description: 

#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/timer_group_reg.h"

#define GPIO_PIN 5 // Pin D2

#define LED_PIN 2 // LED connected to digital pin 2
hw_timer_t * timer = NULL; // declare a timer object and initialize to null
unsigned long lastToggleTime = 0; // Last time the LED was toggled
const unsigned long interval = 1000000; // 1 second in microseconds


void setup() {
 // Initialize the LED pin as an output
 pinMode(LED_PIN, OUTPUT);
 digitalWrite(LED_PIN, LOW); // Ensure the LED is off to start
 timer = timerBegin(0, 80, true); // Timer 0, prescaler 80, count up from 0
 timerStart(timer); // Start the timer
}
void loop() {
 unsigned long currentTime = timerRead(timer); // Get the current timer count
 // Check if the interval has passed
 if (currentTime - lastToggleTime >= interval) {
   digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Toggle the LED state
   lastToggleTime = currentTime; // Update the last toggle time
 }
}

TIMG_T0LO_REG(0): This register holds the current value of the lower 32 bits of Timer 0. By reading this register, we can determine how much time has passed since the timer started.
TIMG_T0UPDATE_REG(0): Writing to this register triggers an update, ensuring that the current timer value is copied to the TIMG_T0LO_REG(0) register for us to read.
