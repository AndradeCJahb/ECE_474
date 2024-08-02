/**
 * @file Lab2Part1Step3ALF.ino
 * @brief Measures and outputs the time to switch a GPIO output from HIGH to LOW using Arduino Library Functions
 *
 * This file contains code to measure and output the amount of time (in microseconds) it takes to switch a GPIO output from HIGH to LOW using the Arduino Library function digitalWrite.
 * 
 * @date 06/25/24
 * @authors Christopher Andrade, Theo Favour
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
 * @brief Loop function to measure and output time for GPIO toggle
 *
 * This function measures the time taken to toggle the GPIO output 1000 times and outputs the time taken in microseconds.
 */
void loop() {
  // Obtain start time for this loop of 1000 digital writes
  long startTime = micros();

  // Toggle D2 output 1000 times
  for (int i = 0; i < 1000; i++) {
    digitalWrite(GPIO_PIN, HIGH);  // Set pin 5 (D2) to output HIGH using digitalWrite
    digitalWrite(GPIO_PIN, LOW);   // Set pin 5 (D2) to output LOW using digitalWrite
  }

  // Calculate and output the amount of time it took to digital write HIGH/LOW 1000 times
  Serial.println(micros() - startTime);
  delay(1000);
}
