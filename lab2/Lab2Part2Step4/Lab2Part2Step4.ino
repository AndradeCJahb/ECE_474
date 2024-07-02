// Filename:    Lab2Step4.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        06/25/24
// Description: This file toggles an off-board LED on and off at 1 second intervals directly accessing TIMG and GPIO registers

#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/timer_group_reg.h"

#define LED_PIN 5                       // LED connected to D2

unsigned long lastToggleTime = 0;       // Last time the LED was toggled
const unsigned long interval = 1000000; // 1 second in microseconds

void setup() {
  // Initialize the LED pin (D2) as an output
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[LED_PIN], PIN_FUNC_GPIO);
  *((volatile uint32_t*) GPIO_ENABLE_REG) |= (1 << LED_PIN);             // set pin 5 (D2) to output
  *((volatile uint32_t*) GPIO_OUT_REG) &= ~(1 << LED_PIN);               // Ensure the LED (D2) is off to start

  // Configure timer to be enabled, have an incrementing counter, and have a divider of 80
  uint32_t config_value = 0;
  config_value |= 1 << 31;              // set enable flag
  config_value |= 1 << 30;              // set increase flag
  config_value |= 80 << 13;             // set divider to 80

  // Clear upper 19 bits, then replace the bits with the previously built config_value
  *((volatile uint32_t*) TIMG_T0CONFIG_REG(0)) &= 00000000000000000001111111111111;
  *((volatile uint32_t*) TIMG_T0CONFIG_REG(0)) |= config_value;
}

void loop() { 
  *((volatile uint32_t*) TIMG_T0UPDATE_REG(0)) += 1;                    // Write to T0Update register to update T0LO register
  unsigned long currentTime = *((volatile uint32_t*) TIMG_T0LO_REG(0)); // Get the current timer count
  // Check if the interval has passed
  if (currentTime - lastToggleTime >= interval) {
    *((volatile uint32_t*) GPIO_OUT_REG) ^= (1 << LED_PIN);             // Toggle the LED state
    lastToggleTime = currentTime;                                       // Update the last toggle time
  }
}
