/**
 * @file Lab2Part3.ino
 * @brief Changes the brightness level of an external LED based on ambient light values from a photoresistor
 *
 * This file contains code to adjust the brightness of an external LED connected to pin D2 based on the ambient light values read from a photoresistor connected to pin A0.
 * 
 * @date 06/28/24
 * @authors Christopher Andrade, Theo Favour
 */

/**
 * @brief Setup function to configure pins and PWM
 *
 * This function initializes pin D2 as an output (to LED) and pin A0 as an input (from photoresistor). It also sets up PWM on channel 0.
 */
void setup() {
  // Initialize pin D2 as output (to LED) and pin A0 as input (from photoresistor)
  pinMode(D2, OUTPUT);
  pinMode(A0, INPUT);

  // PWM setup: channel 0, frequency of 1000 Hz, resolution of 12 bits
  ledcSetup(0, 1000, 12);
  // Attach pin D2 (external LED) to PWM channel 0
  ledcAttachPin(D2, 0);
}

/**
 * @brief Loop function to adjust LED brightness based on photoresistor value
 *
 * This function reads the ambient light value from the photoresistor and adjusts the duty cycle of the PWM signal to change the LED brightness accordingly.
 */
void loop() {
  // Set the duty cycle of PWM channel 0 based on the analog read value from A0 (scaled to be more reactive)
  ledcWrite(0, (analogRead(A0) - 2048) * 2);
}
