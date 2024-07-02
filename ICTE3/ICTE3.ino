// Filename:    ICTE3.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/01/24
// Description: This file toggles an off-board LED on and off utilizing a counter variable

#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/timer_group_reg.h"

#define LED_PIN 5       // LED connected to D2

int counter;            // Static counter delay to create delay          

void setup() {
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[LED_PIN], PIN_FUNC_GPIO);
  *((volatile uint32_t*) GPIO_ENABLE_REG) |= (1 << LED_PIN);      // set pin 5 (D2) to output
}

void loop() {
  counter += 1;                                                   // increment counter
  if (counter == 1000000) {
    *((volatile uint32_t*) GPIO_OUT_REG) ^= (1 << LED_PIN);       // Toggle the LED state
    counter = 0;                                                  // Reset Counter
  }
}
