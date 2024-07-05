// Filename:    Lab2Part3.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        06/28/24
// Description: This file changes the brightness level of an external LED based on the ambient light values from a photoresistor

void setup() {
  // Initialize pin D2 as output (to LED), initialize A0 as input.
  pinMode(D2, OUTPUT);
  pinMode(A0, INPUT);

  ledcSetup(0, 1000, 12); // PWM channel 0, arbitrary frequency of 1000 Hz, resolution of 12 bits (analog read maxes at 4095, also 12 bits)
  ledcAttachPin(D2, 0);   // Attach pin D2 (external LED) to PWM channel 0
}

void loop() {
  ledcWrite(0, (analogRead(A0) - 2048) * 2);  // Sets duty rate of channel 0 (external LED) to A0's analog read (0-4095) with some scaling to be more "reactive"
}
