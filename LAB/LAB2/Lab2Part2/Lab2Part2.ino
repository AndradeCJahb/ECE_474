/**
 * @file Lab2Part2.ino
 * @brief Toggles an off-board LED on and off at 1 second intervals by directly accessing TIMG and GPIO registers
 *
 * This file contains code to toggle an off-board LED connected to pin D2 (GPIO 5) at 1 second intervals using direct register access on an ESP32 microcontroller.
 * 
 * @date 06/25/24
 * @authors Christopher Andrade, Theo Favour
 */

#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/timer_group_reg.h"

#define LED_PIN 5  /**< GPIO pin number for the LED (Pin D2) */

unsigned long lastToggleTime = 0;        /**< Last time the LED was toggled */
const unsigned long interval = 1000000;  /**< 1 second interval in microseconds */

/**
 * @brief Setup function to configure GPIO pin and timer
 *
 * This function sets up the GPIO pin to be used as an output pin and configures the timer.
 */
void setup() {
  // Initialize the LED pin (D2) as an output
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[LED_PIN], PIN_FUNC_GPIO);
  *((volatile uint32_t*) GPIO_ENABLE_REG) |= (1 << LED_PIN);  // Set pin 5 (D2) to output
  *((volatile uint32_t*) GPIO_OUT_REG) &= ~(1 << LED_PIN);    // Ensure the LED (D2) is off to start

  // Configure timer: enable, increment counter, divider of 80
  uint32_t config_value = 0;
  config_value |= 1 << 31;  // Set enable flag
  config_value |= 1 << 30;  // Set increment flag
  config_value |= 80 << 13; // Set divider to 80

  // Clear upper 19 bits and set the timer configuration
  *((volatile uint32_t*) TIMG_T0CONFIG_REG(0)) &= 0x00001FFF;
  *((volatile uint32_t*) TIMG_T0CONFIG_REG(0)) |= config_value;
}

/**
 * @brief Loop function to toggle the LED at 1 second intervals
 *
 * This function toggles the LED state between HIGH and LOW every 1 second by checking the timer value.
 */
void loop() { 
  *((volatile uint32_t*) TIMG_T0UPDATE_REG(0)) += 1;                    // Update T0LO register
  unsigned long currentTime = *((volatile uint32_t*) TIMG_T0LO_REG(0)); // Get the current timer count

  if (currentTime - lastToggleTime >= interval) {                       // Check if the interval has passed
    *((volatile uint32_t*) GPIO_OUT_REG) ^= (1 << LED_PIN);             // Toggle the LED state (on/off)
    lastToggleTime = currentTime;                                       // Update the last toggle time
  }
}
