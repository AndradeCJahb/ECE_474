#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/timer_group_reg.h"

#define LED_PIN 5                       // LED connected to D2

int counter;

void setup() {
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[LED_PIN], PIN_FUNC_GPIO);
  *((volatile uint32_t*) GPIO_ENABLE_REG) |= (1 << LED_PIN);             // set pin 5 (D2) to output

}

void loop() {
  counter += 1;
  if (counter == 1000000) {
    *((volatile uint32_t*) GPIO_OUT_REG) ^= (1 << LED_PIN);             // Toggle the LED state
    counter = 0;
  }
}
