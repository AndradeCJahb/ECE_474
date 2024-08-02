/**
 * @file Lab2Part1Step2.ino
 * @brief Flashes an off-board LED at one second intervals utilizing direct register access
 *
 * This file contains code to flash an off-board LED connected to pin D2 (GPIO 5) at one second intervals using direct register access on an ESP32 microcontroller.
 * 
 * @author Christopher Andrade
 * @author Theo Favour
 * @date 06/25/24
 */

#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"

#define GPIO_PIN 5  /**< GPIO pin number for the LED (Pin D2) */

/**
 * @brief Setup function to configure GPIO pin
 *
 * This function sets up the GPIO pin to be used as an output pin.
 */
void setup() {
  // Set pin 5 (D2) to be a general purpose GPIO input/output
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[GPIO_PIN], PIN_FUNC_GPIO);
  
  // Set pin 5 (D2) to be an output by shifting a '1' into bit 5 of the enable register
  *((volatile uint32_t*) GPIO_ENABLE_REG) |= (1 << GPIO_PIN);
}

/**
 * @brief Loop function to toggle LED state
 *
 * This function toggles the LED state between HIGH and LOW with a delay of one second.
 */
void loop() {
  // Set pin 5 (D2) to output HIGH by shifting '1' into bit 5 of the output register
  *((volatile uint32_t*) GPIO_OUT_REG) |= (1 << GPIO_PIN);
  delay(1000);

  // Set pin 5 (D2) to output LOW by shifting '0' into bit 5 of the output register
  *((volatile uint32_t*) GPIO_OUT_REG) &= ~(1 << GPIO_PIN);
  delay(1000);
}
