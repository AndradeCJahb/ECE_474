// Filename:    Lab2Part1Step3DRA.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        06/25/24
// Description: This file outputs the length of time (in microseconds) it takes to switch an GPIO output from HIGH to LO

#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"

#define GPIO_PIN 5 // Pin D2

void setup() {
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[GPIO_PIN], PIN_FUNC_GPIO);           // set pin 5 (D2) to be a general purpose input/output
  *((volatile uint32_t*) GPIO_ENABLE_REG) |= (1 << GPIO_PIN);           // set pin 5 (D2) to be an output by moving a '1' into bit 5 of enable register
}


void loop() {
  long startTime = micros();
  for (int i = 0; i < 1000; i++) {
    *((volatile uint32_t*) GPIO_OUT_REG) |= (1 << GPIO_PIN);            // set pin 5 (D2) to output HIGH by moving '1' into bit 5 of out register
    *((volatile uint32_t*) GPIO_OUT_REG) &= ~(1 << GPIO_PIN);           // set pin 5 (D2) to output LOW by moving '0' into bit 5 of out register
  }
  Serial.println(Serial.print(micros() - startTime));
  delay(1000);
}
