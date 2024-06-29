// Filename:    Lab2Part2
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        06/25/24
// Description: 

#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/timer_group_reg.h"

#define LED_PIN 5 // LED connected to D2

unsigned long lastToggleTime = 0; // Last time the LED was toggled
const unsigned long interval = 1000000; // 1 second in microseconds

void setup() {
 // Initialize the LED pin as an output
 PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[LED_PIN], PIN_FUNC_GPIO);
 *((volatile uint32_t*) GPIO_ENABLE_REG) |= (1 << LED_PIN);

 *((volatile uint32_t*) GPIO_OUT_REG) &= ~(1 << LED_PIN); // Ensure the LED is off to start
 
 *((volatile uint32_t*) TIMG_T0CONFIG_REG(0)) |= 1110000000001010000 << 13;
 
}
void loop() {
  *((volatile uint32_t*) TIMG_T0UPDATE_REG(0)) +=1;
  unsigned long currentTime = *((volatile uint32_t*) TIMG_T0LO_REG(0); // Get the current timer count
 
  // Check if the interval has passed
  if (currentTime - lastToggleTime >= interval) {
    *((volatile uint32_t*) GPIO_OUT_REG) ^= (1 << LED_PIN); // Toggle the LED state
    lastToggleTime = currentTime; // Update the last toggle time
  }
}

